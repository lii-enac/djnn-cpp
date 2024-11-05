/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Chatty <chatty@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <assert.h>

#include "color_picking.h"
#include "core/core-dev.h"
#include "core/utils/error.h"
#include "core/utils/ext/remotery/Remotery.h"
#include "core/utils/to_string.h"
#include "display/ui.h"
#include "gui/transformation/homography.h"
namespace djnn {
extern int            mouse_tracking; // in display
extern FatProcess*    GenericMouse;   // in gui
static vector<Touch*> gc_touches;     // Garbage_Colector touches

Picking::Picking (Window* win)
    : _win (win), _caught_shape (nullptr), _hovered_shape (nullptr), _mouse_released (true)
{
    // FIXME: uniformiser l'API
    //_win->set_picking_view(this);
}

Picking::~Picking ()
{
    // DBG;
}

void
Picking::init ()
{
}

void
Picking::object_deactivated (PickUI* picked_shape)
{
    /* Reset _caught_shape to nullptr if this object has been removed from picking_view */
    if (_caught_shape == picked_shape) {
        _caught_shape = nullptr;
    }

    /* Reset _hovered_shape to nullptr if this object has been removed from picking_view */
    if (_hovered_shape == picked_shape) {
        _hovered_shape = nullptr;
    }

    /* reset all _active_touches[x]->init_shape ans _active_touches[x]->current_shape */
    for (auto iter = _active_touches.begin (); iter != _active_touches.end (); ++iter) {
        Touch* t = iter->second;
        assert (t);
        if (t->init_shape () == picked_shape)
            t->set_init_shape (nullptr);
        if (t->current_shape () == picked_shape)
            t->set_current_shape (nullptr);
    }
}

#ifndef DJNN_NO_DEBUG
bool
Picking::check_for_ui (PickUI* s)
{
    if (!s || !s->ui ()) {
        // warning (nullptr, "no ui in picked gobj");
        return false;
    }
    return true;
}
#endif

bool
Picking::tabletEnterLeave (PickUI* picked, stylus_type stylus_type)
{
    if (picked && picked != _hovered_shape) {

        if (!check_for_ui(picked)) return false;

        if (_hovered_shape) {
            if (stylus_type == STYLUS_PEN)
                _hovered_shape->ui()->stylus_pen_leave()->schedule_activation();
            else if (stylus_type == STYLUS_ERASER)
                _hovered_shape->ui()->stylus_eraser_leave()->schedule_activation();
            GRAPH_EXEC;
        }

        if (stylus_type == STYLUS_PEN)
            picked->ui()->stylus_pen_enter()->schedule_activation();
        else if (stylus_type == STYLUS_ERASER)
            picked->ui()->stylus_eraser_enter()->schedule_activation();

        _hovered_shape = picked;
        // cut the event synthesis when enter/leave happened
        GRAPH_EXEC;

    } else if (!picked && _hovered_shape) {

        if (stylus_type == STYLUS_PEN)
            _hovered_shape->ui()->stylus_pen_leave()->schedule_activation();
        else if (stylus_type == STYLUS_ERASER)
            _hovered_shape->ui()->stylus_eraser_leave()->schedule_activation();

        _hovered_shape = nullptr;
        // cut the event synthesis when enter/leave happened
        GRAPH_EXEC;
    }

    return false;
}

bool
Picking::genericEnterLeave (PickUI* picked)
{
    auto s = picked;
    if (s) {
        if (s != _hovered_shape) {
            if (!check_for_ui (s))
                return false;
            if (_hovered_shape != nullptr) {
                FatProcess* p1 = _hovered_shape->ui ()->leave ();
                FatProcess* p2 = _hovered_shape->ui ()->mouse_leave ();
                p1->schedule_activation ();
                p2->schedule_activation ();
                // cut the event synthesis when enter/leave happened
                GRAPH_EXEC;
            }
            s->ui ()->enter ()->schedule_activation ();
            s->ui ()->mouse_enter ()->schedule_activation ();

            /* new _hovered_shape */
            _hovered_shape = s;
            // cut the event synthesis when enter/leave happened
            GRAPH_EXEC;
        }
    } else if (_hovered_shape != nullptr) {
        FatProcess* p1 = _hovered_shape->ui ()->leave ();
        FatProcess* p2 = _hovered_shape->ui ()->mouse_leave ();
        p1->schedule_activation ();
        p2->schedule_activation ();

        /* reset */
        _hovered_shape = nullptr;

        // cut the event synthesis when enter/leave happened
        GRAPH_EXEC;
    }

    return false;
}

bool
Picking::genericCheckShapeAfterDraw (double x, double y)
{

    /* release happend and no mouse tracking */
    if (_mouse_released && mouse_tracking == 0)
        return false;

    /* shape */
    bool    exec_ = false;
    PickUI* s     = this->pick (x, y);
    if (s) {
        if (!check_for_ui (s))
            return false;
        double cur_move_x = s->ui ()->move_x ()->get_value ();
        double cur_move_y = s->ui ()->move_y ()->get_value ();

        if (s == _caught_shape) {
            if (cur_move_x == x && cur_move_y == y)
                return exec_; /* return false */
            else {
                s->set_mouse_local_coords (x, y, true);
            }
        }
        s->ui ()->move ()->schedule_activation ();
        s->ui ()->mouse_move ()->schedule_activation ();
    }

    exec_ |= genericEnterLeave (s);

    // TODO : MP
    // chack for touch and tablet/stylus event 
    // tabletEnterLeave ? 

    return exec_;
}

void
Picking::common_press_setting (double x, double y, PickUI* s)
{
    s->ui ()->press_x ()->set_value (x, true);
    s->ui ()->press_y ()->set_value (y, true);
    s->ui ()->move_x ()->set_value (x, true);
    s->ui ()->move_y ()->set_value (y, true);
}

void
Picking::common_press_notify (PickUI* s)
{
    if (s != _hovered_shape)
        s->ui ()->enter ()->schedule_activation ();
    s->ui ()->press ()->schedule_activation ();

    /* reset _hovered_shape and _caught_shape */
    _caught_shape  = s;
    _hovered_shape = s;
}

bool
Picking::genericMousePress (double x, double y, mouse_button button)
{
    _mouse_released = false;
    bool exec_      = false;

    /* windows setting */
    _win->press_x ()->set_value (x, true);
    _win->press_y ()->set_value (y, true);
    // _win->move_x ()->set_value (x, true);
    // _win->move_y ()->set_value (y, true);

    /* shape */
    PickUI* s = this->pick (x, y);
    // std::cerr << s << " " << typeid(s).name() << std::endl;
    if (s != nullptr && check_for_ui (s)) {

        /* setting */
        common_press_setting (x, y, s);
        s->ui ()->mouse_press_x ()->set_value (x, true);
        s->ui ()->mouse_press_y ()->set_value (y, true);
        // s->ui()->mouse_move_x->set_value (x, true);
        // s->ui()->mouse_move_y->set_value (y, true);
        s->set_mouse_local_coords (x, y, false);

        /* event */
        if (s != _hovered_shape)
            s->ui ()->mouse_enter ()->schedule_activation ();
        s->ui ()->mouse_press ()->schedule_activation ();
        common_press_notify (s);

        exec_ = true;
    }

    /* button */
    switch (button) {
    case BUTTON_LEFT:
        ((GUIMouse*)GenericMouse)->left ()->press ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->left_press ()->schedule_activation ();
        }
        break;
    case BUTTON_RIGHT:
        ((GUIMouse*)GenericMouse)->right ()->press ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->right_press ()->schedule_activation ();
        }
        break;
    case BUTTON_MIDDLE:
        ((GUIMouse*)GenericMouse)->middle ()->press ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->middle_press ()->schedule_activation ();
        }
        break;
    default:
        ((GUIMouse*)GenericMouse)->left ()->press ()->schedule_activation ();
    }
    if (((GUIMouse*)GenericMouse)->left ()->press ()->has_coupling () || ((GUIMouse*)GenericMouse)->right ()->press ()->has_coupling () || ((GUIMouse*)GenericMouse)->middle ()->press ()->has_coupling ()) {
        exec_ = true;
    }

    /* windows event schedule event After shape event*/
    if (_win->press ()->has_coupling ()) {
        _win->press ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->press_x ()->has_coupling ()) {
        _win->press_x ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->press_y ()->has_coupling ()) {
        _win->press_y ()->schedule_activation ();
        exec_ = true;
    }

    return exec_;
}

