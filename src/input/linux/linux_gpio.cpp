/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Chatty <chatty@djnn.net>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <glob.h>

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"
#include "core/utils/to_string.h"
#include "core/utils/utils-dev.h"
#include "linux_input.h"

using namespace djnnstl;

namespace djnn {
static int                 num_gpios = 0;
static map<int, int> gpiochips;
static map<int, GPIOLine*> gpio_lines;

void
p_init_gpios ()
{
    int num = 0;
    glob_t globlist;
    if (glob("/sys/class/gpio/gpiochip*/ngpio", 0, NULL, &globlist)) {
        fprintf (stderr, "djnn warning: no /sys/class/gpio/gpiochip*/ngpio \n");
        return;
    }


    /* iterate on all controllers to determine the total number of GPIOs */
    int i=0;
    while (globlist.gl_pathv[i]) {
        const char* filename = globlist.gl_pathv[i];
        int         fd;
        char        buf[10];
        const char* p;
        int         numgpio = 0;

        /* attempt to read the information file for the next controller */
        //fprintf (stderr, "djnn warning: examining %s\n", filename);
        fd = open (filename, O_RDONLY);
        if (fd < 0)
            break;

        /* read the number of GPIOs in this controller */
        read (fd, buf, 10);
        for (p = buf; *p != '\n'; ++p)
            numgpio = 10 * numgpio + *p - '0';
        close (fd);

        auto beg = sizeof("/sys/class/gpio/gpiochip")-1;
        auto substr = strstr(&filename[beg], "/");
        if (substr==nullptr) {
            fprintf (stderr, "djnn warning: could not extract gpiochip number %s\n", filename);
            return;
        }

        int gpiochip = atoi(&filename[beg]);
        if (gpiochip==-1) {
            fprintf (stderr, "djnn warning: could not extract gpiochip number %s %s\n", filename, substr);
            return;
        }

        //fprintf(stderr, "%s %d %s %s %d %s:%d\n", filename, beg, &filename[beg], substr, gpiochip, __FILE__, __LINE__);

        gpiochips[gpiochip] = numgpio;

        /* update the total number of GPIOs */
        num += numgpio;
        ++i;
    }
    globfree(&globlist);

    num_gpios = num - 1;
    if (num_gpios < 0)
        fprintf (stderr, "djnn warning: no GPIO\n");
}

CoreProcess*
p_find_gpio (const string& path, direction_e dir)
{
    try {
        string::size_type             sz;
        int                        index = std::stoi (path, &sz);

        for(auto gpiochip_it: gpiochips) {
            if (index > gpiochip_it.second) continue;

            map<int, GPIOLine*>::iterator it    = gpio_lines.find (index);
            GPIOLine*                     line  = nullptr;
            if (it != gpio_lines.end ()) {
                line = it->second;
            } else {
                int based_index = index + gpiochip_it.first;
                //fprintf(stderr, "%d %s:%d\n", based_index, __FILE__, __LINE__);
                line = new GPIOLine (nullptr, "line" + djnnstl::to_string (based_index), based_index, dir);
                //fprintf(stderr, "%s %s:%d\n", line->get_debug_name().c_str(), __FILE__, __LINE__);
                line->activate ();
            }
            //fprintf(stderr, "%s %s:%d\n", line->get_debug_name().c_str(), __FILE__, __LINE__);
            if (path.length () > (sz + 1))
                return line->find_child_impl (path.substr ((sz + 1)));
            else
                return line;
        }
    } catch (std::invalid_argument& arg) {
        warning (nullptr, "invalid gpio path specification: " + path);
    } catch (std::exception& e) {
        warning (nullptr, "invalid gpio path specification: " + path + " " + e.what());
    }
    return nullptr;
}

GPIOLine::GPIOLine (CoreProcess* parent, const string& name, int pin, direction_e dir)
    : FatProcess (name),
      _pin (pin),
      _dir (dir),
      _iofd (nullptr),
      _action (nullptr),
      _c_action (nullptr)
{
    // if (pin < 0 || pin > num_gpios)
    //     error (this, "no gpio " + __to_string (pin));
    _value = new BoolProperty (this, "value", true);

    /* activate the GPIO interface */
    _fd = open ("/sys/class/gpio/export", O_WRONLY);
    char        buf[64];

    if (_fd < 0)
        error (this, "unable to open gpio");
    write (_fd, buf, snprintf (buf, 64, "%d", _pin));
    close (_fd);

    /* set it to the desired direction */
    const char* direction = _dir == IN ? "in" : "out";
    const int   dirlen    = _dir == IN ? 2 : 3;
    snprintf (buf, 64, "/sys/class/gpio/gpio%d/direction", pin);
    _fd = open (buf, O_WRONLY);
    if (_fd < 0) {
        error (this, "cannot set direction of GPIO " + __to_string (pin));
    }
    write (_fd, direction, dirlen);
    close (_fd);

    // edge
    snprintf (buf, 64, "/sys/class/gpio/gpio%d/edge", pin);
    _fd = open (buf, O_WRONLY);
    if (_fd < 0) {
        error (this, "cannot set edge of GPIO " + __to_string (pin));
    }
    write (_fd, "both", 4);
    close (_fd);


    /* open the value file  */
    snprintf (buf, 64, "/sys/class/gpio/gpio%d/value", pin);
    _fd = open (buf, _dir == IN ? O_RDONLY : O_WRONLY);
    if (_fd < 0) {
        error (this, "cannot open GPIO " + __to_string (pin));
    }
    if (dir == IN) {
        _iofd = new IOFD (nullptr, "gpiofd", _fd);
        _iofd->activate ();
        _action   = new GPIOLineReadAction (this, "read");
        //_c_action = new Coupling (_iofd->find_child_impl ("readable"), ACTIVATION, _action, ACTIVATION);
        _c_action = new Coupling (_iofd->find_child_impl ("except"), ACTIVATION, _action, ACTIVATION);
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
    } else {
        delete _c_action;
        delete _action;
    }
    close (_fd);
    delete _value;
}

void
GPIOLine::read_value ()
{
    char buf[10];
    lseek (_fd, 0, SEEK_SET);
    //printf("*** read_value %d\n", _fd);
    if (int n = read (_fd, buf, 10) > 0) {
        auto v = buf[0] - '0';
        //printf("*** %d %d\n", n, v);
        _value->set_value (v, true);
    }
}

void
GPIOLine::write_value ()
{
    char buf[4];
    int  value = _value->get_value ();
    snprintf (buf, 4, "%d", value);
    write (_fd, buf, strlen (buf) + 1);
}
} // namespace djnn
