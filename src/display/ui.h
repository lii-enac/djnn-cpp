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

extern djnnstl::vector<djnnstl::string> __ui_interface;

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
    DoubleProperty* wheel_x () { return _wheel_x; }
    DoubleProperty* wheel_y () { return _wheel_y; }

    DoubleProperty* local_move_x () { return _local_move_x; }
    DoubleProperty* local_move_y () { return _local_move_y; }
    DoubleProperty* local_press_x () { return _local_press_x; }
    DoubleProperty* local_press_y () { return _local_press_y; }
    DoubleProperty* local_wheel_x () { return _local_wheel_x; }
    DoubleProperty* local_wheel_y () { return _local_wheel_y; }

    DoubleProperty* mouse_press_x () { return _mouse_press_x; }
    DoubleProperty* mouse_press_y () { return _mouse_press_y; }
    DoubleProperty* mouse_move_x () { return _mouse_move_x; }
    DoubleProperty* mouse_move_y () { return _mouse_move_y; }
    DoubleProperty* mouse_wheel_x () { return _mouse_wheel_x; }
    DoubleProperty* mouse_wheel_y () { return _mouse_wheel_y; }

    DoubleProperty* mouse_local_press_x () { return _mouse_local_press_x; }
    DoubleProperty* mouse_local_press_y () { return _mouse_local_press_y; }
    DoubleProperty* mouse_local_move_x () { return _mouse_local_move_x; }
    DoubleProperty* mouse_local_move_y () { return _mouse_local_move_y; }
    DoubleProperty* mouse_local_wheel_x () { return _mouse_local_wheel_x; }
    DoubleProperty* mouse_local_wheel_y () { return _mouse_local_wheel_y; }

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
    DoubleProperty *  _move_x, *_move_y, *_press_x, *_press_y, *_wheel_x, *_wheel_y, *_local_move_x, *_local_move_y, *_local_press_x, *_local_press_y, *_local_wheel_x, *_local_wheel_y;
    DoubleProperty *  _mouse_press_x, *_mouse_press_y, *_mouse_move_x, *_mouse_move_y, *_mouse_wheel_x, *_mouse_wheel_y;
    DoubleProperty *  _mouse_local_press_x, *_mouse_local_press_y, *_mouse_local_move_x, *_mouse_local_move_y, *_mouse_local_wheel_x, *_mouse_local_wheel_y;
    DoubleProperty *  _wheel_dx, *_wheel_dy;
    FatProcess *      _press, *_left, *_right, *_middle, *_move, *_release, *_enter, *_leave, *_mouse, *_mouse_press, *_mouse_release, *_mouse_move, *_mouse_enter, *_mouse_leave, *_touches, *_wheel, *_mouse_wheel;
    FatProcess *      _left_press, *_left_release, *_right_press, *_right_release, *_middle_press, *_middle_release;
    CoreProcess*      parent;
};
} // namespace djnn
