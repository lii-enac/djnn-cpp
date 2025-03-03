/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "display/window.h"

namespace djnn {

extern const djnnstl::vector<djnnstl::string> __ui_interface;

class UI {
  public:
    UI (CoreProcess* parent, FatProcess* f);
    bool is_pickable () { return pickable->get_value (); }
    // void activate (FatProcess* frame) { cpick->enable (frame); }
    void activate (FatProcess* frame) {
        /*
            if the frame was null in the UI constructor cpick was not correctly initialized
            we have to re-set the coupling dst to the actual frame.
        */
        auto* f = dynamic_cast<Window*> (frame);
        if (f) {
            cpick->set_dst (f->damaged ());
            cpick->enable ();
        }
    }
    void deactivate () { cpick->disable (); }
    virtual ~UI ();

    CoreProcess*       get_parent () { return parent; }
    const CoreProcess* get_parent () const { return parent; }

    DoubleProperty* move_x () { return _move_x; }
    DoubleProperty* move_y () { return _move_y; }
    DoubleProperty* press_x () { return _press_x; }
    DoubleProperty* press_y () { return _press_y; }
    DoubleProperty* release_x () { return _release_x; }
    DoubleProperty* release_y () { return _release_y; }
    DoubleProperty* wheel_x () { return _wheel_x; }
    DoubleProperty* wheel_y () { return _wheel_y; }

    DoubleProperty* local_move_x () { return _local_move_x; }
    DoubleProperty* local_move_y () { return _local_move_y; }
    DoubleProperty* local_press_x () { return _local_press_x; }
    DoubleProperty* local_press_y () { return _local_press_y; }
    DoubleProperty* local_release_x () { return _local_release_x; }
    DoubleProperty* local_release_y () { return _local_release_y; }
    DoubleProperty* local_wheel_x () { return _local_wheel_x; }
    DoubleProperty* local_wheel_y () { return _local_wheel_y; }

    DoubleProperty* mouse_press_x () { return _mouse_press_x; }
    DoubleProperty* mouse_press_y () { return _mouse_press_y; }
    DoubleProperty* mouse_release_x () { return _mouse_release_x; }
    DoubleProperty* mouse_release_y () { return _mouse_release_y; }
    DoubleProperty* mouse_move_x () { return _mouse_move_x; }
    DoubleProperty* mouse_move_y () { return _mouse_move_y; }
    DoubleProperty* mouse_wheel_x () { return _mouse_wheel_x; }
    DoubleProperty* mouse_wheel_y () { return _mouse_wheel_y; }

    DoubleProperty* mouse_local_press_x () { return _mouse_local_press_x; }
    DoubleProperty* mouse_local_press_y () { return _mouse_local_press_y; }
    DoubleProperty* mouse_local_release_x () { return _mouse_local_release_x; }
    DoubleProperty* mouse_local_release_y () { return _mouse_local_release_y; }
    DoubleProperty* mouse_local_move_x () { return _mouse_local_move_x; }
    DoubleProperty* mouse_local_move_y () { return _mouse_local_move_y; }
    DoubleProperty* mouse_local_wheel_x () { return _mouse_local_wheel_x; }
    DoubleProperty* mouse_local_wheel_y () { return _mouse_local_wheel_y; }

    DoubleProperty* stylus_pen_pressure () { return _stylus_pen_pressure; }
    FatProcess*     stylus_pen_press () { return _stylus_pen_press; }
    DoubleProperty* stylus_pen_press_x () { return _stylus_pen_press_x; }
    DoubleProperty* stylus_pen_press_y () { return _stylus_pen_press_y; }
    FatProcess*     stylus_pen_release () { return _stylus_pen_release; }
    DoubleProperty* stylus_pen_release_x () { return _stylus_pen_release_x; }
    DoubleProperty* stylus_pen_release_y () { return _stylus_pen_release_y; }
    FatProcess*     stylus_pen_move () { return _stylus_pen_move; }
    DoubleProperty* stylus_pen_move_x () { return _stylus_pen_move_x; }
    DoubleProperty* stylus_pen_move_y () { return _stylus_pen_move_y; }
    FatProcess*     stylus_pen_enter () { return _stylus_pen_enter; }
    FatProcess*     stylus_pen_leave () { return _stylus_pen_leave; }