bool
Picking::genericTouchPress (double x, double y, int id, float pressure)
{
    if (!gc_touches.empty ()) {
        // std::cout << "Press detected, clearing all touches..." << std::endl;
        for (const auto& touch : gc_touches) {
            delete touch;
        }
        gc_touches.clear ();
    }

    /* touch management */
    djnnstl::map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch*                              t;
    if (it != _active_touches.end ()) {
        t = it->second;
        _win->touches ()->remove_child (t);
        if (t->init_shape () != nullptr) {
            t->init_shape ()->ui ()->touches ()->remove_child (t);
            t->leave ();
        }
        _active_touches.erase (it);

        /* delay touch delete in an GC */
        gc_touches.push_back (t);
    }
    t                   = new Touch (_win->touches (), djnnstl::to_string (id), id, x, y, pressure);
    _active_touches[id] = t;

    /* picking/shape management */
    PickUI* s = this->pick (x, y);
    if (s != nullptr) {
        if (!check_for_ui (s))
            return false;
        common_press_setting (x, y, s);
        t->set_init_shape (s);
        t->set_current_shape (s);
        t->set_touch_local_coords (s, x, y, false);
        common_press_notify (s);
        /* press event on touch is replace by touches/$added */
        s->ui ()->touches ()->add_child (t, djnnstl::to_string (id));
        t->enter ();
    }
    return true;
}

