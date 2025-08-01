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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "window.h"

#include "background_color.h"
#include "background_rect.h"
#include "core/control/blank.h"
#include "core/control/spike.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/property/bool_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/ref_property.h"
#include "core/property/text_property.h"
#include "core/tree/component.h"
#include "core/tree/list.h"
#include "core/tree/set.h"
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "display/abstract_display.h"
#include "display/display-dev.h"
#include "display/update_drawing.h"

namespace djnn {
void
Window::init_ui (const string& title, double x, double y, double w, double h)
{
    _pos_x             = new DoubleProperty (this, "x", x);
    _pos_y             = new DoubleProperty (this, "y", y);
    _width             = new DoubleProperty (this, "width", w);
    _height            = new DoubleProperty (this, "height", h);
    _title             = new TextProperty (this, "title", title);
    _key_pressed       = new IntProperty (this, "key-pressed", 0);
    _key_released      = new IntProperty (this, "key-released", 0);
    _key_pressed_text  = new TextProperty (this, "key-pressed_text", "");
    _key_released_text = new TextProperty (this, "key-released_text", "");
    _hidpi_scale       = new DoubleProperty (this, "hidpi_scale", 1);
    _mspf              = new DoubleProperty (this, "mspf", 1);
    _close             = new Spike (nullptr, "close");
    add_symbol ("close", _close);
    _press                 = new Spike (nullptr, "press");
    _release               = new Spike (nullptr, "release");
    _move                  = new Spike (nullptr, "move");
    _wheel                 = new Spike (nullptr, "wheel");
    _stylus_pen            = new Spike (nullptr, "pen");
    _stylus_pen_press      = new Spike (nullptr, "press");
    _stylus_pen_release    = new Spike (nullptr, "release");
    _stylus_pen_move       = new Spike (nullptr, "move");
    _stylus_eraser         = new Spike (nullptr, "eraser");
    _stylus_eraser_press   = new Spike (nullptr, "press");
    _stylus_eraser_release = new Spike (nullptr, "release");
    _stylus_eraser_move    = new Spike (nullptr, "move");
    _touches               = new List (this, "touches");
    _touches->set_activation_state (ACTIVATED);
    _press_x   = new DoubleProperty (nullptr, "press_x", 0);
    _press_y   = new DoubleProperty (nullptr, "press_y", 0);
    _release_x = new DoubleProperty (nullptr, "release_x", 0);
    _release_y = new DoubleProperty (nullptr, "release_y", 0);
    _move_x    = new DoubleProperty (nullptr, "move_x", 0);
    _move_y    = new DoubleProperty (nullptr, "move_y", 0);
    _press->add_symbol ("x", _press_x);
    _press->add_symbol ("y", _press_y);
    _release->add_symbol ("x", _release_x);
    _release->add_symbol ("y", _release_y);
    _move->add_symbol ("x", _move_x);
    _move->add_symbol ("y", _move_y);
    _w_dx = new DoubleProperty (nullptr, "wheel_dx", 0);
    _w_dy = new DoubleProperty (nullptr, "wheel_dy", 0);
    _w_x  = new DoubleProperty (nullptr, "wheel_x", 0);
    _w_y  = new DoubleProperty (nullptr, "wheel_y", 0);
    _wheel->add_symbol ("dx", _w_dx);
    _wheel->add_symbol ("dy", _w_dy);
    _wheel->add_symbol ("x", _w_x);
    _wheel->add_symbol ("y", _w_y);
    _stylus_pen_press_x = new DoubleProperty (nullptr, "press_x", 0);
    _stylus_pen_press_y = new DoubleProperty (nullptr, "press_y", 0);
    _stylus_pen_press->add_symbol ("x", _stylus_pen_press_x);
    _stylus_pen_press->add_symbol ("y", _stylus_pen_press_y);
    _stylus_pen_move_x = new DoubleProperty (nullptr, "move_x", 0);
    _stylus_pen_move_y = new DoubleProperty (nullptr, "move_y", 0);
    _stylus_pen_move->add_symbol ("x", _stylus_pen_move_x);
    _stylus_pen_move->add_symbol ("y", _stylus_pen_move_y);
    _stylus_pen_release_x = new DoubleProperty (nullptr, "release_x", 0);
    _stylus_pen_release_y = new DoubleProperty (nullptr, "release_y", 0);
    _stylus_pen_release->add_symbol ("x", _stylus_pen_release_x);
    _stylus_pen_release->add_symbol ("y", _stylus_pen_release_y);
    _stylus_pen_pressure = new DoubleProperty (nullptr, "pressure", 0);
    _stylus_pen->add_symbol ("pressure", _stylus_pen_pressure);
    _stylus_pen->add_symbol ("press", _stylus_pen_press);
    _stylus_pen->add_symbol ("release", _stylus_pen_release);
    _stylus_pen->add_symbol ("move", _stylus_pen_move);
    _stylus_eraser_press_x = new DoubleProperty (nullptr, "press_x", 0);
    _stylus_eraser_press_y = new DoubleProperty (nullptr, "press_y", 0);
    _stylus_eraser_press->add_symbol ("x", _stylus_eraser_press_x);
    _stylus_eraser_press->add_symbol ("y", _stylus_eraser_press_y);
    _stylus_eraser_move_x = new DoubleProperty (nullptr, "move_x", 0);
    _stylus_eraser_move_y = new DoubleProperty (nullptr, "move_y", 0);
    _stylus_eraser_move->add_symbol ("x", _stylus_eraser_move_x);
    _stylus_eraser_move->add_symbol ("y", _stylus_eraser_move_y);
    _stylus_eraser_release_x = new DoubleProperty (nullptr, "move_x", 0);
    _stylus_eraser_release_y = new DoubleProperty (nullptr, "move_y", 0);
    _stylus_eraser_release->add_symbol ("x", _stylus_eraser_move_x);
    _stylus_eraser_release->add_symbol ("y", _stylus_eraser_move_y);
    _stylus_eraser_pressure = new DoubleProperty (nullptr, "pressure", 0);
    _stylus_eraser->add_symbol ("pressure", _stylus_eraser_pressure);
    _stylus_eraser->add_symbol ("press", _stylus_eraser_press);
    _stylus_eraser->add_symbol ("release", _stylus_eraser_release);
    _stylus_eraser->add_symbol ("move", _stylus_eraser_move);
    add_symbol ("press", _press);
    add_symbol ("move", _move);
    add_symbol ("release", _release);
    add_symbol ("wheel", _wheel);
    add_symbol ("pen", _stylus_pen);
    add_symbol ("eraser", _stylus_eraser);

    _damaged                          = new UndelayedSpike (this, "damaged"); // UndelayedSpike _damaged, connected to UpdateDrawing::damaged, the frame pointer is passed with the action
    FatProcess* update                = UpdateDrawing::instance ()->get_damaged ();
    _c_damaged_update_drawing_damaged = new Coupling (_damaged, ACTIVATION, update, ACTIVATION);

    _screenshot        = new Spike (this, "screenshot");
    _screenshot_path   = new TextProperty (this, "screenshot_path", "screenshot");
    _screenshot_action = new ScreenshotAction (this, "screenshot_action");
    _c_screenshot      = new Coupling (_screenshot, ACTIVATION, _screenshot_action, ACTIVATION);

    _refreshed = new Blank (this, "refreshed");
    graph_add_edge (_damaged, _refreshed);

    _opacity        = new DoubleProperty (this, "opacity", 1.0);
    _opacity_action = new OpacityAction (this, "opacity_action");
    _c_opacity      = new Coupling (_opacity, ACTIVATION, _opacity_action, ACTIVATION);

    _frameless = new BoolProperty (this, "frameless", false);

    _background_opacity        = new DoubleProperty (this, "background_opacity", 1);
    _background_opacity_action = new BackgroundOpacityAction (this, "background_opacity_action");
    _c_background_opacity      = new Coupling (_background_opacity, ACTIVATION, _background_opacity_action, ACTIVATION);

    _background_color           = new BackgroundColor (this, "background_color");
    _background_color_action    = new BackgroundColorAction (this, "background_color_action");
    _c_background_color_toValue = new Coupling (_background_color->find_child ("toValue"), ACTIVATION, _background_color_action, ACTIVATION);
    _c_background_color_toRGB   = new Coupling (_background_color->find_child ("toRGB"), ACTIVATION, _background_color_action, ACTIVATION);

    _background_rect = new BackgroundRect (this, "background_rect");

    _min_width       = new IntProperty (this, "min_width", 0);
    _min_height      = new IntProperty (this, "min_height", 0);
    _min_size_action = new MinimumSizeAction (this, "min_size_action");
    _c_min_width     = new Coupling (_min_width, ACTIVATION, _min_size_action, ACTIVATION);
    _c_min_height    = new Coupling (_min_height, ACTIVATION, _min_size_action, ACTIVATION);
    _win_impl        = DisplayBackend::instance ()->create_window (this, title, x, y, w, h);

    _geometry_action   = new GeometryAction (this, "geometry_action");
    _c_geometry_x      = new Coupling (_pos_x, ACTIVATION, _geometry_action, ACTIVATION);
    _c_geometry_y      = new Coupling (_pos_y, ACTIVATION, _geometry_action, ACTIVATION);
    _c_geometry_width  = new Coupling (_width, ACTIVATION, _geometry_action, ACTIVATION);
    _c_geometry_height = new Coupling (_height, ACTIVATION, _geometry_action, ACTIVATION);

    // impl_deactivate (); // disable couplings
    _c_damaged_update_drawing_damaged->disable ();
    _c_screenshot->disable ();
    _c_opacity->disable ();
    _c_background_opacity->disable ();
    _c_background_color_toValue->disable ();
    _c_background_color_toRGB->disable ();
}

Window::Window (CoreProcess* parent, const string& name, const string& title, double x, double y, double w, double h)
    : FatProcess (name),
      _refresh (false), _holder (nullptr)
{
    init_ui (title, x, y, w, h);
    _display = new RefProperty (this, "display", nullptr);
    finalize_construction (parent, name);
}

Window::Window (CoreProcess* parent, const string& name, const string& title)
: Window (parent, name, title, 0, 0, 600, 600)
{
}

Window::Window (CoreProcess* parent, const string& name)
: Window (parent, name, "djnn/smala app")
{
}

void
Window::impl_activate ()
{
    _background_color->activate ();
    _background_rect->activate ();
    _c_damaged_update_drawing_damaged->enable ();
    _c_screenshot->enable ();
    _c_opacity->enable ();
    _c_background_opacity->enable ();
    _c_background_color_toValue->enable ();
    _c_background_color_toRGB->enable ();
    _c_min_width->enable ();
    _c_min_height->enable ();
    _win_impl->impl_activate ();
    set_minimum_size ();
}

void
Window::impl_deactivate ()
{
    _background_color->deactivate ();
    _background_rect->deactivate ();
    _c_damaged_update_drawing_damaged->disable ();
    _c_screenshot->disable ();
    _c_opacity->disable ();
    _c_background_opacity->disable ();
    _c_background_color_toValue->disable ();
    _c_background_color_toRGB->disable ();
    _c_min_width->disable ();
    _c_min_height->disable ();
    _win_impl->impl_deactivate ();
}

Window::~Window ()
{
    UpdateDrawing::instance ()->remove_window_for_refresh (this);

    delete _c_geometry_x;
    delete _c_geometry_y;
    delete _c_geometry_width;
    delete _c_geometry_height;

    delete _geometry_action;

    delete _c_opacity;
    delete _opacity_action;
    delete _opacity;

    delete _frameless;

    delete _c_background_opacity;
    delete _background_opacity_action;
    delete _background_opacity;

    delete _c_background_color_toValue;
    delete _c_background_color_toRGB;
    delete _background_color_action;
    delete _background_color;

    delete _background_rect;

    delete _c_min_width;
    delete _c_min_height;
    delete _min_width;
    delete _min_height;
    delete _min_size_action;

    graph_remove_edge (_damaged, _refreshed);
    delete _refreshed;

    delete _c_screenshot;
    delete _screenshot_action;
    delete _screenshot_path;
    delete _screenshot;

    delete _c_damaged_update_drawing_damaged;
    delete _damaged;

    delete _pos_x;
    delete _pos_y;
    delete _width;
    delete _height;
    delete _title;
    delete _hidpi_scale;
    delete _mspf;
    delete _key_pressed;
    delete _key_released;
    delete _key_pressed_text;
    delete _key_released_text;
    delete _close;
    delete _press;
    delete _release;
    delete _move;
    delete _wheel;
    delete _touches;
    delete _press_x;
    delete _press_y;
    delete _release_x;
    delete _release_y;
    delete _move_x;
    delete _move_y;
    delete _w_dx;
    delete _w_dy;
    delete _w_x;
    delete _w_y;
    delete _display;

    delete _win_impl;
}

void
Window::UndelayedSpike::impl_activate ()
{
    // pass the frame pointer to UpdateDrawing before notifying synchronously
    UpdateDrawing::instance ()->get_damaged ()->set_data (get_parent ());
    notify_activation ();
}

// void
// Window::UndelayedSpike::coupling_activation_hook ()
// { //DBG;
//   UpdateDrawing::instance ()->get_damaged ()->set_data(get_parent());
//   notify_activation ();
// }

void
Window::perform_screenshot ()
{
    _win_impl->perform_screenshot (_screenshot_path->get_string_value ());
}

void
Window::set_opacity ()
{
    _win_impl->set_opacity (_opacity->get_value ());
}

void
Window::set_background_opacity_and_color ()
{
    _win_impl->set_background_opacity_and_color (
        _background_opacity->get_value (),
        _background_color->r ()->get_value (),
        _background_color->g ()->get_value (),
        _background_color->b ()->get_value (),
        _frameless->get_value ());
}

void
Window::set_minimum_size ()
{
    _win_impl->set_minimum_size (_min_width->get_value (), _min_height->get_value ());
}

void
Window::update_geometry ()
{
    _win_impl->update_geometry ();
}

void
Cursor::UpdateCursorAction::impl_activate ()
{
    ((Cursor*)get_parent ())->update_cursor ();
}

Cursor::Cursor (CoreProcess* parent, const string& name, const string& path, int hotX, int hotY)
    : FatProcess (name), raw_props{.hot_x = hotX, .hot_y = hotY, .path = path, .cursor_shape = 0}, _c_x (nullptr), _c_y (nullptr), _c_path (nullptr), _c_cursor_shape (nullptr), _win (nullptr)

{
    _action       = new UpdateCursorAction (this, "action");
    _shape_action = new UpdateCursorShapeAction (this, "shape_action");
    finalize_construction (parent, name);
}

Cursor::Cursor (CoreProcess* parent, const string& name, int cursor_shape)
    : FatProcess (name), raw_props{.hot_x = -1, .hot_y = -1, .path = "", .cursor_shape = cursor_shape}, _c_x (nullptr), _c_y (nullptr), _c_path (nullptr), _c_cursor_shape (nullptr), _win (nullptr)

{
    _action       = new UpdateCursorAction (this, "action");
    _shape_action = new UpdateCursorShapeAction (this, "shape_action");
    finalize_construction (parent, name);
}

CoreProcess*
Cursor::find_child_impl (const string& name)
{
    auto* res = FatProcess::find_child_impl (name);
    if (res)
        return res;
    else if (name == "hotX") {
        res  = new IntPropertyProxy (this, "hotX", raw_props.hot_x);
        _c_x = new Coupling (res, ACTIVATION, _action, ACTIVATION);
        if (!somehow_activating ())
            _c_x->disable ();
    } else if (name == "hotY") {
        res  = new IntPropertyProxy (this, "hotY", raw_props.hot_y);
        _c_y = new Coupling (res, ACTIVATION, _action, ACTIVATION);
        if (!somehow_activating ())
            _c_y->disable ();
    } else if (name == "path") {
        res     = new TextPropertyProxy (this, "path", raw_props.path);
        _c_path = new Coupling (res, ACTIVATION, _action, ACTIVATION);
        if (!somehow_activating ())
            _c_path->disable ();
    } else if (name == "cursor_shape") {
        res             = new IntPropertyProxy (this, "cursor_shape", raw_props.cursor_shape);
        _c_cursor_shape = new Coupling (res, ACTIVATION, _shape_action, ACTIVATION);
        if (!somehow_activating ())
            _c_cursor_shape->disable ();
    }
    return res;
}

void
Cursor::impl_activate ()
{
    if (_c_x)
        _c_x->enable ();
    if (_c_y)
        _c_y->enable ();
    if (_c_path)
        _c_path->enable ();
    if (_c_cursor_shape)
        _c_cursor_shape->enable ();
    if (_win == nullptr || _win->somehow_deactivating ()) {
        bool        found      = false;
        FatProcess* cur_parent = get_parent ();
        FatProcess* cur_child  = this;
        while (!found && cur_parent != nullptr) {
            if (cur_parent->get_process_type () == CONTAINER_T) {
                Container* cont = dynamic_cast<Container*> (cur_parent);
                for (auto c : cont->children ()) {
                    if (c == cur_child)
                        break;
                    else if (c->get_process_type () == WINDOW_T && c->somehow_activating ()) {
                        _win  = dynamic_cast<Window*> (c);
                        found = true;
                    }
                }
            }
            do {
                cur_child  = cur_parent;
                cur_parent = cur_parent->get_parent ();
            } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
        }
        if (!found) {
            warning (nullptr, " no running frame found for cursor " + get_name () + "\n");
            return;
        }
    }
    if (!raw_props.path.empty ())
        update_cursor ();
    else
        update_cursor_shape ();
}

void
Cursor::update_cursor ()
{
    if (_win)
        _win->set_cursor (raw_props.path, raw_props.hot_x, raw_props.hot_y);
}

void
Cursor::update_cursor_shape ()
{
    if (_win)
        _win->set_cursor (raw_props.cursor_shape);
}

void
Cursor::impl_deactivate ()
{
    if (_c_x)
        _c_x->disable ();
    if (_c_y)
        _c_y->disable ();
    if (_c_path)
        _c_path->disable ();
    if (_c_cursor_shape)
        _c_cursor_shape->disable ();
}

Cursor::~Cursor ()
{
    if (_c_x) {
        auto* x = find_child ("x");
        delete _c_x;
        delete x;
    }
    if (_c_y) {
        auto* y = find_child ("y");
        delete _c_y;
        delete y;
    }
    if (_c_path) {
        auto* path = find_child ("path");
        delete _c_path;
        delete path;
    }
    if (_c_cursor_shape) {
        auto* shape = find_child ("cursor_shape");
        delete _c_cursor_shape;
        delete shape;
    }
}

} /* namespace djnn */
