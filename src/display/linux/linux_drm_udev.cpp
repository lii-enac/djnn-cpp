/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "linux_display.h"
#include "../../core/tree/bool_property.h"
#include "../../core/tree/int_property.h"
#include "../../core/tree/set.h"
#include "../../core/ontology/coupling.h"
#include "../../core/utils/error.h"
#include "../../core/execution/graph.h"
#include <string.h>
#include <iostream>


#include <fcntl.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#ifndef DEBUG
#define DEBUG 1
#endif

using namespace std;

namespace djnn {

  DRMConnector::DRMConnector (Process *p, const string& name, int id, bool connected) : Process (name),
    _display (nullptr), _id (id)
  {
    new BoolProperty (this, "connected", connected);
    Process::finalize_construction (parent, name);
  }

  DRMConnector::~DRMConnector ()
  {
    delete find_component ("connected");
  }

  DRMDevice::DRMDevice (Process *p, const string& name, int fd, int min_width, int max_width, int min_height, int max_height) : Process (name), _fd (fd)
  {
    new IntProperty (this, "min_width", min_width);
    new IntProperty (this, "max_width", max_width);
    new IntProperty (this, "min_height", min_height);
    new IntProperty (this, "max_height", max_height);
    new Set (this, "connectors");
    Process::finalize_construction (parent, name);
  }

  DRMDevice::~DRMDevice ()
  {
    delete find_component ("min_width");
    delete find_component ("max_width");
    delete find_component ("min_height");
    delete find_component ("max_height");
    delete find_component ("connectors");
  }

  DRMUdev::DRMUdev ()
  {
    struct udev_enumerate *dev_filter;
    struct udev_list_entry *dev_list, *dev_list_entry;
    struct udev_device *dev;
    const char *syspath, *filename;
  /* connect to udev */
    _udev_connection = udev_new ();
    if (!_udev_connection) {
      warning  (nullptr, "Cannot connect to udev");
      return;
    }

  /* list all present DRM devices */
    dev_filter = udev_enumerate_new (_udev_connection);
    udev_enumerate_add_match_subsystem (dev_filter, "drm");
    udev_enumerate_add_match_sysname (dev_filter, "card[0-9]*");
    udev_enumerate_scan_devices (dev_filter);
    dev_list = udev_enumerate_get_list_entry (dev_filter);

    udev_list_entry_foreach (dev_list_entry, dev_list) {
      syspath = udev_list_entry_get_name (dev_list_entry);
      dev = udev_device_new_from_syspath (_udev_connection,
       syspath);
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
    _udev_iofd = new IOFD (udev_monitor_get_fd (_udev_mon));
    _udev_iofd->activate ();
    _action = new DRMUdevAction (this);
    _readable_cpl = new Coupling (_udev_iofd->find_component ("readable"), ACTIVATION, _action, ACTIVATION);
    Graph::instance().add_edge (_udev_iofd->find_component ("readable"), _action);
  }

  DRMUdev::~DRMUdev ()
  {
    _udev_iofd->deactivate ();
    udev_monitor_unref (_udev_mon);
    udev_unref (_udev_connection);

    Graph::instance().remove_edge (_udev_iofd->find_component ("readable"), _action);

    delete _readable_cpl;
    delete _action;
    delete _udev_iofd;

  }

  void
  DRMUdev::handle_udev_msg ()
  {
    struct udev_device *dev = udev_monitor_receive_device (_udev_mon);
    if (!dev)
      return;
    const char* filename = udev_device_get_devnode (dev);
    update_drm_device (filename, dev);
    udev_device_unref (dev);
  }

  static std::vector<string> _type_names = {
    "Unknown", "VGA","DVI-I", "DVI-D", "DVI-A",
    "Composite", "SVIDEO", "LVDS", "Component",
    "DIN", "DP", "HDMI-A", "HDMI-B", "TV",
    "eDP", "Virtual", "DSI", "DPI"
  };

  static string
  build_name (drmModeConnector *con)
  {
    string n;
    if (con->connector_type < _type_names.size ())
      n = string (_type_names[con->connector_type] + "-" + std::to_string (con->connector_type_id));
    else
      n = "UNNAMED";
    return n;
  }

  void
  DRMUdev::handle_drm_device (const char* filename, struct udev_device *dev)
  {
    const char *name = udev_device_get_property_value (dev,  "DEVNAME");
    if (!name)
      return;
    if (filename) {
      int fd = open (filename, O_RDWR);
      drmModeRes *res = drmModeGetResources (fd);

      if (!res)
        return;
      DRMDevice* drm_dev = new DRMDevice (GPUs, name, fd, res->min_width, res->max_width, res->min_height, res->max_height);
#if DEBUG
      cout << "New device found: " << name << " " << res->min_width << " " << res->max_width << " "  << res->min_height << " " << res->max_height << endl;
#endif
      drmModeConnector *con;
      for (int i = 0; i < res->count_connectors; ++i)
      {
        uint32_t id = res->connectors[i];
        con = drmModeGetConnector (fd, id);
        string con_name = build_name (con);
        new DRMConnector (drm_dev->find_component ("connectors"), con_name, id, con->connection == DRM_MODE_CONNECTED);
#if DEBUG
        cout << "New connector found " << con_name;
        if (con->connection == DRM_MODE_CONNECTED)
          cout << " STATUS: connected\n";
        else
          cout << " STATUS: not connected\n";
#endif
        drmModeFreeConnector (con);
      }
      drmModeFreeResources (res);
    }
  }

  void
  DRMUdev::update_drm_device (const char* filename, struct udev_device *dev)
  {
    const char *name = udev_device_get_property_value (dev,  "DEVNAME");
    if (!name)
      return;

    DRMDevice* drm_dev = (DRMDevice*) GPUs->find_component (name);
    if (!dev)
      return;

    int fd = drm_dev->fd ();
    drmModeRes *res = drmModeGetResources (fd);
    if (!res)
      return;
    drmModeConnector *con;
    for (int i = 0; i < res->count_connectors; ++i)
    {
      uint32_t id = res->connectors[i];
      con = drmModeGetConnector (fd, id);
      if (!con)
        continue;
      string con_name = build_name (con);
      bool is_connected = con->connection == DRM_MODE_CONNECTED;
      Process *drm_con = drm_dev->find_component ("connectors/" + con_name);
      if (drm_con == nullptr) {
        cout << "drm_con not found\n";
        drmModeFreeConnector (con);
        continue;
      }
      if (((BoolProperty*) drm_con->find_component ("connected"))->get_value () != is_connected) {
        ((BoolProperty*) drm_con->find_component ("connected"))->set_value (is_connected, 1);
#if DEBUG
        cout << "Connector " << con_name << " is now";
        if (con->connection == DRM_MODE_CONNECTED)
          cout << " connected\n";
        else
          cout << " disconnected\n";
#endif
      }
      drmModeFreeConnector (con);
    }
    drmModeFreeResources (res);
  }
}
