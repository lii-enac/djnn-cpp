/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/control/coupling.h"
#include "../../core/tree/blank.h"
#include "shapes.h"

namespace djnn
{
  PolyPoint::PolyPoint (double x, double y) :
      AbstractGObj ()
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
  }

  PolyPoint::PolyPoint (Process* p, const string &n, double x, double y) :
      AbstractGObj (p, n)
  {
    Poly *poly = dynamic_cast<Poly*> (p);
    if (poly == nullptr) {
      cerr << "Parent of polypoint must be <Polygon|Polyline>\n";
      return;
    }
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    poly->points ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PolyPoint::~PolyPoint ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
  }

  void
  PolyPoint::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
  }

  void
  PolyPoint::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
  }

  void
  PolyPoint::draw ()
  {
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_poly_point (_x->get_value (), _y->get_value ());
    }
  }

  Process*
  PolyPoint::clone () {
  
    return new PolyPoint (_x->get_value (), _y->get_value ());

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
    if (_bbh) {delete _bbh; _bbh = nullptr;}
    if (_bbw) {delete _bbw; _bbw = nullptr;}
    if (_bby) {delete _bby; _bby = nullptr;}
    if (_bbx) {delete _bbx; _bbx = nullptr;}
    if (_bounding_box) {delete _bounding_box; _bounding_box = nullptr;}
    if (_points) {delete _points; _points = nullptr;}
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
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
  Poly::clone () {
    
    Poly* newp = new Poly (_closed);

    for (auto p: _points->children ()) {
      newp->_points->add_child (p->clone (), "");
    }

    return newp;
  }
}
