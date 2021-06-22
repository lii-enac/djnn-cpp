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
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/set.h"
#include "core/ontology/coupling.h"
#include "core/utils/error.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/to_string.h"

#include <string.h>
#include <iostream>

#include <fcntl.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#ifndef DEBUG
#define DEBUG 1
#endif



namespace djnn {

  DRMConnector::DRMConnector (ParentProcess* parent, const string& name, int id, bool connected) : FatProcess (name),
    _display (nullptr), _id (id)
  {
    new BoolProperty (this, "connected", connected);
    finalize_construction (parent, name);
  }

  DRMConnector::~DRMConnector ()
  {
    delete find_child ("connected");
  }

  DRMDevice::DRMDevice (ParentProcess* parent, const string& name, int fd, int min_width, int max_width, int min_height, int max_height) : FatProcess (name), _fd (fd)
  {
    new IntProperty (this, "min_width", min_width);
    new IntProperty (this, "max_width", max_width);
    new IntProperty (this, "min_height", min_height);
    new IntProperty (this, "max_height", max_height);
    new Set (this, "connectors");
    finalize_construction (parent, name);
  }

  DRMDevice::~DRMDevice ()
  {
    delete find_child ("min_width");
    delete find_child ("max_width");
    delete find_child ("min_height");
    delete find_child ("max_height");
    delete find_child ("connectors");
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
    _readable_cpl = new Coupling (_udev_iofd->find_child_impl ("readable"), ACTIVATION, _action, ACTIVATION);
  }

  DRMUdev::~DRMUdev ()
  {
    _udev_iofd->deactivate ();
    udev_monitor_unref (_udev_mon);
    udev_unref (_udev_connection);


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

  static vector<string> _type_names = {
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
      n = string (_type_names[con->connector_type] + "-" + djnn::to_string (con->connector_type_id));
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
      std::cout << "New device found: " << name << " " << res->min_width << " " << res->max_width << " "  << res->min_height << " " << res->max_height << endl;
#endif
      drmModeConnector *con;
      for (int i = 0; i < res->count_connectors; ++i)
      {
        uint32_t id = res->connectors[i];
        con = drmModeGetConnector (fd, id);
        string con_name = build_name (con);
        new DRMConnector (drm_dev->find_child_impl ("connectors"), con_name, id, con->connection == DRM_MODE_CONNECTED);
#if DEBUG
        std::cout << "New connector found " << con_name;
        if (con->connection == DRM_MODE_CONNECTED)
          std::cout << " STATUS: connected\n";
        else
          std::cout << " STATUS: not connected\n";
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

    DRMDevice* drm_dev = (DRMDevice*) GPUs->find_child_impl (name);
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
      FatProcess *drm_con = drm_dev->find_child_impl ("connectors/" + con_name);
      if (drm_con == nullptr) {
        std::cout << "drm_con not found\n";
        drmModeFreeConnector (con);
        continue;
      }
      if (((BoolProperty*) drm_con->find_child_impl ("connected"))->get_value () != is_connected) {
        ((BoolProperty*) drm_con->find_child_impl ("connected"))->set_value (is_connected, 1);
#if DEBUG
        std::cout << "Connector " << con_name << " is now";
        if (con->connection == DRM_MODE_CONNECTED)
          std::cout << " connected\n";
        else
          std::cout << " disconnected\n";
#endif
      }
      drmModeFreeConnector (con);
    }
    drmModeFreeResources (res);
  }
}
