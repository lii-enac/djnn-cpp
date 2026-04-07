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

#include <float.h>
#include <math.h>

#include "display/ui.h"

#include "core/core-dev.h" //  add/remove edge
#include "core/tree/list.h"
#include "display/window.h"

// #include "core/utils/error.h"
// #include "utils/debug.h"
// #include "core/utils/iostream.h"


namespace djnn {
//const djnnstl::vector<djnnstl::string> __ui_interface = {"pickable", "press", "release", "left", "right", "middle", "move", "enter", "leave", "touches", "mouse", "wheel", "pen", "eraser"};

const djnnstl::map<djnnstl::string, bool> __ui_interface = {
    {"pickable", true},
    {"press", true},
    {"release", true},
    {"left", true},
    {"right", true},
    {"middle", true},
    {"move", true},
    {"enter", true},
    {"leave", true},
    {"touches", true},
    {"mouse", true},
    {"wheel", true},
    {"pen", true},
    {"eraser", true}
    };


UI::UI (CoreProcess* parent_, FatProcess* f)
    : _parent (parent_)
    , cpick (nullptr)
#if 0
#define EVENT(ev, type, parent) \
    , _##ev (nullptr)
#include "ui_event/ui_event_parent.h"
#include "ui_event/ui_event.h"
#undef EVENT
#define EVENT(ev, type, parent) \
    , _##ev##_x (nullptr) \
    , _##ev##_y (nullptr)
#include "ui_event/ui_event_xy.h"
#include "ui_event/ui_event_xy_local.h"
#undef EVENT
#define EVENT(ev, type, parent) \
    , _##ev (nullptr)
EVENT(wheel_dx, DoubleProperty, wheel)
EVENT(wheel_dy, DoubleProperty, wheel)
EVENT(stylus_pen_pressure, DoubleProperty, parent)
EVENT(stylus_eraser_pressure, DoubleProperty, parent)
#undef EVENT
#endif
{
    pickable = new BoolProperty (_parent, "pickable", true);
    // FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    // cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION, nullptr);
    // if (f != nullptr)
    //   cpick->enable (f);
    FatProcess* update = f;
    if (update)
        update = dynamic_cast<Window*> (f)->damaged ();
    cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION);
    if (f != nullptr)
        cpick->enable ();


#if 1

#define EVENT(ev, type, parent) \
    _##ev = new type(_##parent, #ev); \
    _##ev->set_activation_state (ACTIVATED);
#include "ui_event/ui_event_parent.h"
#undef EVENT

#define EVENT(ev, type, parent) \
    _##ev = new type(_##parent, #ev);
#include "ui_event/ui_event.h"
#undef EVENT

// #define EVENT(ev, type, parent)
//     _##ev = new type(_##parent, #ev, 0); 
// EVENT(wheel_dx, DoubleProperty, wheel)
// EVENT(wheel_dy, DoubleProperty, wheel)
// #undef EVENT

    _wheel_dx = new DoubleProperty(_wheel, "dx", 0);
    _wheel_dy = new DoubleProperty(_wheel, "dy", 0);
    
#define EVENT(ev, type, parent) \
    _##ev = new type(_##parent, #ev, 0); 
EVENT(stylus_pen_pressure, DoubleProperty, parent)
EVENT(stylus_eraser_pressure, DoubleProperty, parent)
#undef EVENT

