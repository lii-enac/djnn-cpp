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

#include "../../core/tree/int_property.h"
#include "../../core/tree/set.h"
#include "../../core/utils/error.h"
#include "../../core/utils/utils-dev.h"
#include "linux_input.h"

#include <linux/input.h>
#include <libevdev/libevdev.h>

namespace djnn {

  LinuxTouch::LinuxTouch (unsigned int fieldmap) : Process ()
  {
    set_activation_state (ACTIVATED);
    _used = UNUSED;
    if (fieldmap & MT_X)
      _x = new IntProperty (this, "x", 0);
    if (fieldmap & MT_Y)
      _y = new IntProperty (this, "y", 0);
    if (fieldmap & MT_W)
      _width = new IntProperty (this, "width", 0);
    if (fieldmap & MT_H)
      _height = new IntProperty (this, "height", 0);
    if (fieldmap & MT_CX)
      _cx = new IntProperty (this, "cx", 0);
    if (fieldmap & MT_CY)
      _cy = new IntProperty (this, "cy", 0);
    if (fieldmap & MT_PRESSURE)
      _pressure = new IntProperty (this, "pressure", 0);
  }

  LinuxTouch::~LinuxTouch ()
  {
    delete _pressure;
    delete _cy;
    delete _cx;
    delete _height;
    delete _width;
    delete _y;
    delete _x;
  }

  LinuxTouchPanel::LinuxTouchPanel (Process *p, const string &n, const struct libevdev *dev) : LinuxDevice (p, n, TOUCH_PANEL)
  {
    _touches = new Set (this, "touches");
    set_activation_state (ACTIVATED);
    _touches->set_activation_state (ACTIVATED);
    _fieldmap = 0;
    _cur_touch = nullptr;
    _nb_slots = libevdev_get_abs_maximum (dev, ABS_MT_SLOT) + 1;
    _max_x = new IntProperty (this, "maxX", libevdev_get_abs_maximum (dev, ABS_MT_POSITION_X));
    _max_y = new IntProperty (this, "maxY", libevdev_get_abs_maximum (dev, ABS_MT_POSITION_Y));
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_POSITION_X))
      _fieldmap |= MT_X;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_POSITION_Y))
      _fieldmap |= MT_Y;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_TOUCH_MAJOR))
      _fieldmap |= MT_W;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_TOUCH_MINOR))
      _fieldmap |= MT_H;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_TOOL_X))
      _fieldmap |= MT_CX;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_TOOL_Y))
      _fieldmap |= MT_CY;
    if (libevdev_has_event_code (dev, EV_ABS, ABS_MT_PRESSURE))
      _fieldmap |= MT_PRESSURE;
    for (int i = 0; i < _nb_slots; i++) {
      LinuxTouch *lt = new LinuxTouch (_fieldmap);
      _v_touches.push_back (lt);
    }
    Process::finalize_construction (p);
  }

  LinuxTouchPanel::~LinuxTouchPanel ()
  {
    _parent->remove_child (this);

    // destroy all touches and clear
    for (std::vector<LinuxTouch*>::iterator it = _v_touches.begin () ; it != _v_touches.end(); ++it)
      delete *it;
    _v_touches.clear ();

    delete _max_y;
    delete _max_x;
    delete _touches;
  }

  void
  LinuxTouchPanel::handle_event (struct input_event *ev)
  {
    switch (ev->type)
    {
    case EV_ABS:
      switch (ev->code)
        {
        case ABS_MT_SLOT:
          _cur_touch = _v_touches[ev->value];
          if (_cur_touch->used () == UNUSED)
            _cur_touch->set_used (NEW);
          break;
        case ABS_MT_TRACKING_ID:
          if (ev->value > -1 && _cur_touch == nullptr) {
            _cur_touch = _v_touches[0];
            if (_cur_touch->used () == UNUSED)
              _cur_touch->set_used (NEW);
            else
              warning (this, "warning, touch already used");
          } else {
            _touches->remove_child (_cur_touch);
            _cur_touch->set_used (UNUSED);
            _cur_touch = nullptr;
          }
          break;
        case ABS_MT_TOOL_X:
          _cur_touch->set_width (ev->value);
          break;
        case ABS_MT_TOOL_Y:
          _cur_touch->set_height (ev->value);
          break;
        case ABS_MT_TOUCH_MAJOR:
          _cur_touch->set_cx (ev->value);
          break;
        case ABS_MT_TOUCH_MINOR:
          _cur_touch->set_cy (ev->value);
          break;
        case ABS_MT_PRESSURE:
          _cur_touch->set_pressure (ev->value);
          break;
        case ABS_MT_POSITION_X:
          _cur_touch->set_x (ev->value);
          break;
        case ABS_MT_POSITION_Y:
          _cur_touch->set_y (ev->value);
          break;
        }
      break;
    case EV_SYN:
      if (ev->code == SYN_REPORT) {
        for (int i = 0; i < _nb_slots; ++i) {
          if (_v_touches[i]->used () == NEW) {
            _touches->add_child (_v_touches[i], "");
            _v_touches[i]->set_used (USED);
          }
        }
      }
      break;
    default:
      return;
    }
  }
}
