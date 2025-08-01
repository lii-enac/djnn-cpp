/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "core/core-dev.h" // graph add/remove edge
#include "core/ontology/coupling.h"
#include "core/property/bool_property.h"
#include "core/property/int_property.h"
#include "core/tree/set.h"
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "core/utils/to_string.h"
#include "display/drm/drm_display.h"

// dbg
// #include "core/utils/iostream.h"
// #include "utils/debug.h"
// #include <thread> // this_thread

#ifndef DEBUG
// #define DEBUG 1
#endif

namespace djnn {

extern CoreProcess* GPUs; // display.h
using djnnstl::string;

DRMDevice::DRMDevice (CoreProcess* parent, const string& name, int fd, int min_width, int max_width,
                      int min_height, int max_height)
    : FatProcess (name), _fd (fd),
      _iofd (this, "vblank_fd", fd), _read_vblank_action (this, "read_vblank_action"),
      _c_read_vblank (_iofd.find_child ("readable"), ACTIVATION, &_read_vblank_action, ACTIVATION, true)
{
    new IntProperty (this, "min_width", min_width);
    new IntProperty (this, "max_width", max_width);
    new IntProperty (this, "min_height", min_height);
    new IntProperty (this, "max_height", max_height);
    new Set (this, "connectors");
    set_activation_state (ACTIVATED);
    _iofd.activate ();
    //_read_vblank_action.set_activation_state (ACTIVATED);
    //_c_read_vblank.enable ();

    graph_add_edge (_iofd.find_child ("readable"), &_read_vblank_action);
    finalize_construction (parent, name);
    // activate ();
}

DRMDevice::~DRMDevice ()
{
    delete find_child ("min_width");
    delete find_child ("max_width");
    delete find_child ("min_height");
    delete find_child ("max_height");
    delete find_child ("connectors");
    graph_remove_edge (_iofd.find_child ("readable"), &_read_vblank_action);
    close (_fd);
}

static void
page_flip_event (int fd, unsigned int frame, unsigned int sec, unsigned int usec, void* data)
{
    DRMConnector* conn = (DRMConnector*)data;
    conn->handle_vblank ();
}

void
DRMDevice::read_vblank ()
{
    drmEventContext ev;
    memset (&ev, 0, sizeof (ev));
    ev.version           = DRM_EVENT_CONTEXT_VERSION;
    ev.page_flip_handler = page_flip_event;
    drmHandleEvent (_fd, &ev);
}

DRMUdev::DRMUdev ()
{
    struct udev_enumerate*  dev_filter;
    struct udev_list_entry *dev_list, *dev_list_entry;
    struct udev_device*     dev;
    const char *            syspath, *filename;
    /* connect to udev */
    _udev_connection = udev_new ();
    if (!_udev_connection) {
        warning (nullptr, "Cannot connect to udev");
        return;
    }

    /* list all present DRM devices */
    dev_filter = udev_enumerate_new (_udev_connection);
    udev_enumerate_add_match_subsystem (dev_filter, "drm");
    udev_enumerate_add_match_sysname (dev_filter, "card[0-9]*");
    udev_enumerate_scan_devices (dev_filter);
    dev_list = udev_enumerate_get_list_entry (dev_filter);

    udev_list_entry_foreach (dev_list_entry, dev_list)
    {
        syspath  = udev_list_entry_get_name (dev_list_entry);
        dev      = udev_device_new_from_syspath (_udev_connection, syspath);
        filename = udev_device_get_devnode (dev);
        if (dev) {
            handle_drm_device (filename, dev);
        }

        udev_device_unref (dev);
    }
    udev_enumerate_unref (dev_filter);

    /* monitor device additions and removals */
    _udev_mon = udev_monitor_new_from_netlink (_udev_connection, "udev");
    udev_monitor_filter_add_match_subsystem_devtype (_udev_mon, "drm", 0);
    udev_monitor_enable_receiving (_udev_mon);
    _udev_iofd = new IOFD (nullptr, "udev_iofd", udev_monitor_get_fd (_udev_mon));
    _udev_iofd->activate ();
    _action       = new DRMUdevAction (this);
    _readable_cpl = new Coupling (_udev_iofd->find_child_impl ("readable"), ACTIVATION, _action, ACTIVATION);
    graph_add_edge (_udev_iofd->find_child_impl ("readable"), _action);
}

DRMUdev::~DRMUdev ()
{
    _udev_iofd->deactivate ();
    udev_monitor_unref (_udev_mon);
    udev_unref (_udev_connection);

    graph_remove_edge (_udev_iofd->find_child_impl ("readable"), _action);

    delete _readable_cpl;
    delete _action;
    delete _udev_iofd;
}

void
DRMUdev::handle_udev_msg ()
{
    struct udev_device* dev = udev_monitor_receive_device (_udev_mon);
    if (!dev)
        return;
    const char* filename = udev_device_get_devnode (dev);
    update_drm_device (filename, dev);
    udev_device_unref (dev);
}

static djnnstl::vector<string> _type_names = {
    "Unknown", "VGA", "DVI-I", "DVI-D", "DVI-A",
    "Composite", "SVIDEO", "LVDS", "Component",
    "DIN", "DP", "HDMI-A", "HDMI-B", "TV",
    "eDP", "Virtual", "DSI", "DPI"};

static string
build_name (drmModeConnector* drm_conn)
{
    string n;
    if (drm_conn->connector_type < _type_names.size ())
        n = string (_type_names[drm_conn->connector_type] + "-" + djnnstl::to_string (drm_conn->connector_type_id));
    else
        n = "UNNAMED";
    return n;
}

void
DRMUdev::handle_drm_device (const char* filename, struct udev_device* dev)
{
    errno            = 0;
    const char* name = udev_device_get_property_value (dev, "DEVNAME");
    if (!name) {
        perror ("");
        if (filename)
            warning (nullptr, string ("DRM: could not udev_device_get_property_value for ") + filename);
        else
            warning (nullptr, "DRM: could not udev_device_get_property_value");
        return;
    }
    if (filename) {
        int fd = open (filename, O_RDWR);
        if (fd == -1) {
            perror ("");
            warning (nullptr, string ("DRM: could not open ") + filename);
            return;
        }
        drmModeRes* res = drmModeGetResources (fd);
        if (!res) {
            perror ("");
            warning (nullptr, "DRM: could not get resources");
            return;
        }
        string      str (name);
        std::size_t found    = str.find_last_of ("/");
        string      dev_name = str.substr (found + 1);
        DRMDevice*  drm_dev  = new DRMDevice (GPUs, dev_name, fd, res->min_width, res->max_width, res->min_height, res->max_height);
#if DEBUG
        djnnstl::cout << "New DRM device found: " << dev_name << " " << res->min_width << " " << res->max_width << " " << res->min_height << " " << res->max_height << djnnstl::endl;
#endif
        drmModeConnector* drm_conn;
        for (int i = 0; i < res->count_connectors; ++i) {
            uint32_t id      = res->connectors[i];
            drm_conn         = drmModeGetConnector (fd, id);
            string conn_name = build_name (drm_conn);
            new DRMConnector (drm_dev->find_child_impl ("connectors"), conn_name, fd, res, drm_conn);
            drmModeFreeConnector (drm_conn);
        }
        drmModeFreeResources (res);
    }
}

void
DRMUdev::update_drm_device (const char* filename, struct udev_device* dev)
{
    const char* name = udev_device_get_property_value (dev, "DEVNAME");
    if (!name)
        return;
    string      str (name);
    std::size_t found    = str.find_last_of ("/");
    string      dev_name = str.substr (found + 1);
    DRMDevice*  drm_dev  = (DRMDevice*)GPUs->find_child_impl (dev_name);
    if (!drm_dev) {
        perror ("");
        warning (nullptr, "DRM: no DRMDevice");
        return;
    }

    int         fd  = drm_dev->get_fd ();
    drmModeRes* res = drmModeGetResources (fd);
    if (!res) {
        perror ("");
        warning (nullptr, "DRM: could not drmModeGetResources");
        return;
    }
    drmModeConnector* drm_conn;
    for (int i = 0; i < res->count_connectors; ++i) {
        uint32_t id = res->connectors[i];
        drm_conn    = drmModeGetConnector (fd, id);
        if (!drm_conn)
            continue;
        string        conn_name    = build_name (drm_conn);
        bool          is_connected = drm_conn->connection == DRM_MODE_CONNECTED;
        DRMConnector* conn         = (DRMConnector*)drm_dev->find_child_impl ("connectors/" + conn_name);
        if (conn == nullptr) {
            perror ("");
            warning (nullptr, "DRM: connector not found");
            drmModeFreeConnector (drm_conn);
            continue;
        }
        if (((BoolProperty*)conn->find_child_impl ("connected"))->get_value () != is_connected) {
            if (is_connected)
                conn->init_connection (res, drm_conn);
            else
                conn->clean ();
            ((BoolProperty*)conn->find_child_impl ("connected"))->set_value (is_connected, 1);
#if DEBUG
            std::cout << "Connector " << conn_name << " is now";
            if (is_connected)
                std::cout << " connected\n";
            else
                std::cout << " disconnected\n";
#endif
        }
        drmModeFreeConnector (drm_conn);
    }
    drmModeFreeResources (res);
}
} // namespace djnn