bool
Picking::genericTabletPress (double x, double y, stylus_type stylus_type, mouse_button button, double pressure)
{
    _mouse_released = false;
    bool exec_      = false;

    // Set stylus parameters in the window
    if (stylus_type == STYLUS_PEN) {
        _win->stylus_pen_press_x ()->set_value (x, true);
        _win->stylus_pen_press_y ()->set_value (y, true);
        _win->stylus_pen_pressure ()->set_value (pressure, true);
    } else if (stylus_type == STYLUS_ERASER) {
        _win->stylus_eraser_press_x ()->set_value (x, true);
        _win->stylus_eraser_press_y ()->set_value (y, true);
        _win->stylus_eraser_pressure ()->set_value (pressure, true);
    }

    // Handle PickUI object
    PickUI* s = this->pick (x, y);
    if (s && check_for_ui (s)) {
        // Set stylus parameters in PickUI
        if (stylus_type == STYLUS_PEN) {
            s->ui ()->stylus_pen_press_x ()->set_value (x, true);
            s->ui ()->stylus_pen_press_y ()->set_value (y, true);
            s->ui ()->stylus_pen_pressure ()->set_value (pressure, true);

            if (s != _hovered_shape)
                s->ui ()->stylus_pen_enter ()->schedule_activation ();
            s->ui ()->stylus_pen_press ()->schedule_activation ();
        } else if (stylus_type == STYLUS_ERASER) {
            s->ui ()->stylus_eraser_press_x ()->set_value (x, true);
            s->ui ()->stylus_eraser_press_y ()->set_value (y, true);
            s->ui ()->stylus_eraser_pressure ()->set_value (pressure, true);

            if (s != _hovered_shape)
                s->ui ()->stylus_eraser_enter ()->schedule_activation ();
            s->ui ()->stylus_eraser_press ()->schedule_activation ();
        }

        // Update captured shapes
        _caught_shape  = s;
        _hovered_shape = s;
        exec_          = true;
    }

    // Handle button activation
    if (s && check_for_ui (s)) {
        switch (button) {
        case BUTTON_LEFT:
            s->ui ()->left_press ()->schedule_activation ();
            break;
        case BUTTON_RIGHT:
            s->ui ()->right_press ()->schedule_activation ();
            break;
        case BUTTON_MIDDLE:
            s->ui ()->middle_press ()->schedule_activation ();
            break;
        default:
            break;
        }
    }

    // Schedule stylus events for window after PickUI events
    if (stylus_type == STYLUS_PEN && _win->stylus_pen_press ()->has_coupling ()) {
        _win->stylus_pen_press ()->schedule_activation ();
        exec_ = true;
    } else if (stylus_type == STYLUS_ERASER && _win->stylus_eraser_press ()->has_coupling ()) {
        _win->stylus_eraser_press ()->schedule_activation ();
        exec_ = true;
    }

    return exec_;
}

