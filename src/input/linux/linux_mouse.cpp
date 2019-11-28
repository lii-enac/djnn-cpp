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

#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "core/tree/spike.h"

#include "linux_input.h"
#include <linux/input.h>
#include <libevdev/libevdev.h>

namespace djnn {
  LinuxMouse::LinuxMouse (Process *parent, const string &name, const struct libevdev *dev) : LinuxDevice (parent, name, MOUSE)
  {
    _move = new Spike (this, "move");
    _move_dx = new IntProperty (nullptr, "dx", 0);
    _move_dy = new IntProperty (nullptr, "dy", 0);
    _move->add_symbol ("dx", _move_dx);
    _move->add_symbol ("dy", _move_dy);
    _btn = new Spike (this, "button");
    _press = new Spike (_btn, "press");
    _release = new Spike (_btn, "release");
    _btn_name = new TextProperty (nullptr, "name", "");
    _btn->add_symbol ("name", _btn_name);
    int w, hw;
    w = libevdev_has_event_code (dev, EV_REL, REL_WHEEL);
    hw = libevdev_has_event_code (dev, EV_REL, REL_HWHEEL);
    if (w || hw) {
      _wheel = new Spike (this, "wheel");
      if (w) {
        _wheel_dy = new IntProperty (nullptr, "dy", 0);
        _wheel->add_symbol ("dy", _wheel_dy);
      }
      if (hw) {
        _wheel_dx = new IntProperty (nullptr, "dx", 0);
        _wheel->add_symbol ("dx", _wheel_dx);
      }
    }
    set_activation_state (ACTIVATED);
    Process::finalize_construction (parent, name);
  }

  LinuxMouse::~LinuxMouse ()
  {
    get_parent ()->remove_child (this);

    delete _wheel_dx;
    delete _wheel_dy;
    delete _wheel;
    delete _btn_name;
    delete _release;
    delete _press;
    delete _btn;
    delete _move_dy;
    delete _move_dx;
    delete _move;
  }

  void
  LinuxMouse::mouse_btn_event (const char* name, int val)
  {
    _btn_name->set_value (name, true);
    if (val)
      _press->activate ();
    else
      _release->activate ();
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
