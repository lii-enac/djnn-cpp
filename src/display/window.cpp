/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
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

#include "display/display.h"
#include "display/abstract_display.h"
#include "display/update_drawing.h"

#include "core/tree/spike.h"
#include "core/tree/component.h"
#include "core/tree/set.h"
#include "core/tree/list.h"
#include "core/utils/error.h"
#include "core/execution/graph.h"

#include <iostream>

namespace djnn
{
  void
  Window::init_ui (const std::string& title, double x, double y, double w, double h)
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
    _hidpi_scale = new DoubleProperty (this, "hidpi_scale", 1);
    _mspf = new DoubleProperty (this, "mspf", 1);
    _close = new Spike (nullptr, "close");
    add_symbol ("close", _close);
    _press = new Spike (nullptr, "press");
    _release = new Spike (nullptr, "release");
    _move = new Spike (nullptr, "move");
    _wheel = new Spike (nullptr, "wheel");
    _touches = new List (this, "touches");
    _touches->set_activation_state (ACTIVATED);
    _press_x = new DoubleProperty (nullptr, "press_x", 0);
    _press_y = new DoubleProperty (nullptr, "press_y", 0);
    _move_x = new DoubleProperty (nullptr, "move_x", 0);
    _move_y = new DoubleProperty (nullptr, "move_y", 0);
    _press->add_symbol ("x", _press_x);
    _press->add_symbol ("y", _press_y);
    _move->add_symbol ("x", _move_x);
    _move->add_symbol ("y", _move_y);
    _w_dx = new DoubleProperty (nullptr, "wheel_dx", 0);
    _w_dy = new DoubleProperty (nullptr, "wheel_dy", 0);
    _wheel->add_symbol ("dx", _w_dx);
    _wheel->add_symbol ("dy", _w_dy);
    add_symbol ("press", _press);
    add_symbol ("move", _move);
    add_symbol ("release", _release);
    add_symbol ("wheel", _wheel);

    _damaged = new UndelayedSpike (this, "damaged"); // UndelayedSpike _damaged, connected to UpdateDrawing::damaged, the frame pointer is passed with the action
    FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    _c_damaged_update_drawing_damaged = new Coupling (_damaged, ACTIVATION, update, ACTIVATION, true);
    Graph::instance ().add_edge (_damaged, update);

    _screenshot = new Spike (this, "screenshot");
    _screenshot_path = new TextProperty (this, "screenshot_path", "screenshot");
    _screenshot_action = new ScreenshotAction (this, "screenshot_action");
    _c_screenshot = new Coupling (_screenshot, ACTIVATION, _screenshot_action, ACTIVATION);
    Graph::instance ().add_edge (_screenshot, _screenshot_action);

    _refreshed = new Blank (this, "refreshed");

    _opacity = new DoubleProperty (this, "opacity", 1.0);
    _opacity_action = new OpacityAction (this, "opacity_action");
    _c_opacity = new Coupling (_opacity, ACTIVATION, _opacity_action, ACTIVATION);
    Graph::instance ().add_edge (_opacity, _opacity_action);

