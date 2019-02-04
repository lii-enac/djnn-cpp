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

#include "abstract_gobj.h"
#include "backend.h"
#include "abstract_backend.h"
#include "../core/tree/spike.h"
#include "../core/tree/component.h"
#include "../core/tree/set.h"
#include "window.h"

namespace djnn
{
  void
  Window::init_ui (const std::string &title, double x, double y, double w, double h)
  {
    _pos_x = new DoubleProperty (this, "x", x);
    _pos_y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    _title = new TextProperty (this, "title", title);
    _key_pressed = new IntProperty (this, "key-pressed", 0);
    _key_released = new IntProperty (this, "key-released", 0);
    _key_pressed_text = new TextProperty (this, "key-pressed_text", "");
    _key_released_text = new TextProperty (this, "key-released_text", "");
    _close = new Spike;
    add_symbol ("close", _close);
    _press = new Spike;
    _release = new Spike;
    _move = new Spike;
    _wheel = new Spike;
    _touches = new Set (this, "touches");
    _touches->set_activated ();
    _press_x = new DoubleProperty (0);
    _press_y = new DoubleProperty (0);
    _move_x = new DoubleProperty (0);
    _move_y = new DoubleProperty (0);
    _press->add_symbol ("x", _press_x);
    _press->add_symbol ("y", _press_y);
    _move->add_symbol ("x", _move_x);
    _move->add_symbol ("y", _move_y);
    _w_dx = new DoubleProperty (0);
    _w_dy = new DoubleProperty (0);
    _wheel->add_symbol ("dx", _w_dx);
    _wheel->add_symbol ("dy", _w_dy);
    add_symbol ("press", _press);
    add_symbol ("move", _move);
    add_symbol ("release", _release);
    add_symbol ("wheel", _wheel);

    _win_impl = Backend::instance ()->create_window (this, title, x, y, w, h);
  }

  Window::Window (const std::string &title, double x, double y, double w, double h) :
    Process(), _refresh (false), _holder (nullptr)
  {
    init_ui (title, x, y, w, h);
  }

  Window::Window (Process *p, const std::string &n, const std::string &title, double x, double y, double w,
		  double h) :
      Process (p, n), _refresh (false), _holder (nullptr)
  {
    init_ui (title, x, y, w, h);
    Process::finalize ();
  }

  Window::~Window ()
  {
    delete _pos_x;
    delete _pos_y;
    delete _w_dx;
    delete _w_dy;
    delete _width;
    delete _height;
    delete _title;
    delete _close;
    delete _press;
    delete _release;
    delete _move;
    delete _wheel;
    delete _win_impl;
  }


} /* namespace djnn */
