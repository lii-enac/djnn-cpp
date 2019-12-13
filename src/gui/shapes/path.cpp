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

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "core/ontology/coupling.h"
#include "core/tree/blank.h"
#include "shapes.h"

#include "core/execution/graph.h"


#include <iostream>

namespace djnn
{
  PathPoint::PathPoint (Process* parent, const string &name, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      cerr << "Parent of PathPoint must be <Path>\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathPoint::~PathPoint ()
  {
    delete _cx;
    delete _cy;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  PathPoint::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
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
    auto _frame = frame ();
    if(_cx) _cx->enable (_frame);
    if(_cy) _cy->enable (_frame);
  }

  void
  PathPoint::impl_deactivate ()
  {
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
    AbstractGObj::impl_deactivate ();
  }

  void
  PathLine::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_line (raw_props.x, raw_props.y);
    }
  }

  Process*
  PathLine::clone ()
  {
    return new PathLine (nullptr, get_name (), raw_props.x, raw_props.y);
  }

  void
  PathMove::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_move (raw_props.x, raw_props.y);
    }
  }

  Process*
  PathMove::clone ()
  {
    return new PathMove (nullptr, get_name (), raw_props.x, raw_props.y);
  }

  PathClosure::PathClosure (Process* parent, const string &name) :
    AbstractGObj (parent, name)
  {
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      cerr << "Parent of path closure must be Path\n";
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

  Process*
  PathClosure::clone ()
  {
    return new PathClosure (nullptr, get_name ());
  }

  PathQuadratic::PathQuadratic (Process* parent, const string &name, double x1, double y1, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{ .x1=x1, .y1=y1, .x=x, .y=y },
      _cx1 (nullptr), _cy1 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      cerr << "Parent of path quadratic must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathQuadratic::~PathQuadratic ()
  {
    delete _cx1;
    delete _cy1;
    delete _cx;
    delete _cy;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("x");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  PathQuadratic::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
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
    auto _frame = frame ();
    if (_cx1) _cx1->enable (_frame);
    if (_cy1) _cy1->enable (_frame);
    if (_cx)  _cx->enable (_frame);
    if (_cy)  _cy->enable (_frame);
  }

  void
  PathQuadratic::impl_deactivate ()
  {
    if(_cx1) _cx1->disable ();
    if(_cy1) _cy1->disable ();
    if(_cx)  _cx->disable ();
    if(_cy)  _cy->disable ();
    AbstractGObj::impl_deactivate ();
  }

  void
  PathQuadratic::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_quadratic (raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
    }
  }

  Process*
  PathQuadratic::clone ()
  {
    return new PathQuadratic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x, raw_props.y);
  }


  PathCubic::PathCubic (Process* parent, const string &name, double x1, double y1, double x2, double y2, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2, .x=x, .y=y},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr), _cx (nullptr), _cy (nullptr)
  {
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      cerr << "Parent of path cubic must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathCubic::~PathCubic ()
  {
    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;
    delete _cx;
    delete _cy;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("x1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y1");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("x2");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y2");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("x");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  PathCubic::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
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
    auto _frame = frame ();
    if (_cx1) _cx1->enable (_frame);
    if (_cy1) _cy1->enable (_frame);
    if (_cx2) _cx2->enable (_frame);
    if (_cy2) _cy2->enable (_frame);
    if (_cx)  _cx ->enable (_frame);
    if (_cy)  _cy ->enable (_frame);
  }

  void
  PathCubic::impl_deactivate ()
  {
    if(_cx1) _cx1->disable ();
    if(_cy1) _cy1->disable ();
    if(_cx2) _cx2->disable ();
    if(_cy2) _cy2->disable ();
    if(_cx)  _cx ->disable ();
    if(_cy)  _cy ->disable ();
    AbstractGObj::impl_deactivate ();
  }

  void
  PathCubic::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_cubic (raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
    }
  }

  Process*
  PathCubic::clone ()
  {
    return new PathCubic (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2, raw_props.x, raw_props.y);
  }

  PathArc::PathArc (Process* parent, const string &name, double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y) :
      AbstractGObj (parent, name),
      raw_props{.rx=rx, .ry=ry, .rotx=rotx, .fl=fl, .swfl=swfl, .x=x, .y=y},
      _crx (nullptr), _cry (nullptr), _crotx (nullptr), _cfl (nullptr), _cswfl(nullptr), _cx (nullptr), _cy (nullptr)
  {
    Path *path = dynamic_cast<Path*> (parent);
    if (path == nullptr) {
      cerr << "Parent of path arc must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PathArc::~PathArc ()
  {
    delete _crotx;
    delete _cfl;
    delete _cswfl;
    delete _crx;
    delete _cry;
    delete _cx;
    delete _cy;

    if (symtable ().empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = symtable ().find ("rotx");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("fl");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("swfl");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("rx");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("ry");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("x");
      if (it != symtable ().end ())
        delete it->second;

      it = symtable ().find ("y");
      if (it != symtable ().end ())
        delete it->second;
    }
  }

  Process*
  PathArc::find_component (const string& name)
  {
    Process* res = AbstractGObj::find_component(name);
    if(res) return res;

    Coupling ** coupling;
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
    auto _frame = frame ();
    if(_crotx) _crotx->enable (_frame);
    if(_cfl) _cfl->enable (_frame);
    if(_cswfl) _cswfl->enable (_frame);
    if(_crx) _crx->enable (_frame);
    if(_cry) _cry->enable (_frame);
    if(_cx) _cx->enable (_frame);
    if(_cx) _cy->enable (_frame);
  }

  void
  PathArc::impl_deactivate ()
  {
    if(_crotx) _crotx->disable ();
    if(_cfl) _cfl->disable ();
    if(_cswfl) _cswfl->disable ();
    if(_crx) _crx->disable ();
    if(_cry) _cry->disable ();
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
    AbstractGObj::impl_deactivate ();
  }

  void
  PathArc::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_arc (raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
    }
  }

  Process*
  PathArc::clone ()
  {
    return new PathArc (nullptr, get_name (), raw_props.rx, raw_props.ry, raw_props.rotx, raw_props.fl, raw_props.swfl, raw_props.x, raw_props.y);
  }

  Path::Path (Process* parent, const string &name) :
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
    Process::finalize_construction (parent, name);
  }

  Path::~Path ()
  {
    delete _items;

    delete _bbh;
    delete _bbw;
    delete _bby;
    delete _bbx;
    delete _bounding_box;
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
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path (this);
    }
  }

  Process*
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
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_clip (this);
    }
  }

  Process*
  PathClip::clone ()
  {
    PathClip* clone = new PathClip (nullptr, get_name ());
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
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