    _win_impl = DisplayBackend::instance ()->create_window (this, title, x, y, w, h);
  }

  Window::Window (ParentProcess* parent, const std::string& name, const std::string& title, double x, double y, double w,
		  double h) :
      FatProcess (name),
      _refresh (false), _holder (nullptr)
  {
    init_ui (title, x, y, w, h);
    _display = new RefProperty (this, "display", nullptr);
    finalize_construction (parent, name);
  }

  void
  Window::impl_activate ()
  {
    _c_damaged_update_drawing_damaged->enable();
    _c_screenshot->enable();
    _c_opacity->enable ();
    _win_impl->impl_activate ();
  }
  
  void
  Window::impl_deactivate ()
  {
    _c_damaged_update_drawing_damaged->disable();
    _c_screenshot->disable();
    _c_opacity->disable ();
    _win_impl->impl_deactivate ();
  }

  Window::~Window ()
  {
    UpdateDrawing::instance ()->remove_window_for_refresh(this);
    Graph::instance ().remove_edge (_damaged, UpdateDrawing::instance ()->get_damaged ());
    delete _c_damaged_update_drawing_damaged;
    delete _damaged;

    Graph::instance ().remove_edge (_opacity, _opacity_action);
    delete _c_opacity;
    delete _opacity_action;
    delete _opacity;

    delete _refreshed;

    Graph::instance ().remove_edge (_screenshot, _screenshot_action);
    delete _c_screenshot;
    delete _screenshot_action;
    delete _screenshot_path;
    delete _screenshot;

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
    delete _move_x;
    delete _move_y;
    delete _w_dx;
    delete _w_dy;
    delete _display;

    delete _win_impl;
  }

  void 
  Window::UndelayedSpike::impl_activate ()
  {
    // pass the frame pointer to UpdateDrawing before notifying synchronously 
    UpdateDrawing::instance ()->get_damaged ()->set_data (get_parent());
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
    _win_impl->perform_screenshot(_screenshot_path->get_string_value());
  }

  void
  Window::set_opacity ()
  {
    _win_impl->set_opacity (_opacity->get_value());
  }

  void
  Cursor::UpdateCursorAction::impl_activate () {
    ((Cursor*) get_parent ())->update_cursor ();
  }

  Cursor::Cursor (ParentProcess* parent, const std::string& name, const std::string& path, int hotX, int hotY) :
      FatProcess (name), raw_props
        { .hot_x = hotX, .hot_y = hotY, .path = path }, _c_x (nullptr), _c_y (nullptr), _c_path (nullptr), _win (nullptr)

  {
    _action = new UpdateCursorAction (this, "action");
    finalize_construction (parent, name);
  }

  FatChildProcess*
  Cursor::find_child_impl (const std::string& name)
  {
    auto * res = FatProcess::find_child (name);
    if (res)
      return res;
    else if (name == "hotX") {
      res = new IntPropertyProxy (this, "hotX", raw_props.hot_x);
      _c_x = new Coupling (res, ACTIVATION, _action, ACTIVATION);
      if (!somehow_activating())
        _c_x->disable ();
    } else if (name == "hotY") {
      res = new IntPropertyProxy (this, "hotY", raw_props.hot_y);
      _c_y = new Coupling (res, ACTIVATION, _action, ACTIVATION);
      if (!somehow_activating())
        _c_y->disable ();
    } else if (name == "path") {
      res = new TextPropertyProxy (this, "path", raw_props.path);
      _c_path = new Coupling (res, ACTIVATION, _action, ACTIVATION);
      if (!somehow_activating())
        _c_path->disable ();
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
    if (_win == nullptr || _win->somehow_deactivating ()) {
      bool found = false;
      FatProcess *cur_parent = get_parent ();
      FatProcess *cur_child = this;
      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_process_type () == CONTAINER_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_process_type () == WINDOW_T && c->somehow_activating ()) {
              _win = dynamic_cast<Window*> (c);
              found = true;
            }
          }
        }
        do {
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
      }
      if (!found) {
        warning ( nullptr, " no running frame found for cursor " + get_name () + "\n" );
        return;
      }
    }
    update_cursor ();
  }

  void
  Cursor::update_cursor ()
  {
    if (_win)
      _win->set_cursor (raw_props.path, raw_props.hot_x, raw_props.hot_y);
  }

  void
  Cursor::impl_deactivate () {
    if (_c_x)
      _c_x->disable ();
    if (_c_y)
      _c_y->disable ();
    if (_c_path)
      _c_path->disable ();
  }

  Cursor::~Cursor ()
  {
    if (_c_x) {
      auto * x = find_child ("x");
      delete _c_x;
      delete x;
    }
    if (_c_y) {
      auto * y = find_child ("y");
      delete _c_y;
      delete y;
    }
    if (_c_path) {
      auto * path = find_child ("path");
      delete _c_path;
      delete path;
    }
  }

} /* namespace djnn */
