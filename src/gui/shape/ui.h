#pragma once

#include "abstract_gshape.h"
#include "core/tree/double_property.h"
#include "core/tree/int_property.h"

#include "display/window.h"

namespace djnn {
    class UI {
    public:
        UI (ParentProcess* parent, FatProcess *f);
        bool is_pickable () { return pickable->get_value (); }
        //void activate (FatProcess* frame) { cpick->enable (frame); }
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
        friend class Picking;
    private:
        BoolProperty *pickable;
        CouplingWithData *cpick;
        DoubleProperty *move_x, *move_y, *press_x, *press_y, *local_move_x, *local_move_y, *local_press_x, *local_press_y;
        DoubleProperty *mouse_press_x, *mouse_press_y, *mouse_move_x, *mouse_move_y;
        DoubleProperty *mouse_local_press_x, *mouse_local_press_y, *mouse_local_move_x, *mouse_local_move_y;
        FatProcess *press, *left, *right, *middle, *move, *release, *enter, *leave, *mouse, *mouse_press, *mouse_release, *mouse_move, *mouse_enter, *mouse_leave, *touches;
        FatProcess *left_press, *left_release, *right_press, *right_release, *middle_press, *middle_release;
        ParentProcess* parent;
    };

    inline bool is_pickable (AbstractGShape * s) {
      return s->has_ui () && s->get_ui ()->is_pickable ();
    }
}