bool
Picking::genericMouseMove (double x, double y)
{
    rmt_BeginCPUSample (genericMouseMove, RMTSF_Aggregate);
    bool exec_ = false;

    rmt_BeginCPUSample (windows_setting, RMTSF_Aggregate);
    /* windows setting */
    double old_x = _win->move_x ()->get_value ();
    double old_y = _win->move_y ()->get_value ();
    if (x != old_x)
        _win->move_x ()->set_value (x, true);
    if (y != old_y)
        _win->move_y ()->set_value (y, true);
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (shape, RMTSF_Aggregate);
    /* shape */
    rmt_BeginCPUSample (pick, RMTSF_Aggregate);
    PickUI* s = this->pick (x, y);
    rmt_EndCPUSample ();
    exec_ |= genericEnterLeave (s);
    if (s && check_for_ui (s)) {

        /* setting */
        if (x != old_x) {
            DoubleProperty* p1 = s->ui ()->move_x ();
            DoubleProperty* p2 = s->ui ()->mouse_move_x ();
            p1->set_value (x, true);
            p2->set_value (x, true);
            if (p1->has_coupling ())
                p1->schedule_activation ();
            if (p2->has_coupling ())
                p2->schedule_activation ();
        }
        if (y != old_y) {
            DoubleProperty* p1 = s->ui ()->move_y ();
            DoubleProperty* p2 = s->ui ()->mouse_move_y ();
            p1->set_value (y, true);
            p2->set_value (y, true);
            if (p1->has_coupling ())
                p1->schedule_activation ();
            if (p2->has_coupling ())
                p2->schedule_activation ();
        }
        s->set_mouse_local_coords (x, y, true);

        /* event */
        s->ui ()->move ()->schedule_activation ();
        s->ui ()->mouse_move ()->schedule_activation ();

        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (caught_shape, RMTSF_Aggregate);
    /* _caught_shape */
    if (_caught_shape != nullptr && _caught_shape != s) {

        /* setting */
        if (x != old_x) {
            DoubleProperty* p1 = _caught_shape->ui ()->move_x ();
            DoubleProperty* p2 = _caught_shape->ui ()->mouse_move_x ();
            p1->set_value (x, true);
            p2->set_value (x, true);
            if (p1->has_coupling ())
                p1->schedule_activation ();
            if (p2->has_coupling ())
                p2->schedule_activation ();
        }
        if (y != old_y) {
            DoubleProperty* p1 = _caught_shape->ui ()->move_y ();
            DoubleProperty* p2 = _caught_shape->ui ()->mouse_move_y ();
            p1->set_value (y, true);
            p2->set_value (y, true);
            if (p1->has_coupling ())
                p1->schedule_activation ();
            if (p2->has_coupling ())
                p2->schedule_activation ();
        }
        _caught_shape->set_mouse_local_coords (x, y, true);

        /* event */
        FatProcess* p1 = _caught_shape->ui ()->move ();
        FatProcess* p2 = _caught_shape->ui ()->mouse_move ();
        p1->schedule_activation ();
        p2->schedule_activation ();

        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (generic_mouse, RMTSF_Aggregate);
    /* generic mouse setting */
    ((GUIMouse*)GenericMouse)->x ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->y ()->set_value (y, true);
    /* generic mouse event */
    if (((GUIMouse*)GenericMouse)->move ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->move ()->schedule_activation ();
        exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->x ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->x ()->schedule_activation ();
        exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->y ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->y ()->schedule_activation ();
        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (window_event, RMTSF_Aggregate);
    /* windows event schedule event After shape event*/
    if (_win->move ()->has_coupling ()) {
        _win->move ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->move_x ()->has_coupling ()) {
        _win->move_x ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->move_y ()->has_coupling ()) {
        _win->move_y ()->schedule_activation ();
        exec_ = true;
    }
    rmt_EndCPUSample ();
    rmt_EndCPUSample ();
    return exec_;
}

bool
Picking::genericTouchMove (double x, double y, int id, float pressure)
{
    djnnstl::map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch*                              t;
    /* touch exist */
    if (it != _active_touches.end ()) {
        t = it->second;

        /* setting */
        t->set_move_x (x);
        t->set_move_y (y);
        t->set_pressure (pressure);
        t->set_id (id);

        /* shape */
        PickUI* s          = this->pick (x, y);
        PickUI* cur_shape  = t->current_shape ();
        PickUI* init_shape = t->init_shape ();
        if (s == nullptr && cur_shape != nullptr) {
            if (cur_shape != init_shape)
                cur_shape->ui ()->touches ()->remove_child (t);
            t->set_current_shape (nullptr);
            /* touch event */
            t->leave ();
        } else if (s != nullptr && s != cur_shape) {
            if (!check_for_ui (s))
                return false;
            if (cur_shape != nullptr && cur_shape != init_shape)
                cur_shape->ui ()->touches ()->remove_child (t);
            s->ui ()->touches ()->add_child (t, djnnstl::to_string (id));
            t->set_current_shape (s);
            /* setting */
            s->ui ()->move_x ()->set_value (x, true);
            s->ui ()->move_y ()->set_value (y, true);
            t->set_touch_local_coords (s, x, y, true);
            /* touch and shape event */
            s->ui ()->move ()->schedule_activation ();
            t->enter ();
        }

        /* touch event */
        t->get_move ()->schedule_activation ();
        genericEnterLeave (s);

    }
    /* touch do not exist = press */
    else {
        genericTouchPress (x, y, id, pressure);
    }

    return true;
}

bool
Picking::genericTabletMove (double x, double y, stylus_type stylus_type, double pressure)
{

    rmt_BeginCPUSample (genericTabletMove, RMTSF_Aggregate);
    bool exec_ = false;

    rmt_BeginCPUSample (windows_setting, RMTSF_Aggregate);

    // Set initial stylus coordinates and pressure in window settings
    double old_x = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_move_x ()->get_value () : _win->stylus_eraser_move_x ()->get_value ();
    double old_y = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_move_y ()->get_value () : _win->stylus_eraser_move_y ()->get_value ();

    // Update pressure in window settings
    auto win_pressure = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_pressure () : _win->stylus_eraser_pressure ();
    win_pressure->set_value (pressure, true);
    exec_ = true;

    // Update x and y coordinates if they have changed
    if (x != old_x) {
        if (stylus_type == STYLUS_PEN)
            _win->stylus_pen_move_x ()->set_value (x, true);
        else
            _win->stylus_eraser_move_x ()->set_value (x, true);
        exec_ = true;
    }
    if (y != old_y) {
        if (stylus_type == STYLUS_PEN)
            _win->stylus_pen_move_y ()->set_value (y, true);
        else
            _win->stylus_eraser_move_y ()->set_value (y, true);
        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (shape, RMTSF_Aggregate);

    // Handle shape interaction
    rmt_BeginCPUSample (pick, RMTSF_Aggregate);
    PickUI* s = this->pick (x, y);
    rmt_EndCPUSample ();
    exec_ |= tabletEnterLeave (s, stylus_type);

    if (s && check_for_ui (s)) {
        // Update x and y coordinates in shape settings
        if (x != old_x || y != old_y) {
            auto move_x = (stylus_type == STYLUS_PEN) ? s->ui ()->stylus_pen_move_x () : s->ui ()->stylus_eraser_move_x ();
            auto move_y = (stylus_type == STYLUS_PEN) ? s->ui ()->stylus_pen_move_y () : s->ui ()->stylus_eraser_move_y ();

            if (x != old_x)
                move_x->set_value (x, true);
            if (y != old_y)
                move_y->set_value (y, true);

            if (move_x->has_coupling ())
                move_x->schedule_activation ();
            if (move_y->has_coupling ())
                move_y->schedule_activation ();
        }

        // Trigger movement and pressure events for shape
        auto move_event     = (stylus_type == STYLUS_PEN) ? s->ui ()->stylus_pen_move () : s->ui ()->stylus_eraser_move ();
        auto pressure_event = (stylus_type == STYLUS_PEN) ? s->ui ()->stylus_pen_pressure () : s->ui ()->stylus_eraser_pressure ();

        move_event->schedule_activation ();
        pressure_event->set_value (pressure, true);
        pressure_event->schedule_activation ();

        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (caught_shape, RMTSF_Aggregate);

    // Handle _caught_shape if it's different from current shape `s`
    if (_caught_shape && _caught_shape != s) {
        if (x != old_x || y != old_y) {
            auto caught_x = (stylus_type == STYLUS_PEN) ? _caught_shape->ui ()->stylus_pen_move_x () : _caught_shape->ui ()->stylus_eraser_move_x ();
            auto caught_y = (stylus_type == STYLUS_PEN) ? _caught_shape->ui ()->stylus_pen_move_y () : _caught_shape->ui ()->stylus_eraser_move_y ();

            if (x != old_x)
                caught_x->set_value (x, true);
            if (y != old_y)
                caught_y->set_value (y, true);

            if (caught_x->has_coupling ())
                caught_x->schedule_activation ();
            if (caught_y->has_coupling ())
                caught_y->schedule_activation ();
        }

        // Trigger movement and pressure events for _caught_shape
        auto move_event     = (stylus_type == STYLUS_PEN) ? _caught_shape->ui ()->stylus_pen_move () : _caught_shape->ui ()->stylus_eraser_move ();
        auto pressure_event = (stylus_type == STYLUS_PEN) ? _caught_shape->ui ()->stylus_pen_pressure () : _caught_shape->ui ()->stylus_eraser_pressure ();

        move_event->schedule_activation ();
        pressure_event->set_value (pressure, true);
        pressure_event->schedule_activation ();

        exec_ = true;
    }
    rmt_EndCPUSample ();

    rmt_BeginCPUSample (window_event, RMTSF_Aggregate);

    // Schedule window events after shape events
    auto win_move   = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_move () : _win->stylus_eraser_move ();
    auto win_move_x = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_move_x () : _win->stylus_eraser_move_x ();
    auto win_move_y = (stylus_type == STYLUS_PEN) ? _win->stylus_pen_move_y () : _win->stylus_eraser_move_y ();

    if (win_move->has_coupling ()) {
        win_move->schedule_activation ();
        exec_ = true;
    }
    if (win_move_x->has_coupling ()) {
        win_move_x->schedule_activation ();
        exec_ = true;
    }
    if (win_move_y->has_coupling ()) {
        win_move_y->schedule_activation ();
        exec_ = true;
    }

    rmt_EndCPUSample ();
    rmt_EndCPUSample ();

    return exec_;
}

bool
Picking::genericMouseRelease (double x, double y, mouse_button button)
{
    _mouse_released = true;
    bool exec_      = false;

    /* windows setting */
    _win->release_x ()->set_value (x, true);
    _win->release_y ()->set_value (y, true);

    /* shape */
    PickUI* s = this->pick (x, y);
    if (s && check_for_ui (s)) {

        /* setting */
        s->ui ()->release_x ()->set_value (x, true);
        s->ui ()->release_y ()->set_value (y, true);
        s->ui ()->mouse_release_x ()->set_value (x, true);
        s->ui ()->mouse_release_y ()->set_value (y, true);
        s->set_mouse_local_coords (x, y, false);

        s->ui ()->release ()->schedule_activation ();
        s->ui ()->mouse_release ()->schedule_activation ();
        /* event if no mouse tracking */
        if (mouse_tracking == 0) {
            s->ui ()->leave ()->schedule_activation ();
            s->ui ()->mouse_leave ()->schedule_activation ();
        }

        exec_ = true;
    }

    /* reset _hovered_shape if no mouse tracking */
    if (mouse_tracking == 0) {
        _hovered_shape = nullptr;
    }

    /* reset _caught_shape */
    if (_caught_shape && _caught_shape != s) {
        FatProcess* p1 = _caught_shape->ui ()->release ();
        FatProcess* p2 = _caught_shape->ui ()->mouse_release ();
        p1->schedule_activation ();
        p2->schedule_activation ();
        exec_ = true;
    }
    _caught_shape = nullptr;

    /* button */
    switch (button) {
    case BUTTON_LEFT:
        ((GUIMouse*)GenericMouse)->left ()->release ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->left_release ()->schedule_activation ();
        }
        break;
    case BUTTON_RIGHT:
        ((GUIMouse*)GenericMouse)->right ()->release ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->right_release ()->schedule_activation ();
        }
        break;
    case BUTTON_MIDDLE:
        ((GUIMouse*)GenericMouse)->middle ()->release ()->schedule_activation ();
        if (s != nullptr && check_for_ui (s)) {
            s->ui ()->middle_release ()->schedule_activation ();
        }
        break;
    default:
        ((GUIMouse*)GenericMouse)->left ()->release ()->activate ();
    }
    if (((GUIMouse*)GenericMouse)->left ()->release ()->has_coupling () || ((GUIMouse*)GenericMouse)->right ()->release ()->has_coupling () || ((GUIMouse*)GenericMouse)->middle ()->release ()->has_coupling ()) {
        exec_ = true;
    }

    /* windows event schedule event After shape event*/
    if (_win->release ()->has_coupling ()) {
        _win->release ()->schedule_activation ();
        exec_ = true;
    }

    return exec_;
}

bool
Picking::genericTouchRelease (double x, double y, int id, float pressure)
{

    PickUI*                             s  = this->pick (x, y);
    djnnstl::map<int, Touch*>::iterator it = _active_touches.find (id);
    Touch*                              t;
    /* touch exist */
    if (it != _active_touches.end ()) {
        t = it->second;

        /* setting */
        t->set_move_x (x);
        t->set_move_y (y);
        t->set_pressure (pressure);
        t->set_id (id);

        /* shape */
        PickUI* current_shape = t->current_shape ();
        PickUI* init_shape    = t->init_shape ();
        if (init_shape != nullptr) {
            init_shape->ui ()->touches ()->remove_child (t);
            t->set_touch_local_coords (init_shape, x, y, true);
        }
        if (current_shape != nullptr) {
            current_shape->ui ()->touches ()->remove_child (t);
        }

        /* touch event */
        if (current_shape || init_shape)
            t->leave ();

        t->set_init_shape (nullptr);
        t->set_current_shape (nullptr);

        /* remove touch from list */
        _win->touches ()->remove_child (t);
        _active_touches.erase (it);

        /* delay touch delete in an GC */
        gc_touches.push_back (t);
    }

    /* common shape event */
    if (s) {
        if (!check_for_ui (s))
            return false;
        s->ui ()->release ()->schedule_activation ();
        s->ui ()->leave ()->schedule_activation ();
    }

    /* reset _caught_shape */
    if (_caught_shape && _caught_shape != s) {
        _caught_shape->ui ()->release ()->schedule_activation ();
    }
    _caught_shape = nullptr;

    /* reset _hovered_shape */
    _hovered_shape = nullptr;

    return true;
}

bool
Picking::genericTabletRelease (double x, double y, stylus_type stylus_type, mouse_button button, double pressure)
{

    _mouse_released = true;
    bool exec_      = false;

    /* Windows setting - set release coordinates based on stylus type */
    if (stylus_type == STYLUS_PEN) {
        _win->stylus_pen_release_x ()->set_value (x, true);
        _win->stylus_pen_release_y ()->set_value (y, true);
    } else if (stylus_type == STYLUS_ERASER) {
        _win->stylus_eraser_release_x ()->set_value (x, true);
        _win->stylus_eraser_release_y ()->set_value (y, true);
    }

    /* Shape handling */
    PickUI* s = this->pick (x, y);
    if (s && check_for_ui (s)) {
        // Set release coordinates and trigger release/leave events for shape
        if (stylus_type == STYLUS_PEN) {
            s->ui ()->stylus_pen_release_x ()->set_value (x, true);
            s->ui ()->stylus_pen_release_y ()->set_value (y, true);
            s->ui ()->stylus_pen_release ()->schedule_activation ();
            s->ui ()->stylus_pen_leave ()->schedule_activation ();
        } else if (stylus_type == STYLUS_ERASER) {
            s->ui ()->stylus_eraser_release_x ()->set_value (x, true);
            s->ui ()->stylus_eraser_release_y ()->set_value (y, true);
            s->ui ()->stylus_eraser_release ()->schedule_activation ();
            s->ui ()->stylus_eraser_leave ()->schedule_activation ();
        }
        exec_ = true;
    }

    /* Reset _hovered_shape if mouse tracking is disabled */
    if (mouse_tracking == 0) {
        _hovered_shape = nullptr;
    }

    /* Reset _caught_shape and trigger release event if different from current shape */
    if (_caught_shape && _caught_shape != s) {
        if (stylus_type == STYLUS_PEN) {
            _caught_shape->ui ()->stylus_pen_release ()->schedule_activation ();
        } else if (stylus_type == STYLUS_ERASER) {
            _caught_shape->ui ()->stylus_eraser_release ()->schedule_activation ();
        }
        exec_ = true;
    }
    _caught_shape = nullptr;

    /* Reset _hovered_shape */
    _hovered_shape = nullptr;

    /* Handle button-specific release events */
    if (s && check_for_ui (s)) {
        switch (button) {
        case BUTTON_LEFT:
            s->ui ()->left_release ()->schedule_activation ();
            break;
        case BUTTON_RIGHT:
            s->ui ()->right_release ()->schedule_activation ();
            break;
        case BUTTON_MIDDLE:
            s->ui ()->middle_release ()->schedule_activation ();
            break;
        default:
            break;
        }
    }

    /* Schedule windows event after shape event */
    if (stylus_type == STYLUS_PEN) {
        if (_win->stylus_pen_release ()->has_coupling ()) {
            _win->stylus_pen_release ()->schedule_activation ();
            exec_ = true;
        }
    } else if (stylus_type == STYLUS_ERASER) {
        if (_win->stylus_eraser_release ()->has_coupling ()) {
            _win->stylus_eraser_release ()->schedule_activation ();
            exec_ = true;
        }
    }

    return exec_;
}

bool
Picking::genericMouseWheel (double x, double y, double cursorx, double cursory)
{
    bool exec_ = false;

    /* window setting */
    _win->wheel_dx ()->set_value (x, true);
    _win->wheel_dy ()->set_value (y, true);
    _win->wheel_x ()->set_value (cursorx, true);
    _win->wheel_y ()->set_value (cursory, true);

    /* generic mouse setting */
    ((GUIMouse*)GenericMouse)->dx ()->set_value (x, true);
    ((GUIMouse*)GenericMouse)->dy ()->set_value (y, true);

    /* generic mouse event */
    if (((GUIMouse*)GenericMouse)->wheel ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->wheel ()->schedule_activation ();
        exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->dx ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->dx ()->schedule_activation ();
        exec_ = true;
    }
    if (((GUIMouse*)GenericMouse)->dy ()->has_coupling ()) {
        ((GUIMouse*)GenericMouse)->dy ()->schedule_activation ();
        exec_ = true;
    }

    /* windows event schedule event After shape event*/
    if (_win->wheel ()->has_coupling ()) {
        _win->wheel ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->wheel_dx ()->has_coupling ()) {
        _win->wheel_dx ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->wheel_dy ()->has_coupling ()) {
        _win->wheel_dy ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->wheel_x ()->has_coupling ()) {
        _win->wheel_x ()->schedule_activation ();
        exec_ = true;
    }
    if (_win->wheel_y ()->has_coupling ()) {
        _win->wheel_y ()->schedule_activation ();
        exec_ = true;
    }

    if (_hovered_shape != nullptr) {
        _hovered_shape->ui ()->wheel_dx ()->set_value (x, true);
        _hovered_shape->ui ()->wheel_dy ()->set_value (y, true);
        _hovered_shape->ui ()->wheel_x ()->set_value (cursorx, true);
        _hovered_shape->ui ()->wheel_y ()->set_value (cursory, true);
        if (_hovered_shape->ui ()->wheel ()->has_coupling ()) {
            _hovered_shape->ui ()->wheel ()->schedule_activation ();
            exec_ = true;
        }
        if (_hovered_shape->ui ()->wheel_dx ()->has_coupling ()) {
            _hovered_shape->ui ()->wheel_dx ()->schedule_activation ();
            exec_ = true;
        }
        if (_hovered_shape->ui ()->wheel_dy ()->has_coupling ()) {
            _hovered_shape->ui ()->wheel_dy ()->schedule_activation ();
            exec_ = true;
        }
        if (_hovered_shape->ui ()->wheel_x ()->has_coupling ()) {
            _hovered_shape->ui ()->wheel_x ()->schedule_activation ();
            exec_ = true;
        }
        if (_hovered_shape->ui ()->wheel_y ()->has_coupling ()) {
            _hovered_shape->ui ()->wheel_y ()->schedule_activation ();
            exec_ = true;
        }
    }
    return exec_;
}

} /* namespace djnn */