    // make sure enter events are processed before leave events
    
#define EVENT(enter, leave) \
    graph_add_edge (_##enter, _##leave);
    /*bool enter_and_leave = ##enter && _##leave; \
    if (!enter_and_leave && (_##enter && _##leave)) graph_add_edge (_##enter, _##leave);*/
    EVENT (enter, leave);             
    EVENT (mouse_enter, mouse_leave);
    EVENT (stylus_pen_enter, stylus_pen_leave);             
    EVENT (stylus_eraser_enter, stylus_eraser_leave);
#undef EVENT

    CoreProcess * _nullptr_ = nullptr;

#define EVENT(ev, type, parent) \
    _##ev##_x = new DoubleProperty (_##parent, #ev "_x", 0); \
    _##ev##_y = new DoubleProperty (_##parent, #ev "_y", 0);
#include "ui_event/ui_event_xy_local.h"
#undef EVENT

#define EVENT(ev, type, parent) \
    _##ev##_x = new DoubleProperty (_##parent, #ev "_x", 0); \
    _##ev##_y = new DoubleProperty (_##parent, #ev "_y", 0); \
    ev()->add_symbol ("x", _##ev##_x); \
    ev()->add_symbol ("y", _##ev##_y); \
    ev()->add_symbol ("local_x", _##ev##_local_x); \
    ev()->add_symbol ("local_y", _##ev##_local_y);
#include "ui_event/ui_event_xy.h"
#undef EVENT

#endif
}


#if 0
#define EVENT(ev, type, parent) \
type* \
UI::ev () \
{ \
    if (_##ev) return _##ev; \
    _##ev = new type(_##parent, #ev); \
    _##ev->set_activation_state (ACTIVATED); \
    return _##ev; \
}
#include "ui_event/ui_event_parent.h"
#undef EVENT


#define EVENT(ev, type, parent) \
type* \
UI::ev () \
{ \
    if (_##ev) return _##ev; \
    _##ev = new type(_##parent, #ev); \
    return _##ev; \
}
#include "ui_event/ui_event.h"
#undef EVENT

#define EVENT(ev, type, parent) \
DoubleProperty* \
UI::ev () \
{ \
    if (_##ev) return _##ev; \
    _##ev = new type(_##parent, #ev, 0); \
    return _##ev; \
}
EVENT(wheel_dx, DoubleProperty, wheel)
EVENT(wheel_dy, DoubleProperty, wheel)
EVENT(stylus_pen_pressure, DoubleProperty, parent)
EVENT(stylus_eraser_pressure, DoubleProperty, parent)
#undef EVENT

static CoreProcess * nullptr_ () { return nullptr; }

#define EVENT(ev, type, parent) \
DoubleProperty* \
UI::ev##_x () \
{ \
    if (_##ev##_x) return _##ev##_x; \
    _##ev##_x = new DoubleProperty(parent (), #ev "_x", 0); \
    ev()->add_symbol ("x", _##ev##_x); \
    ev()->add_symbol ("local_x", ev##_local_x ()); \
    return _##ev##_x; \
} \
DoubleProperty* \
UI::ev##_y () \
{ \
    if (_##ev##_y) return _##ev##_y; \
    _##ev##_y = new DoubleProperty(parent (), #ev "_y", 0); \
    ev()->add_symbol ("y", _##ev##_y); \
    ev()->add_symbol ("local_y", ev##_local_y ()); \
    return _##ev##_y; \
}
#include "ui_event/ui_event_xy.h"
#undef EVENT


#define EVENT(ev, type, parent) \
DoubleProperty* \
UI::ev##_x () \
{ \
    if (_##ev##_x) return _##ev##_x; \
    _##ev##_x = new DoubleProperty(parent (), #ev "_x", 0); \
    return _##ev##_x; \
} \
DoubleProperty* \
UI::ev##_y () \
{ \
    if (_##ev##_y) return _##ev##_y; \
    _##ev##_y = new DoubleProperty(parent (), #ev "_y", 0); \
    return _##ev##_y; \
}
#include "ui_event/ui_event_xy_local.h"
#undef EVENT

#endif

UI::~UI ()
{
    /* HACK
     * clear _children of the (djnn:List) touches before
     * deleting it (touches), to avoid ~Container to delete touches inside "touches list"
     * which are also in a _active_touches list
     */
    if (_touches)
        ((List*)_touches)->clear ();

#if 1
#define EVENT(enter, leave) \
    /*graph_remove_edge (_##enter, _##leave);*/ \
    /*bool enter_and_leave = ##enter && _##leave;*/ \
    /*if (!enter_and_leave && (_##enter && _##leave)) graph_remove_edge (_##enter, _##leave);*/ \
    if (_##enter && _##leave) graph_remove_edge (_##enter, _##leave);
    EVENT (enter, leave);             
    EVENT (mouse_enter, mouse_leave);
    EVENT (stylus_pen_enter, stylus_pen_leave);             
    EVENT (stylus_eraser_enter, stylus_eraser_leave);
#undef EVENT
#endif

    CoreProcess * _nullptr_ = nullptr;

#define EVENT(ev, type, parent) \
    if (_##ev) { \
        _##ev->remove_symbol ("local_x"); \
        _##ev->remove_symbol ("local_y"); \
        _##ev->remove_symbol ("x"); \
        _##ev->remove_symbol ("y"); \
    } \
    if (_##parent) _##parent->remove_child(_##ev##_x); \
    if (_##parent) _##parent->remove_child(_##ev##_y); \
    delete _##ev##_x; \
    delete _##ev##_y;
#include "ui_event/ui_event_xy.h"
#undef EVENT

#define EVENT(ev, type, parent) \
    if (_##parent) _##parent->remove_child(_##ev##_x); \
    if (_##parent) _##parent->remove_child(_##ev##_y); \
    delete _##ev##_x; \
    delete _##ev##_y;
#include "ui_event/ui_event_xy_local.h"
#undef EVENT

#define EVENT(ev, type, parent) \
    if (_##ev && _##parent) _##parent->remove_child(_##ev); \
    delete _##ev;
EVENT(wheel_dx, DoubleProperty, wheel)
EVENT(wheel_dy, DoubleProperty, wheel)
EVENT(stylus_pen_pressure, DoubleProperty, parent)
EVENT(stylus_eraser_pressure, DoubleProperty, parent)
#include "ui_event/ui_event.h"
#include "ui_event/ui_event_parent.h"
#undef EVENT

    delete cpick;
    _parent->remove_child (pickable);
    delete pickable;

#if 0
    _mouse_wheel->remove_symbol ("local_y");
    _mouse_wheel->remove_symbol ("local_x");
    _mouse_wheel->remove_symbol ("y");
    _mouse_wheel->remove_symbol ("x");

    _mouse_release->remove_symbol ("local_y");
    _mouse_release->remove_symbol ("local_x");
    _mouse_release->remove_symbol ("y");
    _mouse_release->remove_symbol ("x");

    _mouse_press->remove_symbol ("local_y");
    _mouse_press->remove_symbol ("local_x");
    _mouse_press->remove_symbol ("y");
    _mouse_press->remove_symbol ("x");

    _mouse_move->remove_symbol ("local_y");
    _mouse_move->remove_symbol ("local_x");
    _mouse_move->remove_symbol ("y");
    _mouse_move->remove_symbol ("x");

    delete _mouse_local_wheel_y;
    delete _mouse_local_wheel_x;
    delete _mouse_local_move_y;
    delete _mouse_local_move_x;
    delete _mouse_local_release_y;
    delete _mouse_local_release_x;
    delete _mouse_local_press_y;
    delete _mouse_local_press_x;

    delete _mouse_wheel_y;
    delete _mouse_wheel_x;
    delete _mouse_move_y;
    delete _mouse_move_x;
    delete _mouse_release_y;
    delete _mouse_release_x;
    delete _mouse_press_y;
    delete _mouse_press_x;

    _release->remove_symbol ("local_y");
    _release->remove_symbol ("local_x");
    _release->remove_symbol ("y");
    _release->remove_symbol ("x");

    _press->remove_symbol ("local_y");
    _press->remove_symbol ("local_x");
    _press->remove_symbol ("y");
    _press->remove_symbol ("x");

    _move->remove_symbol ("local_y");
    _move->remove_symbol ("local_x");
    _move->remove_symbol ("y");
    _move->remove_symbol ("x");

    delete _local_release_y;
    delete _local_release_x;
    delete _local_press_y;
    delete _local_press_x;
    delete _local_move_y;
    delete _local_move_x;

    _wheel->remove_child (_local_wheel_y);
    _wheel->remove_child (_local_wheel_x);
    delete _local_wheel_y;
    delete _local_wheel_x;
    _wheel->remove_child (_wheel_y);
    _wheel->remove_child (_wheel_x);
    delete _wheel_y;
    delete _wheel_x;
    _wheel->remove_child (_wheel_dy);
    _wheel->remove_child (_wheel_dx);
    delete _wheel_dy;
    delete _wheel_dx;

    delete _release_y;
    delete _release_x;
    delete _press_y;
    delete _press_x;
    delete _move_y;
    delete _move_x;

    _wheel->set_activation_state (DEACTIVATED);
    _middle->set_activation_state (DEACTIVATED);
    _right->set_activation_state (DEACTIVATED);
    _left->set_activation_state (DEACTIVATED);
    _mouse->set_activation_state (DEACTIVATED);
    _touches->set_activation_state (DEACTIVATED);

    _parent->remove_child (_touches);
    /* HACK
     * clear _children of the (djnn:List) touches before
     * deleting it (touches), to avoid ~Container to delete touches inside "touches list"
     * which are also in a _active_touches list
     */
    ((List*)_touches)->clear ();
    delete _touches;

    graph_remove_edge (_mouse_enter, _mouse_leave);
    graph_remove_edge (_enter, _leave);

    _mouse->remove_child (_mouse_wheel);
    delete _mouse_wheel;
    _mouse->remove_child (_mouse_leave);
    delete _mouse_leave;
    _mouse->remove_child (_mouse_enter);
    delete _mouse_enter;
    _mouse->remove_child (_mouse_move);
    delete _mouse_move;
    _mouse->remove_child (_mouse_release);
    delete _mouse_release;
    _mouse->remove_child (_mouse_press);
    delete _mouse_press;
    _parent->remove_child (_wheel);
    delete _wheel;
    _parent->remove_child (_mouse);
    delete _mouse;
    _parent->remove_child (_leave);
    delete _leave;
    _parent->remove_child (_enter);
    delete _enter;
    _parent->remove_child (_release);
    delete _release;
    _middle->remove_child (_middle_release);
    delete _middle_release;
    _middle->remove_child (_middle_press);
    delete _middle_press;
    _parent->remove_child (_middle);
    delete _middle;
    _right->remove_child (_right_release);
    delete _right_release;
    _right->remove_child (_right_press);
    delete _right_press;
    _parent->remove_child (_right);
    delete _right;
    _left->remove_child (_left_release);
    delete _left_release;
    _left->remove_child (_left_press);
    delete _left_press;
    _parent->remove_child (_left);
    delete _left;
    _parent->remove_child (_move);
    delete _move;
    _parent->remove_child (_press);
    delete _press;

    delete cpick;
    _parent->remove_child (pickable);
    delete pickable;
#endif
}

void
UI::activate (FatProcess* frame)
{
    /*
        if the frame was null in the UI constructor cpick was not correctly initialized
        we have to re-set the coupling dst to the actual frame.
    */
    auto* f = dynamic_cast<Window*> (frame);
    if (f && cpick) {
        cpick->set_dst (f->damaged ());
        cpick->enable ();
    }
}

void
UI::deactivate ()
{
    if (cpick) cpick->disable ();
}

} /* namespace djnn */
