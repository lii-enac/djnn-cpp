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
 *      Stephane Conversy <stephane.conversy@enac.fr>
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

using namespace std;

namespace djnn {

  PolyPoint::PolyPoint (Process* parent, const string &name, double x, double y) :
      AbstractGObj (parent, name),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr) return ;

    /* if not cloning we test parent with dynamic_cast */
    Poly *poly = dynamic_cast<Poly*> (parent);
    if (poly == nullptr) {
      cerr << "Parent of polypoint must be <Polygon|Polyline>\n";
      return;
    }
    poly->points ()->add_child (this, "");
    parent->add_symbol(name, this);
  }

  PolyPoint::~PolyPoint ()
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
  PolyPoint::find_component (const string& name)
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
  PolyPoint::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    auto _frame = frame ();
    if(_cx) _cx->enable (_frame);
    if(_cy) _cy->enable (_frame);
  }

  void
  PolyPoint::impl_deactivate ()
  {
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
    AbstractGObj::impl_deactivate ();
  }

  void
  PolyPoint::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_poly_point (raw_props.x, raw_props.y);
    }
  }

  Process*
  PolyPoint::clone () {
    return new PolyPoint (nullptr, get_name (), raw_props.x, raw_props.y);
  }

  Poly::Poly (Process* parent, const string &name, int closed) :
      AbstractGShape (parent, name), _closed (closed)
  {
    _points = new List (this, "points");
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

  Poly::~Poly ()
  {
    delete _bbh;
    delete _bbw;
    delete _bby;
    delete _bbx;
    delete _bounding_box;
    delete _points;
  }

  void
  Poly::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    _points->activate ();
  }

  void
  Poly::impl_deactivate ()
  {
    _points->deactivate ();
    AbstractGShape::impl_deactivate ();
  }

  void
  Poly::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_poly (this);
    }
  }

  void
  Poly::set_bounding_box (double x, double y, double w, double h)
  {
    _bbx->set_value (x, true);
    _bby->set_value (y, true);
    _bbw->set_value (w, true);
    _bbh->set_value (h, true);
  }

  Process*
  Poly::clone ()
  {  
    Poly* newp = new Poly (nullptr, get_name (), _closed);
    for (auto p: _points->children ()) {
      newp->_points->add_child (p->clone (), "");
    }
    return newp;
  }
}
