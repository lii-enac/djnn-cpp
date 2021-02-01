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
#include "core/utils/error.h"
#include "core/ontology/coupling.h"
#include "core/core-dev.h" // graph add/remove edge

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <libevdev/libevdev.h>

using namespace std;

namespace djnn {
	Evdev::Evdev (const char* node) : _action (nullptr), _aborted (false)
  {
    int err;
    _fd = open (node, O_RDONLY|O_NONBLOCK);
    if (_fd < 0) {
      warning (nullptr, "Cannot open " + string (node) + " for reading: " + string (strerror (errno)));
      _aborted = true;
      return;
    } else {
      /* test if this device is grabbed by another app */
      err = ioctl (_fd, EVIOCGRAB, 1); /* attempt grabbing */
      if (err == 0) /* if success, release the grab */
      err = ioctl (_fd, EVIOCGRAB, 0);
      if (err) {
        warning (nullptr, " " + string (node) + " is grabbed by another process");
        close (_fd);
        _aborted = true;
        return;
      }
    }
    _dev = libevdev_new ();
    err = libevdev_set_fd (_dev, _fd);

    if (err < 0) {
      warning (nullptr, "failed to init evdev device " + string (node));
      _aborted = true;
      close (_fd);
      return;
    }

    _name = string (libevdev_get_name (_dev));
    _djn_dev = map_device (_dev, _name);

    if (_djn_dev == nullptr) {
      _aborted = true;
      libevdev_free (_dev);
      close (_fd);
      return;
    }
    // FIXME: this should be done lazily
    _iofd = new IOFD (nullptr, "evdevfd", _fd);
    _iofd->activate ();
    _action = new EvdevAction (this);
    _readable_cpl = new Coupling (_iofd->find_child_impl ("readable"), ACTIVATION, _action, ACTIVATION);
  }

  Evdev::~Evdev ()
  {
    if (_aborted)
      return;
    _iofd->deactivate ();

    delete _readable_cpl;
    delete _action;
    delete _iofd;
    
    libevdev_free (_dev);
    close (_fd);
  }

  void
  Evdev::handle_evdev_msg ()
  {
    int err;
    struct input_event ev;

    do {
      err = libevdev_next_event (_dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
      if (err == LIBEVDEV_READ_STATUS_SYNC) {
        warning (nullptr, "input events may have been lost for device " + _name);
      } else if (err == LIBEVDEV_READ_STATUS_SUCCESS) {
        _djn_dev->handle_event (&ev);
      }
    } while (err == LIBEVDEV_READ_STATUS_SUCCESS);
    if (err != -EAGAIN && err != -EINTR) {
      return;
    }
  }
}
