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

#include "gui/gui-dev.h"
#include "gui/layer.h"
#include "abstract_gshape.h"
#include "gui/transformation/homography.h"
#include "core/control/spike.h"
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
#include "display/ui.h"

#include "core/utils/error.h"

#include "core/utils/iostream.h"
#include "utils/debug.h"

#include <math.h>
#include <float.h>


namespace djnn
{
  FatChildProcess*
  SVGHolder::find_child_impl (const string& path)
  {
    auto * p = Container::find_child_impl (path);
    if (p == nullptr && _gobj != nullptr)
      p = _gobj->find_child_impl (path);
    return p;
  }

  /*SVGHolder*
  SVGHolderimpl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    SVGHolder* newh = new SVGHolder (nullptr, "SVGHolder");

    for (auto c : _children) {
      auto * child = c->clone ();
      if (child != nullptr)
        newh->add_child (child, this->find_child_name(c));
    }

    newh->_gobj = dynamic_cast<FatProcess*>(newh->_children.back ());

    return newh;
  }*/

  SVGHolder*
  SVGHolder::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto * clone = new SVGHolder (nullptr, "SVGHolder");
    origs_clones[this] = clone;
    for (auto c : _children) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->add_child (cclone , this->find_child_name(c));
    }
    clone->_gobj = dynamic_cast<FatProcess*>(clone->_children.back ());
    return clone;
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
  
  void
  Touch::set_touch_local_coords (PickUI *p, double x, double y, bool is_move)
  {
    
    AbstractGShape *s = dynamic_cast<AbstractGShape*> (p);

    if (!s)
      return;

    /* compute local coords */
    Homography *h = dynamic_cast<Homography*> (s->inverted_matrix ());
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m13 + h->raw_props.m14
        - s->origin_x ()->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y + h->raw_props.m23 + h->raw_props.m24
        - s->origin_x ()->get_value ();
    
    /* 1 -touches */ 
    /* it's a press */
    if (!is_move) {
      this->set_local_init_x (loc_x);
      this->set_local_init_y (loc_y);
    } 
    /* we choose to set/init move even on press */
    this->set_local_move_x (loc_x);
    this->set_local_move_y (loc_y);

    /* 2 - common mouse + touch on the PickUI interface */
    if (!is_move)  {
      s->ui ()->local_press_x ()->set_value (loc_x, true);
      s->ui ()->local_press_y ()->set_value (loc_y, true);
    }

    /* we choose to set/init move even on press */
    s->ui ()->local_move_x ()->set_value (loc_x, true);
    s->ui ()->local_move_y ()->set_value (loc_y, true);
  }


  void
  AbstractGShape::init_ui ()
  {
    _ui = new UI (this, get_frame ());

    if( !_inverted_matrix) {
      _inverted_matrix = new Homography (this, "inverted_matrix");
      _inverted_matrix->set_activation_state (ACTIVATED);
    }

  }

  AbstractGShape::AbstractGShape (ParentProcess* parent, const string& name, int z) :
    AbstractGObj (parent, name), PickUI (false), _matrix (nullptr), _inverted_matrix (nullptr), _z (this, "z", z), _c_z_prop (nullptr)
  {
    _origin_x = new DoubleProperty (this, "origin_x", 0);
    _origin_y = new DoubleProperty (this, "origin_y", 0);
  }

  FatChildProcess*
  AbstractGShape::find_child_impl (const string& path)
  {
    // looking for ui interface
    if (_ui) {
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
    } else if (_c_z_prop == nullptr && (key.compare ("z") == 0)) {
      FatProcess *update = nullptr;
      if (find_layer ()) update = find_layer()->damaged ();
      else if (get_frame ()) update = get_frame ()->damaged ();
      _c_z_prop = new CouplingWithData (&_z, ACTIVATION, update, ACTIVATION);
    } else {
      /*  "press", "release", "move", "enter", "leave", "touches" */
      vector<string>::iterator it = __ui_interface.begin ();
      found = false;
      while (!found && it != __ui_interface.end ()) {
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
    if (_ui)
      _ui->deactivate ();
    // remove from picking_view if is the current object
    if (is_pickable(this)) {
      if (this->get_frame ()) {
        if(this->get_frame ()->picking_view ()) {
          this->get_frame ()->picking_view ()->object_deactivated (this);
        }
      }
    }
    if (_c_z_prop != nullptr) disable (_c_z_prop);
    AbstractGObj::impl_deactivate ();
  }

  void
  AbstractGShape::impl_activate ()
  {
   AbstractGObj::impl_activate ();
   auto _frame = get_frame ();
   if (_c_z_prop != nullptr) enable (_c_z_prop, _frame->damaged ());
   if (_ui)
     _ui->activate (get_frame());
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

    delete _c_z_prop;
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

  // pair<double, double>
  void
  AbstractGShape::set_mouse_local_coords (double x, double y, bool is_move)
  {
    /* compute local coords */
    Homography *h = dynamic_cast<Homography*> (_inverted_matrix);
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m13 + h->raw_props.m14
        - _origin_x->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y + h->raw_props.m23 + h->raw_props.m24
      - _origin_y->get_value();

    /* 1 - mouse */
    /* it's a press */
    if (!is_move) {
      this->ui()->mouse_local_press_x()->set_value(loc_x, true);
      this->ui()->mouse_local_press_y()->set_value(loc_y, true);
    }
    /* we choose to set/init move even on press */
    this->ui()->mouse_local_move_x()->set_value(loc_x, true);
    this->ui()->mouse_local_move_y()->set_value(loc_y, true);

    /* 2 - common mouse + touch */
    if (!is_move) {
      this->ui()->local_press_x()->set_value(loc_x, true);
      this->ui()->local_press_y()->set_value(loc_y, true);
    }

    /* we choose to set/init move even on press */
    this->ui()->local_move_x()->set_value(loc_x, true);
    this->ui()->local_move_y()->set_value(loc_y, true);
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