    DoubleProperty* stylus_eraser_pressure () { return _stylus_eraser_pressure; }
    FatProcess*     stylus_eraser_press () { return _stylus_eraser_press; }
    DoubleProperty* stylus_eraser_press_x () { return _stylus_eraser_press_x; }
    DoubleProperty* stylus_eraser_press_y () { return _stylus_eraser_press_y; }
    FatProcess*     stylus_eraser_release () { return _stylus_eraser_release; }
    DoubleProperty* stylus_eraser_release_x () { return _stylus_eraser_release_x; }
    DoubleProperty* stylus_eraser_release_y () { return _stylus_eraser_release_y; }
    FatProcess*     stylus_eraser_move () { return _stylus_eraser_move; }
    DoubleProperty* stylus_eraser_move_x () { return _stylus_eraser_move_x; }
    DoubleProperty* stylus_eraser_move_y () { return _stylus_eraser_move_y; }
    FatProcess*     stylus_eraser_enter () { return _stylus_eraser_enter; }
    FatProcess*     stylus_eraser_leave () { return _stylus_eraser_leave; }

    DoubleProperty* wheel_dx () { return _wheel_dx; }
    DoubleProperty* wheel_dy () { return _wheel_dy; }
    FatProcess*     press () { return _press; }
    FatProcess*     left () { return _left; }
    FatProcess*     right () { return _right; }
    FatProcess*     middle () { return _middle; }
    FatProcess*     move () { return _move; }
    FatProcess*     release () { return _release; }
    FatProcess*     enter () { return _enter; }
    FatProcess*     leave () { return _leave; }
    FatProcess*     mouse () { return _mouse; }
    FatProcess*     mouse_press () { return _mouse_press; }
    FatProcess*     mouse_release () { return _mouse_release; }
    FatProcess*     mouse_move () { return _mouse_move; }
    FatProcess*     mouse_enter () { return _mouse_enter; }
    FatProcess*     mouse_leave () { return _mouse_leave; }
    FatProcess*     touches () { return _touches; }
    FatProcess*     left_press () { return _left_press; }
    FatProcess*     left_release () { return _left_release; }
    FatProcess*     right_press () { return _right_press; }
    FatProcess*     right_release () { return _right_release; }
    FatProcess*     middle_press () { return _middle_press; }
    FatProcess*     middle_release () { return _middle_release; }
    FatProcess*     wheel () { return _wheel; }
    FatProcess*     mouse_wheel () { return _mouse_wheel; }

  private:
    BoolProperty*     pickable;
    CouplingWithData* cpick;
    DoubleProperty *  _move_x, *_move_y, *_press_x, *_press_y, *_release_x, *_release_y, *_wheel_x, *_wheel_y, *_local_move_x, *_local_move_y, *_local_press_x, *_local_press_y, *_local_release_x, *_local_release_y, *_local_wheel_x, *_local_wheel_y;
    DoubleProperty *  _mouse_press_x, *_mouse_press_y, *_mouse_release_x, *_mouse_release_y, *_mouse_move_x, *_mouse_move_y, *_mouse_wheel_x, *_mouse_wheel_y;
    DoubleProperty *  _mouse_local_press_x, *_mouse_local_press_y, *_mouse_local_release_x, *_mouse_local_release_y, *_mouse_local_move_x, *_mouse_local_move_y, *_mouse_local_wheel_x, *_mouse_local_wheel_y;
    DoubleProperty *  _wheel_dx, *_wheel_dy;
    DoubleProperty *  _stylus_pen_press_x, *_stylus_pen_press_y, *_stylus_eraser_press_x, *_stylus_eraser_press_y;
    DoubleProperty *  _stylus_pen_release_x, *_stylus_pen_release_y, *_stylus_eraser_release_x, *_stylus_eraser_release_y;
    DoubleProperty *  _stylus_pen_move_x, *_stylus_pen_move_y, *_stylus_eraser_move_x, *_stylus_eraser_move_y;
    DoubleProperty *  _stylus_pen_pressure, *_stylus_eraser_pressure;
    FatProcess *      _press, *_left, *_right, *_middle, *_move, *_release, *_enter, *_leave, *_mouse, *_mouse_press, *_mouse_release, *_mouse_move, *_mouse_enter, *_mouse_leave, *_touches, *_wheel, *_mouse_wheel;
    FatProcess *      _left_press, *_left_release, *_right_press, *_right_release, *_middle_press, *_middle_release;
    FatProcess *      _stylus_pen_press, *_stylus_eraser_press;
    FatProcess *      _stylus_pen_release, *_stylus_eraser_release;
    FatProcess *      _stylus_pen_move, *_stylus_eraser_move;
    FatProcess *      _stylus_pen_enter, *_stylus_eraser_enter;
    FatProcess *      _stylus_pen_leave, *_stylus_eraser_leave;
    FatProcess *      _stylus_pen, *_stylus_eraser;
    CoreProcess*      parent;
};
} // namespace djnn
