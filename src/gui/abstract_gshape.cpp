/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "abstract_gshape.h"
#include "transformation/transformations.h"
#include "../core/tree/spike.h"
#include "../core/tree/set.h"
#include "../display/window.h"
#include "backend.h"
#include "abstract_backend.h"
#include "../display/display.h"
#include "../display/abstract_display.h"
#include "picking/picking.h"

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  vector<string> AbstractGShape::_ui =
    { "press", "release", "move", "enter", "leave", "touches", "mouse" };

  Process*
  SVGHolder::find_component (const string &path)
  {
    Process *p = Container::find_component (path);
    if (p == nullptr && _gobj != nullptr)
      p = _gobj->find_component (path);
    return p;
  }

  Process*
  SVGHolder::clone ()
  {
    SVGHolder* newh = new SVGHolder ();

    for (auto c : _children) {
      newh->add_child (c->clone (), this->find_component_name(c));
    }

    newh->_gobj = newh->_children.back ();

    return newh;
  }

  void
  Touch::init_touch (int id, double init_x, double init_y, double init_pressure)
  {
    _init_x = new DoubleProperty (this, "init_x", init_x);
    _init_y = new DoubleProperty (this, "init_y", init_y);
    _local_init_x = new DoubleProperty (this, "local_init_x", 0);
    _local_init_y = new DoubleProperty (this, "local_init_y", 0);
    _move_x = new DoubleProperty (nullptr, "move_x", init_x);
    _move_y = new DoubleProperty (nullptr, "move_y", init_y);
    _local_move_x = new DoubleProperty (nullptr, "local_move_x", 0);
    _local_move_y = new DoubleProperty (nullptr, "local_move_y", 0);
    _pressure = new DoubleProperty (this, "pressure", init_pressure);
    _id = new IntProperty (this, "id", id);
    _move = new Spike (this, "move");
    _move->add_symbol ("x", _move_x);
    _move->add_symbol ("y", _move_y);
    _move->add_symbol ("local_x", _local_move_x);
    _move->add_symbol ("local_y", _local_move_y);
    _enter = new Spike (this, "enter");
    _leave = new Spike (this, "leave");
    _release = new Spike (this, "release");
    _last_shape = new RefProperty (this, "last_shape", nullptr);
  }

  Touch::Touch (Process *p, const string &n, int id, double init_x, double init_y, double init_pressure) :
      Process (n), _shape (nullptr)
  {
    init_touch (id, init_x, init_y, init_pressure);
    set_activation_state (ACTIVATED);
    Process::finalize_construction (p);
  }

  Touch::Touch () :
      Process (), _shape (nullptr)
  {
    init_touch (0, 0, 0, 0);
    set_activation_state (ACTIVATED);
  }

  Touch::~Touch ()
  {
    delete _init_x;
    delete _init_y;
    delete _local_init_x;
    delete _local_init_y;
    _move->remove_symbol ("local_x");
    _move->remove_symbol ("local_y");
    _move->remove_symbol ("x");
    _move->remove_symbol ("y");
    delete _move_x;
    delete _move_y;
    delete _local_move_x;
    delete _local_move_y;
    delete _move;
    delete _enter;
    delete _leave;
    delete _pressure;
    delete _id;
    delete _last_shape;
  }

  UI::UI (Process *p) : parent (p)
  {
    press = new Spike (p, "press");
    move = new Spike (p, "move");
    release = new Spike (p, "release");
    enter = new Spike (p, "enter");
    leave = new Spike (p, "leave");
    mouse = new Component (p, "mouse");
    mouse_press = new Spike (mouse, "press");
    mouse_release = new Spike (mouse, "release");
    mouse_move  = new Spike (mouse, "move");
    mouse_enter = new Spike (mouse, "enter");
    mouse_leave = new Spike (mouse, "leave");
    touches = new List (p, "touches");
    touches->set_activation_state (ACTIVATED);

    move_x = new DoubleProperty (nullptr, "move_x", 0);
    move_y = new DoubleProperty (nullptr, "move_y", 0);
    press_x = new DoubleProperty (nullptr, "press_x", 0);
    press_y = new DoubleProperty (nullptr, "press_y", 0);

    local_move_x = new DoubleProperty (nullptr, "local_move_x", 0);
    local_move_y = new DoubleProperty (nullptr, "local_move_y", 0);
    local_press_x = new DoubleProperty (nullptr, "local_press_x", 0);
    local_press_y = new DoubleProperty (nullptr, "local_press_y", 0);

    move->add_symbol ("x", move_x);
    move->add_symbol ("y", move_y);
    move->add_symbol ("local_x", local_move_x);
    move->add_symbol ("local_y", local_move_y);

    press->add_symbol ("x", press_x);
    press->add_symbol ("y", press_y);
    press->add_symbol ("local_x", local_press_x);
    press->add_symbol ("local_y", local_press_y);

    mouse_press_x = new DoubleProperty (nullptr, "mouse_press_x", 0);
    mouse_press_y = new DoubleProperty (nullptr, "mouse_press_y", 0);
    mouse_move_x = new DoubleProperty (nullptr, "mouse_move_x", 0);
    mouse_move_y = new DoubleProperty (nullptr, "mouse_move_y", 0);

    mouse_local_press_x = new DoubleProperty (nullptr, "mouse_local_press_x", 0);
    mouse_local_press_y = new DoubleProperty (nullptr, "mouse_local_press_y", 0);
    mouse_local_move_x = new DoubleProperty (nullptr, "mouse_local_move_x", 0);
    mouse_local_move_y = new DoubleProperty (nullptr, "mouse_local_move_y", 0);

    mouse_move->add_symbol ("x", mouse_move_x);
    mouse_move->add_symbol ("y", mouse_move_y);
    mouse_move->add_symbol ("local_x", mouse_local_move_x);
    mouse_move->add_symbol ("local_y", mouse_local_move_y);

    mouse_press->add_symbol ("x", mouse_press_x);
    mouse_press->add_symbol ("y", mouse_press_y);
    mouse_press->add_symbol ("local_x", mouse_local_press_x);
    mouse_press->add_symbol ("local_y", mouse_local_press_y);
  }

  UI::~UI()
  {
    move->remove_symbol ("x");
    move->remove_symbol ("y");
    move->remove_symbol ("local_x");
    move->remove_symbol ("local_y");

    press->remove_symbol ("x");
    press->remove_symbol ("y");
    press->remove_symbol ("local_x");
    press->remove_symbol ("local_y");

    mouse_move->remove_symbol ("x");
    mouse_move->remove_symbol ("y");
    mouse_move->remove_symbol ("local_x");
    mouse_move->remove_symbol ("local_y");

    mouse_press->remove_symbol ("x");
    mouse_press->remove_symbol ("y");
    mouse_press->remove_symbol ("local_x");
    mouse_press->remove_symbol ("local_y");

    delete mouse_press_x;
    delete mouse_press_y;
    delete mouse_move_x;
    delete mouse_move_y;

    delete mouse_local_press_x;
    delete mouse_local_press_y;
    delete mouse_local_move_x;
    delete mouse_local_move_y;

    delete move_x;
    delete move_y;
    delete press_x;
    delete press_y;

    delete local_move_x;
    delete local_move_y;
    delete local_press_x;
    delete local_press_y;

    parent->remove_child (touches);
    parent->remove_child (mouse);
    parent->remove_child (press);
    parent->remove_child (release);
    parent->remove_child (move);
    parent->remove_child (enter);
    parent->remove_child (leave);

    mouse->remove_child (mouse_press);
    mouse->remove_child (mouse_release);
    mouse->remove_child (mouse_move);
    mouse->remove_child (mouse_enter);
    mouse->remove_child (mouse_leave);

    /* HACK 
    * clear _children of the (djnn:List) touches before
    * deleting it (touches), to avoid ~Container to delete touches inside "touches list"
    * which are also in a _active_touches list
    */
    ((List*) touches)->clear ();
    delete touches;
    
    delete press;
    delete release;
    delete move;
    delete enter;
    delete leave;
    delete mouse_press;
    delete mouse_release;
    delete mouse_move;
    delete mouse_enter;
    delete mouse_leave;
    delete mouse;
  }

  void
  AbstractGShape::init_ui ()
  {
    ui = new UI (this);

    if( !_inverted_matrix) {
      _inverted_matrix = new Homography (this, "inverted_matrix");
      _inverted_matrix->set_activation_state (ACTIVATED);
    }

  }

  AbstractGShape::AbstractGShape () :
    AbstractGObj (), _matrix (nullptr), _inverted_matrix (nullptr), ui (nullptr)
  {
    _origin_x = new DoubleProperty (this, "origin_x", 0);
    _origin_y = new DoubleProperty (this, "origin_y", 0);
  }

  AbstractGShape::AbstractGShape (Process *p, const std::string& n) :
    AbstractGObj (p, n), _matrix (nullptr), _inverted_matrix (nullptr), ui (nullptr)
  {
    _origin_x = new DoubleProperty (this, "origin_x", 0);
    _origin_y = new DoubleProperty (this, "origin_y", 0);
  }

  Process*
  AbstractGShape::find_component (const string &path)
  {
    if (ui)
      return Process::find_component (path);
    else {
      size_t found = path.find_first_of ('/');
      string key = path;
      if (found != string::npos) {
        key = path.substr (0, found);
      }
      if (key.compare ("matrix") == 0) {
        _matrix = new Homography (this, "matrix");
        _matrix->set_activation_state (ACTIVATED);
      }
      else if (key.compare ("inverted_matrix") == 0) {
        _inverted_matrix = new Homography (this, "inverted_matrix");
        _inverted_matrix->set_activation_state (ACTIVATED);
      }
      else {
        /*  "press", "release", "move", "enter", "leave", "touches" */
        vector<string>::iterator it = _ui.begin ();
        found = false;
        while (!found && it != _ui.end ()) {
          if (key.compare (*it) == 0) {
            found = true;
            init_ui ();
          }
          it++;
        }
      }
      return Process::find_component (path);
    }
  }

  void
  AbstractGShape::impl_deactivate ()
  {
    // remove from picking_view if is the current object
    if (is_pickable(this)) {
      if (this->frame ()) {
        if(this->frame ()->picking_view ()) {
          this->frame ()->picking_view ()->object_deactivated (this);
        }
      }
    }

    AbstractGObj::impl_deactivate ();
  }

  AbstractGShape::~AbstractGShape ()
  {

    // remove _origin_x and _origin_y
    delete _origin_x;
    delete _origin_y;

    // remove matrix and inverted_matrix if necessary
    delete _matrix;
    delete _inverted_matrix;

    // remove _ui if it exist
    delete ui;
  }

  void
  AbstractGShape::pick ()
  {
    //std::cerr << this << " " << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ <<  std::endl;
    auto _frame = frame ();
    if (somehow_activating () && is_pickable(this) && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->pick_gshape (this);
    }
  }

} /* namespace djnn */
