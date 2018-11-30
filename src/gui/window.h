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

#pragma once

#include "../core/tree/double_property.h"
#include "../core/tree/text_property.h"
#include "../core/tree/process.h"

#include <iostream>

namespace djnn
{

  class WinImpl {
  public:
    WinImpl () {}
    virtual ~WinImpl () {}
    virtual void activate () = 0;
    virtual void deactivate () = 0;
    virtual void update () = 0;
  };

  class Window : public Process
  {
  public:
    Window (const std::string& title, double x, double y, double w, double h);
    Window (Process *p, const std::string& n, const std::string& title, double x, double y, double w, double h);
    virtual ~Window ();

    DoubleProperty* pos_x () { return _pos_x; }
    DoubleProperty* pos_y () { return _pos_y; }
    DoubleProperty* wheel_dx () { return _w_dx; }
    DoubleProperty* wheel_dy () { return _w_dy; }
    DoubleProperty* width () { return _width; }
    DoubleProperty* height () { return _height; }
    TextProperty* title () { return _title; }
    IntProperty* key_pressed () { return _key_pressed; }
    IntProperty* key_released () { return _key_released; }
    TextProperty* key_pressed_text () { return _key_pressed_text; }
    TextProperty* key_released_text () { return _key_released_text; }
    Process* close () { return _close; }
    WinImpl* win_impl () { return _win_impl; }
    void set_refresh (bool r) { _refresh = r; }
    bool refresh () { return _refresh; }
    void update () { _win_impl->update (); };
    void activate () override { _win_impl->activate (); }
    void deactivate () override { _win_impl->deactivate (); }
    Process* press () { return _press; }
    Process* move () { return _move; }
    Process* release () { return _release; }
    Process* wheel () { return _wheel; }
    Process* touches () { return _touches; }
    DoubleProperty* press_x () { return _press_x; }
    DoubleProperty* press_y () { return _press_y; }
    DoubleProperty* move_x () { return _move_x; }
    DoubleProperty* move_y () { return _move_y; }
    void set_frame ();
    Process* holder () { return _holder; }
    void set_holder (Process *p) { _holder = p; }

  private:
    void init_ui (const std::string &title, double x, double y, double w, double h);
    void set_frame_to_component (Process* c);
    DoubleProperty* _pos_x;
    DoubleProperty* _pos_y;
    DoubleProperty* _w_dx, *_w_dy;
    DoubleProperty* _width;
    DoubleProperty* _height;
    TextProperty* _title;
    Process *_close;
    Process* _press;
    Process* _move;
    Process* _release;
    Process* _wheel;
    Process* _touches;
    DoubleProperty *_press_x;
    DoubleProperty *_press_y;
    DoubleProperty *_move_x;
    DoubleProperty *_move_y;
    TextProperty *_key_pressed_text;
    IntProperty *_key_pressed;
    TextProperty *_key_released_text;
    IntProperty *_key_released;
    WinImpl *_win_impl;
    bool _refresh;
    Process* _holder;
  };

} /* namespace djnn */
