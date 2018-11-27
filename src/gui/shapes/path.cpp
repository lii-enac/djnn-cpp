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
  PathPoint::PathPoint (Process* p, const string &n, double x, double y) :
      AbstractGObj (p, n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path point must be Path\n";
      return;
    }
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    path->items ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PathPoint::PathPoint (double x, double y) :
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

  PathPoint::~PathPoint ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
  }

  void
  PathPoint::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
  }

  void
  PathPoint::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
  }

  void
  PathLine::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_line (_x->get_value (), _y->get_value ());
    }
  }

  Process*
  PathLine::clone ()
  {
    return new PathLine (_x->get_value (), _y->get_value ());
  }

  void
  PathMove::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_move (_x->get_value (), _y->get_value ());
    }
  }

  Process*
  PathMove::clone ()
  {
    return new PathMove (_x->get_value (), _y->get_value ());
  }

  PathClosure::PathClosure (Process* p, const string &n) :
    AbstractGObj (p, n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path closure must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  void
  PathClosure::draw ()
  {
    Backend::instance ()->draw_path_closure ();
  }

  Process*
  PathClosure::clone ()
  {
    return new PathClosure ();
  }

  PathQuadratic::PathQuadratic (Process* p, const string &n, double x1, double y1, double x, double y) :
      AbstractGObj (p, n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path quadratic must be Path\n";
      return;
    }
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    path->items ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PathQuadratic::PathQuadratic (double x1, double y1, double x, double y) :
      AbstractGObj ()
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
  }

  PathQuadratic::~PathQuadratic ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_cx1) {delete _cx1; _cx1 = nullptr;}
    if (_cy1) {delete _cy1; _cy1 = nullptr;}
    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_x1) {delete _x1; _x1 = nullptr;}
    if (_y1) {delete _y1; _y1 = nullptr;}
  }

  void
  PathQuadratic::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _cx1->enable (_frame);
    _cy1->enable (_frame);
  }

  void
  PathQuadratic::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _cx1->disable ();
    _cy1->disable ();
  }

  void
  PathQuadratic::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double x = _x->get_value ();
      double y = _y->get_value ();
      double x1 = _x1->get_value ();
      double y1 = _y1->get_value ();
      Backend::instance ()->draw_path_quadratic (x1, y1, x, y);
    }
  }

  Process*
  PathQuadratic::clone ()
  {
    return new PathQuadratic (_x1->get_value (), _y1->get_value (), _x->get_value (), _y->get_value ());
  }

  PathCubic::PathCubic (Process* p, const string &n, double x1, double y1, double x2, double y2, double x, double y) :
      AbstractGObj (p, n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path cubic must be Path\n";
      return;
    }
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx2->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cy2->disable ();
    path->items ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PathCubic::PathCubic (double x1, double y1, double x2, double y2, double x, double y) :
      AbstractGObj ()
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cy1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx2->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cy2->disable ();
  }

  PathCubic::~PathCubic ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_cx1) {delete _cx1; _cx1 = nullptr;}
    if (_cy1) {delete _cy1; _cy1 = nullptr;}
    if (_cx2) {delete _cx2; _cx2 = nullptr;}
    if (_cy2) {delete _cy2; _cy2 = nullptr;}


    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_x1) {delete _x1; _x1 = nullptr;}
    if (_y1) {delete _y1; _y1 = nullptr;}
    if (_x2) {delete _x2; _x2 = nullptr;}
    if (_y2) {delete _y2; _y2 = nullptr;}
  }

  void
  PathCubic::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _cx1->enable (_frame);
    _cy1->enable (_frame);
    _cx2->enable (_frame);
    _cy2->enable (_frame);
  }

  void
  PathCubic::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _cx1->disable ();
    _cy1->disable ();
    _cx2->disable ();
    _cy2->disable ();
  }

  void
  PathCubic::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double x = _x->get_value ();
      double y = _y->get_value ();
      double x1 = _x1->get_value ();
      double y1 = _y1->get_value ();
      double x2 = _x2->get_value ();
      double y2 = _y2->get_value ();
      Backend::instance ()->draw_path_cubic (x1, y1, x2, y2, x, y);
    }
  }

  Process*
  PathCubic::clone ()
  {
    return new PathCubic (_x1->get_value (), _y1->get_value (), _x2->get_value (), _y2->get_value (), _x->get_value (),
                          _y->get_value ());
  }

  PathArc::PathArc (Process* p, const string &n, double rx, double ry, double rotx, double fl, double swfl, double x,
                    double y) :
      AbstractGObj (p, n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path arc must be Path\n";
      return;
    }
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _rotx = new DoubleProperty (this, "rotx", rotx);
    _fl = new DoubleProperty (this, "fl", fl);
    _swfl = new DoubleProperty (this, "swfl", swfl);
    _rx = new DoubleProperty (this, "rx", rx);
    _ry = new DoubleProperty (this, "ry", ry);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _crotx = new Coupling (_rotx, ACTIVATION, update, ACTIVATION);
    _crotx->disable ();
    _cfl = new Coupling (_fl, ACTIVATION, update, ACTIVATION);
    _cfl->disable ();
    _cswfl = new Coupling (_swfl, ACTIVATION, update, ACTIVATION);
    _cswfl->disable ();
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _crx->disable ();
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    _cry->disable ();
    path->items ()->add_child (this, "");
    p->add_symbol(n, this);
  }

  PathArc::PathArc (double rx, double ry, double rotx, double fl, double swfl, double x, double y) :
      AbstractGObj ()
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _rotx = new DoubleProperty (this, "rotx", rotx);
    _fl = new DoubleProperty (this, "fl", fl);
    _swfl = new DoubleProperty (this, "swfl", swfl);
    _rx = new DoubleProperty (this, "rx", rx);
    _ry = new DoubleProperty (this, "ry", ry);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _crotx = new Coupling (_rotx, ACTIVATION, update, ACTIVATION);
    _crotx->disable ();
    _cfl = new Coupling (_fl, ACTIVATION, update, ACTIVATION);
    _cfl->disable ();
    _cswfl = new Coupling (_swfl, ACTIVATION, update, ACTIVATION);
    _cswfl->disable ();
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _crx->disable ();
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    _cry->disable ();
  }

  PathArc::~PathArc ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_crotx) {delete _crotx; _crotx = nullptr;}
    if (_cfl) {delete _cfl; _cfl = nullptr;}
    if (_cswfl) {delete _cswfl; _cswfl = nullptr;}
    if (_crx) {delete _crx; _crx = nullptr;}
    if (_cry) {delete _cry; _cry = nullptr;}

    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_rotx) {delete _rotx; _rotx = nullptr;}
    if (_fl) {delete _fl; _fl = nullptr;}
    if (_swfl) {delete _swfl; _swfl = nullptr;}
    if (_rx) {delete _rx; _rx = nullptr;}
    if (_ry) {delete _ry; _ry = nullptr;}
  }

  void
  PathArc::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _crotx->enable (_frame);
    _cfl->enable (_frame);
    _cswfl->enable (_frame);
    _crx->enable (_frame);
    _cry->enable (_frame);
  }

  void
  PathArc::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _crotx->disable ();
    _cfl->disable ();
    _cswfl->disable ();
    _crx->disable ();
    _cry->disable ();
  }

  void
  PathArc::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double x = _x->get_value ();
      double y = _y->get_value ();
      double rotx = _rotx->get_value ();
      double fl = _fl->get_value ();
      double swfl = _swfl->get_value ();
      double rx = _rx->get_value ();
      double ry = _ry->get_value ();
      Backend::instance ()->draw_path_arc (rx, ry, rotx, fl, swfl, x, y);
    }
  }

  Process*
  PathArc::clone ()
  {
    return new PathArc (_rx->get_value (), _ry->get_value (), _rotx->get_value (), _fl->get_value (),
                        _swfl->get_value (), _x->get_value (), _y->get_value ());
  }

  Path::Path () :
      AbstractGShape ()
  {
    _items = new List (this, "items");
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

  Path::Path (Process* p, const string &n) :
      AbstractGShape (p, n)
  {
    _items = new List (this, "items");
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

  Path::~Path ()
  {
    delete _items;
  }

  void
  Path::activate ()
  {
    AbstractGObj::activate ();
    _items->activation ();
  }

  void
  Path::deactivate ()
  {
    AbstractGObj::deactivate ();
    _items->deactivation ();
  }

  void
  Path::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path (this);
    }
  }

  Process*
  Path::clone ()
  {
    Path* clone = new Path ();
    for (auto p: _items->children ()) {
      clone->items ()->add_child (p->clone (), "");
    }
    return clone;
  }

  void
  PathClip::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_clip (this);
    }
  }

  Process*
  PathClip::clone ()
  {
    PathClip* clone = new PathClip ();
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
