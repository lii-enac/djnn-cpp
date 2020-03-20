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

#include <linux/input.h>
#include <libevdev/libevdev.h>


namespace djnn {
  LinuxDevice*
  map_device (const struct libevdev *dev, const std::string &n)
  {
    //int keyFound = 0;

    /* mouse if we have at least relative x and y and button left */
    if (libevdev_has_event_code (dev, EV_REL, REL_X)
      && libevdev_has_event_code (dev, EV_REL, REL_Y)
      && libevdev_has_event_code (dev, EV_KEY, BTN_LEFT)) {
        return new LinuxMouse (Mice, n, dev);
    } else if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_POSITION_X)
      && libevdev_has_event_code (dev, EV_ABS, ABS_MT_POSITION_Y)) {
        return new LinuxTouchPanel (TouchPanels, n, dev);
    }
    warning (nullptr, "unknown device type");
    return nullptr;
  }
}
