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

#include "linux_input.h"
#include "../../core/ontology/coupling.h"
#include "../../core/utils/error.h"
#include <string.h>

namespace djnn {

  Udev::Udev ()
  {
    struct udev_enumerate *dev_filter;
    struct udev_list_entry *dev_list, *dev_list_entry;

  /* connect to udev */
    _udev_connection = udev_new ();
    if (!_udev_connection) {
      warning  (nullptr, "Cannot connect to udev");
      return;
    }

  /* list all present devices managed by the input subsystem */
    dev_filter = udev_enumerate_new (_udev_connection);
    udev_enumerate_add_match_subsystem (dev_filter, "input");
    udev_enumerate_scan_devices (dev_filter);
    dev_list = udev_enumerate_get_list_entry (dev_filter);

    udev_list_entry_foreach (dev_list_entry, dev_list) {
      const char *syspath = udev_list_entry_get_name (dev_list_entry);
      struct udev_device *dev = udev_device_new_from_syspath (_udev_connection,
       syspath);
      if (dev)
        handle_input_device (dev);

      udev_device_unref (dev);
    }
    udev_enumerate_unref (dev_filter);

  /* monitor device additions and removals */
    _udev_mon = udev_monitor_new_from_netlink (_udev_connection, "udev");
    udev_monitor_filter_add_match_subsystem_devtype (_udev_mon, "input", 0);
    udev_monitor_enable_receiving (_udev_mon);
    _udev_iofd = new IOFD (udev_monitor_get_fd (_udev_mon));
    _udev_iofd->activation ();
    _action = new UdevAction (this);
    _readable_cpl = new Coupling (_udev_iofd->find_component ("readable"), ACTIVATION, _action, ACTIVATION);
    Graph::instance().add_edge (_udev_iofd->find_component ("readable"), _action);
  }

  Udev::~Udev ()
  {
    _udev_iofd->deactivation ();
    udev_monitor_unref (_udev_mon);
    udev_unref (_udev_connection);
    
    Graph::instance().remove_edge (_udev_iofd->find_component ("readable"), _action);
    delete _readable_cpl;
    delete _action;
    delete _udev_iofd;

  }

  void
  Udev::handle_udev_msg ()
  {
    const char *action;
    struct udev_device *dev = udev_monitor_receive_device (_udev_mon);

    if (!dev)
      return;

    action = udev_device_get_action (dev);

  /* new device */
    if (strcmp (action, "add") == 0) {
      handle_input_device (dev);

  /* device removed */
    } else if (strcmp (action, "remove") == 0) {
      string dev_sys_name = string (udev_device_get_sysname (dev));
      map<string, Evdev*>::iterator it = _sysname_to_dev.find (dev_sys_name);
      if (it != _sysname_to_dev.end ()) {
        Evdev* djn_dev = it->second;
        _sysname_to_dev.erase (it);
        delete djn_dev;
      }
    }
    udev_device_unref (dev);
  }

  void
  Udev::handle_input_device (struct udev_device *dev)
  {
    const char* node = udev_device_get_devnode (dev);

  /* select evdev nodes */
    if (node && strncmp (node, "/dev/input/event", 16) == 0) {
    string sysname = string (udev_device_get_sysname (dev)); /* = "eventN" */
      Evdev* djn_dev = new Evdev (node);
      _sysname_to_dev[sysname] = djn_dev;
    }
  }
}
