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

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"
#include "../../core/control/coupling.h"
#include "../../core/tree/blank.h"
#include "shapes.h"

using namespace std;

namespace djnn {

  PolyPoint::PolyPoint (double x, double y) :
      AbstractGObj (),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
  }

  PolyPoint::PolyPoint (Process* p, const string &n, double x, double y) :
      AbstractGObj (p,n),
      raw_props{.x=x, .y=y},
      _cx(nullptr), _cy(nullptr)
  {
    Poly *poly = dynamic_cast<Poly*> (p);
    if (poly == nullptr) {
      cerr << "Parent of polypoint must be <Polygon|Polyline>\n";
      return;
    }
    poly->points ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PolyPoint::~PolyPoint ()
  {
    delete _cx;
    delete _cy;

    if (_symtable.empty () == false) {
      std::map<std::string, Process*>::iterator it;

      it = _symtable.find ("x");
      if (it != _symtable.end ())
        delete it->second;

      it = _symtable.find ("y");
      if (it != _symtable.end ())
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
  PolyPoint::activate ()
  {
    AbstractGObj::activate ();
    if(_cx) _cx->enable (_frame);
    if(_cy) _cy->enable (_frame);
  }

  void
  PolyPoint::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
  }

  void
  PolyPoint::draw ()
  {
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_poly_point (raw_props.x, raw_props.y);
    }
  }

  Process*
  PolyPoint::clone () {
    return new PolyPoint (raw_props.x, raw_props.y);
  }

  Poly::Poly (int closed) :
      AbstractGShape (), _closed (closed)
  {
    _points = new List (this, "points");
    _bounding_box = new Blank (this, "bounding_box");
    _bbx = new DoubleProperty (0);
    _bby = new DoubleProperty (0);
    _bbw = new DoubleProperty (0);
    _bbh = new DoubleProperty (0);
    _bounding_box->add_symbol ("x", _bbx);
    _bounding_box->add_symbol ("y", _bby);
    _bounding_box->add_symbol ("width", _bbw);
    _bounding_box->add_symbol ("height", _bbh);
  }

  Poly::Poly (Process* p, const string &n, int closed) :
      AbstractGShape (p, n), _closed (closed)
  {
    _points = new List (this, "points");
    _bounding_box = new Blank (this, "bounding_box");
    _bbx = new DoubleProperty (0);
    _bby = new DoubleProperty (0);
    _bbw = new DoubleProperty (0);
    _bbh = new DoubleProperty (0);
    _bounding_box->add_symbol ("x", _bbx);
    _bounding_box->add_symbol ("y", _bby);
    _bounding_box->add_symbol ("width", _bbw);
    _bounding_box->add_symbol ("height", _bbh);
    Process::finalize ();
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
  Poly::activate ()
  {
    AbstractGObj::activate ();
    _points->activation ();
  }

  void
  Poly::deactivate ()
  {
    AbstractGObj::deactivate ();
    _points->deactivation ();
  }

  void
  Poly::draw ()
  {
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
    Poly* newp = new Poly (_closed);
    for (auto p: _points->children ()) {
      newp->_points->add_child (p->clone (), "");
    }
    return newp;
  }
}
