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
 *      Stéphane Convercy <stephane.convercy@enac.fr>
 *
 */

#include "display/display-dev.h" // DisplayBackend::instance
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "gui/shape/sdf.h"

#include "core/tree/blank.h"
#include "core/tree/list.h"
#include "display/window.h"
#include "gui/shape/path.h"
#include "gui/shape/path_clip.h"

#include <iostream>
#include "utils/debug.h" // UNIMPL


namespace djnn
{
  PathPoint::PathPoint (ParentProcess* parent, const std::string& name, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      std::cerr << "Parent of PathPoint must be <Path>\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathPoint::~PathPoint ()
  {
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  FatChildProcess*
  PathPoint::find_child_impl (const std::string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  PathPoint::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathPoint::impl_deactivate ()
  {
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathLine::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_line (raw_props.x, raw_props.y);
    }
  }

  PathLine*
  PathLine::clone ()
  {
    return new PathLine (nullptr, get_name (), raw_props.x, raw_props.y);
  }

  void
  PathMove::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_move (raw_props.x, raw_props.y);
    }
  }

  PathMove*
  PathMove::clone ()
  {
    return new PathMove (nullptr, get_name (), raw_props.x, raw_props.y);
  }

  PathClosure::PathClosure (ParentProcess* parent, const std::string& name) :
    AbstractGObj (parent, name)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      std::cerr << "Parent of path closure must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  void
  PathClosure::draw ()
  {
    Backend::instance ()->draw_path_closure ();
  }

  PathClosure*
  PathClosure::clone ()
  {
    return new PathClosure (nullptr, get_name ());
  }

  PathQuadratic::PathQuadratic (ParentProcess* parent, const std::string& name, double x1, double y1, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{ .x1=x1, .y1=y1, .x=x, .y=y },
      _cx1 (nullptr), _cy1 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      std::cerr << "Parent of path quadratic must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathQuadratic::~PathQuadratic ()
  {
    remove_edge (_cx1);
    remove_edge (_cy1);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx1;
    delete _cy1;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  FatChildProcess*
  PathQuadratic::find_child_impl (const std::string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  PathQuadratic::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx1, damaged);
    enable (_cy1, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathQuadratic::impl_deactivate ()
  {
    disable (_cx1);
    disable (_cy1);
    disable (_cx);
    disable (_cy);

    AbstractGObj::impl_deactivate ();
  }

  void
  PathQuadratic::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_quadratic (raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
    }
  }

  PathQuadratic*
  PathQuadratic::clone ()
  {
    return new PathQuadratic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
  }


  PathCubic::PathCubic (ParentProcess* parent, const std::string& name, double x1, double y1, double x2, double y2, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2, .x=x, .y=y},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      std::cerr << "Parent of path cubic must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathCubic::~PathCubic ()
  {
    remove_edge (_cx1);
    remove_edge (_cy1);
    remove_edge (_cx2);
    remove_edge (_cy2);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y1");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x2");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y2");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  FatChildProcess*
  PathCubic::find_child_impl (const std::string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp=&raw_props.x2;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp=&raw_props.y2;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);    

    return res;
  }

  void
  PathCubic::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx1, damaged);
    enable (_cy1, damaged);
    enable (_cx2, damaged);
    enable (_cy2, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathCubic::impl_deactivate ()
  {
    disable (_cx1);
    disable (_cy1);
    disable (_cx2);
    disable (_cy2);
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathCubic::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_cubic (raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
    }
  }

  PathCubic*
  PathCubic::clone ()
  {
    return new PathCubic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
  }

  PathArc::PathArc (ParentProcess* parent, const std::string& name, double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y) :
      AbstractGObj (parent, name),
      raw_props{.rx=rx, .ry=ry, .rotx=rotx, .fl=fl, .swfl=swfl, .x=x, .y=y},
      _crx (nullptr), _cry (nullptr), _crotx (nullptr), _cfl (nullptr), _cswfl(nullptr), _cx (nullptr), _cy (nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      std::cerr << "Parent of path arc must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathArc::~PathArc ()
  {
    remove_edge (_crotx);
    remove_edge (_cfl);
    remove_edge (_cswfl);
    remove_edge (_crx);
    remove_edge (_cry);
    remove_edge (_cx);
    remove_edge (_cy);

    delete _crotx;
    delete _cfl;
    delete _cswfl;
    delete _crx;
    delete _cry;
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("rotx");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("fl");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("swfl");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("rx");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("ry");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;
    }
  }

  FatChildProcess*
  PathArc::find_child_impl (const std::string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="rotx") {
      coupling=&_crotx;
      rawp=&raw_props.rotx;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="fl") {
      coupling=&_cfl;
      rawp=&raw_props.fl;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="swfl") {
      coupling=&_cswfl;
      rawp=&raw_props.swfl;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="rx") {
      coupling=&_crx;
      rawp=&raw_props.rx;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="ry") {
      coupling=&_cry;
      rawp=&raw_props.ry;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;

    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);    

    return res;
  }

