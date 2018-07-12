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
#include "../../core/tree/text_property.h"
#include "../../core/tree/spike.h"

#include "linux_input.h"
#include <linux/input.h>
#include <libevdev/libevdev.h>

namespace djnn {
  LinuxMouse::LinuxMouse (Process *p, const string &n, const struct libevdev *dev) : LinuxDevice (p, n, MOUSE)
  {
    _move = make_unique<Spike> (this, "move");
    _move_dx = make_unique<IntProperty> (nullptr, "dx", 0);
    _move_dy = make_unique<IntProperty> (nullptr, "dy", 0);
    _move->add_symbol ("dx", _move_dx.get ());
    _move->add_symbol ("dy", _move_dy.get ());
    _btn = make_unique<Spike> (this, "button");
    _press = make_unique<Spike> (_btn.get (), "press");
    _release = make_unique<Spike> (_btn.get (), "release");
    _btn_name = make_unique<TextProperty> (nullptr, "name", "");
    _btn->add_symbol ("name", _btn_name.get ());
    int w, hw;
    w = libevdev_has_event_code (dev, EV_REL, REL_WHEEL);
    hw = libevdev_has_event_code (dev, EV_REL, REL_HWHEEL);
    if (w || hw) {
      _wheel = make_unique<Spike> (this, "wheel");
      if (w) {
        _wheel_dy = make_unique<IntProperty> (nullptr, "dy", 0);
        _wheel->add_symbol ("dy", _wheel_dy.get ());
      }
      if (hw) {
        _wheel_dx = make_unique<IntProperty> (nullptr, "dx", 0);
        _wheel->add_symbol ("dx", _wheel_dx.get ());
      }
    }
    _activation_state = activated;
    Process::finalize ();
  }

  LinuxMouse::~LinuxMouse ()
  {
    _parent->remove_child (this);
  }

  void
  LinuxMouse::mouse_btn_event (const char* name, int val)
  {
    _btn_name->set_value (name, true);
    if (val)
      _press->activation ();
    else
      _release->activation ();
  }

  void
  LinuxMouse::handle_event (struct input_event *ev)
  {
    switch (ev->type)
    {
    case EV_KEY:
      switch (ev->code)
        {
        case BTN_LEFT:
          mouse_btn_event ("left", ev->value);
          break;
        case BTN_MIDDLE:
          mouse_btn_event ("middle", ev->value);
          break;
        case BTN_RIGHT:
          mouse_btn_event ("right", ev->value);
          break;
        case BTN_SIDE:
          mouse_btn_event ("side", ev->value);
          break;
        }
      break;
    case EV_REL:
      switch (ev->code)
        {
        case REL_X:
          _move_dx->set_value (ev->value, true);
          break;
        case REL_Y:
           _move_dy->set_value (ev->value, true);
          break;
        case REL_WHEEL:
           _wheel_dy->set_value (ev->value, true);
          break;
        case REL_HWHEEL:
          _wheel_dx->set_value (ev->value, true);
          break;
        }
      break;
    default:
      return;
    }
  }
}
