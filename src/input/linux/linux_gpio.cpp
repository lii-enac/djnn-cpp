/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Chatty <chatty@djnn.net>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Sébastien Leriche <sebastien.leriche@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
/*  ---------------------------------------------------------------------
 *  PORTAGE libgpiod v2 (character device API) pour Alpine Linux 3.24+.
 *
 *  Remplace l'ancienne couche sysfs (/sys/class/gpio/...), incompatible
 *  avec certains contrôleurs modernes (ex: RP1 du Raspberry Pi 5), et
 *  N'UTILISE PAS l'API v1 de libgpiod (gpiod_chip_open_by_number, etc.)
 *  qui n'est plus celle fournie par le paquet Alpine `libgpiod`
 *  (so:libgpiod.so.3 = ABI v2).
 *
 *  Pré-requis :
 *    apk add libgpiod libgpiod-dev
 *    lien :  -lgpiod
 *
 *  ---------------------------------------------------------------------
 */
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <glob.h>
#include <gpiod.h>

#include "core/core-dev.h"
#include "core/utils/error.h"
#include "core/utils/to_string.h"
#include "core/utils/utils-dev.h"
#include "linux_input.h"

using namespace djnnstl;

namespace djnn {

static int                 num_gpios = 0;
static map<int, int> gpiochips;
static map<int, GPIOLine*> gpio_lines;

// ---------------------------------------------------------------------
// p_init_gpios : énumère /dev/gpiochip* via libgpiod v2.
// ---------------------------------------------------------------------
void
p_init_gpios ()
{
    int num = 0;
    glob_t globlist;

    if (glob ("/dev/gpiochip*", 0, NULL, &globlist)) {
        fprintf (stderr, "djnn warning: no /dev/gpiochip* found\n");
        return;
    }

    int i = 0;
    while (globlist.gl_pathv[i]) {
        const char* path = globlist.gl_pathv[i];

        // En v2, vérifier d'abord que le device est bien un gpiochip
        if (!gpiod_is_gpiochip_device (path)) {
            ++i;
            continue;
        }

        struct gpiod_chip* chip = gpiod_chip_open (path);
        if (!chip) {
            fprintf (stderr, "djnn warning: cannot open %s - %s\n", path, strerror (errno));
            ++i;
            continue;
        }

        struct gpiod_chip_info* info = gpiod_chip_get_info (chip);
        if (!info) {
            fprintf (stderr, "djnn warning: cannot get info for %s\n", path);
            gpiod_chip_close (chip);
            ++i;
            continue;
        }

        unsigned int numgpio = gpiod_chip_info_get_num_lines (info);

        const char* base = strstr (path, "gpiochip");
        int gpiochip_num = base ? atoi (base + strlen ("gpiochip")) : -1;

        if (gpiochip_num < 0) {
            fprintf (stderr, "djnn warning: could not extract gpiochip number from %s\n", path);
            gpiod_chip_info_free (info);
            gpiod_chip_close (chip);
            ++i;
            continue;
        }

        gpiochips[gpiochip_num] = numgpio;
        num += numgpio;

        gpiod_chip_info_free (info);
        gpiod_chip_close (chip);
        ++i;
    }
    globfree (&globlist);

    num_gpios = num - 1;
    if (num_gpios < 0)
        fprintf (stderr, "djnn warning: no GPIO\n");
}

CoreProcess*
p_find_gpio (const string& path, direction_e dir)
{
    try {
        string::size_type sz;
        int                index = std::stoi (path, &sz);

        for (auto gpiochip_it : gpiochips) {
            if (index > gpiochip_it.second) continue;

            int composite_key = gpiochip_it.first * 1000 + index;

            map<int, GPIOLine*>::iterator it   = gpio_lines.find (composite_key);
            GPIOLine*                     line = nullptr;

            if (it != gpio_lines.end ()) {
                line = it->second;
            } else {
                line = new GPIOLine (nullptr,
                                      "line" + djnnstl::to_string (composite_key),
                                      gpiochip_it.first,
                                      index,
                                      dir);
                line->activate ();
                gpio_lines[composite_key] = line;
            }

            if (path.length () > (sz + 1))
                return line->find_child_impl (path.substr ((sz + 1)));
            else
                return line;
        }
    } catch (std::invalid_argument& arg) {
        warning (nullptr, "invalid gpio path specification: " + path);
    } catch (std::exception& e) {
        warning (nullptr, "invalid gpio path specification: " + path + " " + e.what ());
    }
    
    return nullptr;
}

// ---------------------------------------------------------------------
// GPIOLine constructeur (API v2)
//
// En v2, on ne "request" plus une ligne isolément : on construit
//   - des line_settings (direction, edge detection)
//   - un line_config qui associe l'offset à ces settings
//   - une request_config (nom du consommateur)
// puis on appelle gpiod_chip_request_lines() pour obtenir un
// gpiod_line_request, à partir duquel on récupère un fd unique
// (gpiod_line_request_get_fd) réutilisable tel quel dans l'IOFD de djnn.
// ---------------------------------------------------------------------
GPIOLine::GPIOLine (CoreProcess* parent, const string& name,
                     int chip_number, int line_offset, direction_e dir)
    : FatProcess (name),
      _pin (line_offset),
      _dir (dir),
      _chip (nullptr),
      _request (nullptr),
      _event_buf (nullptr),
      _line_offset ((unsigned int) line_offset),
      _fd (-1),
      _iofd (nullptr),
      _action (nullptr),
      _c_action (nullptr)
{   
    _value = new BoolProperty (this, "value", true);

    char chip_path[32];
    snprintf (chip_path, sizeof (chip_path), "/dev/gpiochip%d", chip_number);

    _chip = gpiod_chip_open (chip_path);
    if (!_chip) {
        error (this, string ("unable to open ") + chip_path + " - " + strerror (errno));
    }

    struct gpiod_line_settings* settings = gpiod_line_settings_new ();
    if (!settings) {
        gpiod_chip_close (_chip);
        error (this, "gpiod_line_settings_new failed");
    }

    if (_dir == IN) {
        gpiod_line_settings_set_direction (settings, GPIOD_LINE_DIRECTION_INPUT);
        // équivalent de l'ancien "edge=both" en sysfs - c'est précisément
        // ce que l'ancienne implémentation sysfs ne supportait pas sur RP1
        gpiod_line_settings_set_edge_detection (settings, GPIOD_LINE_EDGE_BOTH);
    } else {
        gpiod_line_settings_set_direction (settings, GPIOD_LINE_DIRECTION_OUTPUT);
        gpiod_line_settings_set_output_value (settings, GPIOD_LINE_VALUE_INACTIVE);
    }

    struct gpiod_line_config* line_cfg = gpiod_line_config_new ();
    if (!line_cfg) {
        gpiod_line_settings_free (settings);
        gpiod_chip_close (_chip);
        error (this, "gpiod_line_config_new failed");
    }

    unsigned int offsets[1] = { _line_offset };
    if (gpiod_line_config_add_line_settings (line_cfg, offsets, 1, settings) < 0) {
        gpiod_line_config_free (line_cfg);
        gpiod_line_settings_free (settings);
        gpiod_chip_close (_chip);
        error (this, "gpiod_line_config_add_line_settings failed - " + string (strerror (errno)));
    }

    struct gpiod_request_config* req_cfg = gpiod_request_config_new ();
    if (req_cfg) {
        gpiod_request_config_set_consumer (req_cfg, "djnn");
    }

    _request = gpiod_chip_request_lines (_chip, req_cfg, line_cfg);

    if (req_cfg) gpiod_request_config_free (req_cfg);
    gpiod_line_config_free (line_cfg);
    gpiod_line_settings_free (settings);

    if (!_request) {
        gpiod_chip_close (_chip);
        error (this, "cannot request gpio line " + __to_string (line_offset)
                          + " on chip " + __to_string (chip_number)
                          + " - " + strerror (errno));
    }

    if (dir == IN) {
        _fd = gpiod_line_request_get_fd (_request);
        if (_fd < 0) {
            error (this, "cannot get request fd for gpio line "
                              + __to_string (line_offset) + " - " + strerror (errno));
        }
        // buffer réutilisé à chaque évènement (évite réallocation)
        _event_buf = gpiod_edge_event_buffer_new (1);

        _iofd = new IOFD (nullptr, "gpiofd", _fd);
        _iofd->activate ();

        _action   = new GPIOLineReadAction (this, "read");
        _c_action = new Coupling (_iofd->find_child_impl ("readable"), ACTIVATION, _action, ACTIVATION);
        //_c_action = new Coupling (_iofd->find_child_impl ("except"), ACTIVATION, _action, ACTIVATION);

    } else {
        _action   = new GPIOLineWriteAction (this, "write");
        _c_action = new Coupling (_value, ACTIVATION, _action, ACTIVATION);
    }

    finalize_construction (parent, name);
}

GPIOLine::~GPIOLine ()
{
    if (_dir == IN) {
        _iofd->deactivate ();
        delete _c_action;
        delete _action;
        delete _iofd;
        if (_event_buf)
            gpiod_edge_event_buffer_free (_event_buf);
    } else {
        delete _c_action;
        delete _action;
    }

    if (_request)
        gpiod_line_request_release (_request);
    if (_chip)
        gpiod_chip_close (_chip);

    delete _value;
}

// ---------------------------------------------------------------------
// read_value : lit l'évènement de front en attente sur le fd, puis relit
// l'état logique courant de la ligne.
// ---------------------------------------------------------------------
void
GPIOLine::read_value ()
{
    int n = gpiod_line_request_read_edge_events (_request, _event_buf, 1);
    if (n < 0) {
        warning (this, string ("gpiod_line_request_read_edge_events failed - ") + strerror (errno));
        return;
    }

    enum gpiod_line_value v = gpiod_line_request_get_value (_request, _line_offset);
    if (v == GPIOD_LINE_VALUE_ERROR) {
        warning (this, string ("gpiod_line_request_get_value failed - ") + strerror (errno));
        return;
    }

    _value->set_value (v == GPIOD_LINE_VALUE_ACTIVE ? 1 : 0, true);
}

// ---------------------------------------------------------------------
// write_value
// ---------------------------------------------------------------------
void
GPIOLine::write_value ()
{
    int value = _value->get_value ();
    enum gpiod_line_value gv = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

    if (gpiod_line_request_set_value (_request, _line_offset, gv) < 0) {
        warning (this, string ("gpiod_line_request_set_value failed - ") + strerror (errno));
    }
}

} // namespace djnn