  void
  PathArc::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    //auto _frame = get_frame ();
    auto * damaged = get_frame ()->damaged ();
    enable (_crotx, damaged);
    enable (_cfl, damaged);
    enable (_cswfl, damaged);
    enable (_crx, damaged);
    enable (_cry, damaged);
    enable (_cx, damaged);
    enable (_cy, damaged);
  }

  void
  PathArc::impl_deactivate ()
  {
    disable (_crotx);
    disable (_cfl);
    disable (_cswfl);
    disable (_crx);
    disable (_cry);
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
  }

  void
  PathArc::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_arc (raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
    }
  }

  PathArc*
  PathArc::clone ()
  {
    return new PathArc (nullptr, get_name (), raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
  }

  Path::Path (ParentProcess* parent, const std::string& name) :
      AbstractGShape (parent, name)
  {
    _items = new List (this, "items");
    _bounding_box = new Blank (this, "bounding_box");
    _bbx = new DoubleProperty (nullptr, "bbx", 0);
    _bby = new DoubleProperty (nullptr, "bby", 0);
    _bbw = new DoubleProperty (nullptr, "bbw", 0);
    _bbh = new DoubleProperty (nullptr, "bbh", 0); 
    _bounding_box->add_symbol ("x", _bbx);
    _bounding_box->add_symbol ("y", _bby);
    _bounding_box->add_symbol ("width", _bbw);
    _bounding_box->add_symbol ("height", _bbh);
    invalidate_bounding_box ();
    finalize_construction (parent, name);
  }

  Path::~Path ()
  {
    delete _bounding_box;

    delete _bbx;
    delete _bby;
    delete _bbw;
    delete _bbh;
    delete _items;
  }

  void
  Path::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    _items->activate ();
  }

  void
  Path::impl_deactivate ()
  {
    _items->deactivate ();
    AbstractGShape::impl_deactivate ();
  }

  void
  Path::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_path (this);
      AbstractGShape::post_draw ();
    }
  }

  Path*
  Path::clone ()
  {
    Path* clone = new Path (nullptr, get_name ());
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
  }

  void
  PathClip::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_clip (this);
    }
  }

  PathClip*
  PathClip::clone ()
  {
    PathClip* clone = new PathClip (nullptr, get_name ());
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
  }

  void
  Path::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    // warning : does not work until the path is drawn
    // also, if a point in the path is modified, bbox is not recomputed until the path is drawn again
    if (is_bounding_box_valid ()) {
      x = _bbx->get_value ();
      y = _bby->get_value ();
      w = _bbw->get_value ();
      h = _bbh->get_value ();
    } else {
      UNIMPL;
      // should be computed for picking...
      //for (auto p: _items->children ()) { 
    }
  }

  double
  Path::sdf (double x, double y) const
  {
    UNIMPL;
    return 0;
  }

  void
  Path::set_bounding_box (double x, double y, double w, double h)
  {
    _bbx->set_value (x, true);
    _bby->set_value (y, true);
    _bbw->set_value (w, true);
    _bbh->set_value (h, true);
  }
}
