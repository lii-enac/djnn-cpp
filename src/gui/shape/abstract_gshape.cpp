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
#include "gui/transformation/homography.h"
#include "core/tree/spike.h"
#include "core/tree/set.h"
#include "core/tree/list.h"
#include "core/tree/component_observer.h"
#include "display/window.h"
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/abstract_display.h"
#include "display/update_drawing.h"
#include "gui/picking/picking.h"
#include "gui/picking/analytical_picking_context.h"
#include "gui/style/style_sheet.h"
#include "gui/shape/svg_holder.h"
#include "gui/shape/ui.h"

#include "core/utils/error.h"

#include "core/utils/iostream.h"
#include "utils/debug.h"

#include <math.h>
#include <float.h>


namespace djnn
{
  static
  vector<string>
  //AbstractGShape::
  _ui =
    { "pickable", "press", "release", "left", "right", "middle", "move", "enter", "leave", "touches", "mouse" };

  FatChildProcess*
  SVGHolder::find_child_impl (const string& path)
  {
    auto * p = Container::find_child_impl (path);
    if (p == nullptr && _gobj != nullptr)
      p = _gobj->find_child_impl (path);
    return p;
  }

  SVGHolder*
  SVGHolder::clone ()
  {
    SVGHolder* newh = new SVGHolder (nullptr, "SVGHolder");

    for (auto c : _children) {
      auto * child = c->clone ();
      if (child != nullptr)
        newh->add_child (child, this->find_child_name(c));
    }

    newh->_gobj = dynamic_cast<FatProcess*>(newh->_children.back ());

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

  Touch::Touch (ParentProcess* parent, const string& name, int id, double init_x, double init_y, double init_pressure) :
      FatProcess (name), _shape (nullptr)
  {
    init_touch (id, init_x, init_y, init_pressure);
    set_activation_state (ACTIVATED);
    finalize_construction (parent, name);
  }

  Touch::~Touch ()
  {
    delete _last_shape;
    delete _release;
    delete _leave;
    delete _enter;
    _move->remove_symbol ("local_x");
    _move->remove_symbol ("local_y");
    _move->remove_symbol ("x");
    _move->remove_symbol ("y");
    delete _move;
    delete _id;
    delete _pressure;
    delete _local_move_y;
    delete _local_move_x;
    delete _move_y;
    delete _move_x;
    delete _local_init_y;
    delete _local_init_x;
    delete _init_y;
    delete _init_x;
  }

  UI::UI (ParentProcess* parent_, FatProcess *f) : parent (parent_)
  {
    pickable = new BoolProperty (parent, "pickable", true);
    //FatProcess *update = UpdateDrawing::instance ()->get_damaged ();
    //cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION, nullptr);
    //if (f != nullptr)
    //  cpick->enable (f);
    FatProcess *update = f;
    if (update) update = dynamic_cast<Window*>(f)->damaged ();
    cpick = new CouplingWithData (pickable, ACTIVATION, update, ACTIVATION);
    if (f != nullptr) cpick->enable ();

    press = new Spike (parent, "press");
    move = new Spike (parent, "move");
    left = new Component (parent, "left");
    left_press = new Spike (left, "press");
    left_release = new Spike (left, "release");
    right = new Component (parent, "right");
    right_press = new Spike (right, "press");
    right_release = new Spike (right, "release");
    middle = new Component (parent, "middle");
    middle_press = new Spike (middle, "press");
    middle_release = new Spike (middle, "release");
    release = new Spike (parent, "release");
    enter = new Spike (parent, "enter");
    leave = new Spike (parent, "leave");
    mouse = new Component (parent, "mouse");
    mouse_press = new Spike (mouse, "press");
    mouse_release = new Spike (mouse, "release");
    mouse_move  = new Spike (mouse, "move");
    mouse_enter = new Spike (mouse, "enter");
    mouse_leave = new Spike (mouse, "leave");

    touches = new List (parent, "touches");
    touches->set_activation_state (ACTIVATED);
    mouse->set_activation_state(ACTIVATED);
    left->set_activation_state (ACTIVATED);
    right->set_activation_state (ACTIVATED);
    middle->set_activation_state (ACTIVATED);

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
    parent->remove_child (left);
    parent->remove_child (right);
    parent->remove_child (middle);

    mouse->remove_child (mouse_press);
    mouse->remove_child (mouse_release);
    mouse->remove_child (mouse_move);
    mouse->remove_child (mouse_enter);
    mouse->remove_child (mouse_leave);

    left->remove_child (left_press);
    left->remove_child (left_release);

    right->remove_child (right_press);
    right->remove_child (right_release);

    middle->remove_child (middle_press);
    middle->remove_child (middle_release);

    /* HACK 
    * clear _children of the (djnn:List) touches before
    * deleting it (touches), to avoid ~Container to delete touches inside "touches list"
    * which are also in a _active_touches list
    */
    ((List*) touches)->clear ();
    delete touches;
    
    delete mouse_leave;
    delete mouse_enter;
    delete mouse_move;
    delete mouse_release;
    delete mouse_press;
    delete mouse;
    delete leave;
    delete enter;
    delete release;
    delete move;
    delete press;
    delete left;
    delete left_press;
    delete left_release;
    delete right;
    delete right_press;
    delete right_release;
    delete middle;
    delete middle_press;
    delete middle_release;
    delete cpick;
    delete pickable;
  }

  void
  AbstractGShape::init_ui ()
  {
    ui = new UI (this, get_frame ());

    if( !_inverted_matrix) {
      _inverted_matrix = new Homography (this, "inverted_matrix");
      _inverted_matrix->set_activation_state (ACTIVATED);
    }

  }

  AbstractGShape::AbstractGShape (ParentProcess* parent, const string& name) :
    AbstractGObj (parent, name), _matrix (nullptr), _inverted_matrix (nullptr), ui (nullptr)
  {
    _origin_x = new DoubleProperty (this, "origin_x", 0);
    _origin_y = new DoubleProperty (this, "origin_y", 0);
  }

  FatChildProcess*
  AbstractGShape::find_child_impl (const string& path)
  {
    // looking for ui interface
    if (ui) {
      FatChildProcess* process = FatProcess::find_child_impl (path);
      if (process != nullptr)
        return process;
    }

    // looking for something else
    size_t found = path.find_first_of ('/');
    string key = path;
    if (found != string::npos) {
      key = path.substr (0, found);
    }

    if ((_matrix == nullptr) && (key.compare ("matrix") == 0)) {
      _matrix = new Homography (this, "matrix");
      _matrix->set_activation_state (ACTIVATED);
    }
    else if ((_inverted_matrix == nullptr) && (key.compare ("inverted_matrix") == 0)) {
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
    return FatProcess::find_child_impl (path);
  }

  void
  AbstractGShape::impl_deactivate ()
  {
    UI *ui = get_ui();
    if (ui)
      ui->deactivate ();
    // remove from picking_view if is the current object
    if (is_pickable(this)) {
      if (this->get_frame ()) {
        if(this->get_frame ()->picking_view ()) {
          this->get_frame ()->picking_view ()->object_deactivated (this);
        }
      }
    }

    AbstractGObj::impl_deactivate ();
  }

  void
  AbstractGShape::impl_activate ()
  {
   AbstractGObj::impl_activate ();
   UI *ui = get_ui();
   if (ui)
     ui->activate (get_frame());
  }

  void
  AbstractGShape::add_style_class (const string& classname)
  {
    int id = StyleSheet::get_id(classname);
    if (id != -1)
      _classes.push_back(id);
    else {
      warning (this, "Style " + classname + " not found");
    }
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
    auto _frame = get_frame ();
    if (somehow_activating () && is_pickable(this) && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->pick_gshape (this);
    }
  }

  void
  AbstractGShape::pre_draw ()
  {
    if (_classes.empty())
      return;
    ComponentObserver::instance ().start_draw ();

    StyleSheet::draw_style (_classes);
  }
  void
  AbstractGShape::post_draw ()
  {
    if (_classes.empty())
      return;
    ComponentObserver::instance ().end_draw ();
  }

  void
  AbstractGShape::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = -1;
    y = -1;
    w = -1;
    h = -1;
  }

  double
  AbstractGShape::sdf (double x, double y) const
  {
    return DBL_MAX;
  }

  AbstractGShape*
  AbstractGShape::pick_analytical (PickAnalyticalContext& pac)
  {
    if ( !is_pickable (this) ) return nullptr;

    // fast culling with bounding box
    double x,y,w,h;
    get_bounding_box (x,y,w,h);
    if( !( (x - pac.half_outline_width) <= pac.x &&
            pac.x <= (x + w + pac.half_outline_width*2) &&
           (y - pac.half_outline_width) <= pac.y &&
            pac.y <= (y + h + pac.half_outline_width*2)))
          return nullptr;

    // distance function
    double d = sdf (pac.x, pac.y);
    //std::cerr << this << " x:" << pac.x << " y:" << pac.y << " d:" << d << __FL__;

    if(pac.filled) {
      if (d<1.0)
        return this;
    }
    if(pac.outlined) {
      float t = pac.half_outline_width;
      d = abs(d) - t;
      if (d<1.0)
        return this;
    } 
    return nullptr;
  }

} /* namespace djnn */
