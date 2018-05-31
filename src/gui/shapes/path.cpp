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
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/coupling.h"
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
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    path->items ()->add_child (this, "");
  }

  PathPoint::~PathPoint ()
  {
    delete _cx;
    delete _cy;
    delete _x;
    delete _y;
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

  void
  PathMove::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_move (_x->get_value (), _y->get_value ());
    }
  }

  PathClosure::PathClosure (Process* p, const string &n)
  {
    Path *path = dynamic_cast<Path*> (p);
    if (path == nullptr) {
      cerr << "Parent of path closure must be Path\n";
      return;
    }
    path->items ()->add_child (this, "");
  }

  void
  PathClosure::draw ()
  {
    Backend::instance ()->draw_path_closure ();
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
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    path->items ()->add_child (this, "");
  }

  PathQuadratic::~PathQuadratic ()
  {
    delete _cx;
    delete _cy;
    delete _cx1;
    delete _cy1;
    delete _x;
    delete _y;
    delete _x1;
    delete _y1;
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

  PathCubic::PathCubic (Process* p, const string &n, double x1, double y1, double x2, double y2, double x,
                        double y) :
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
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    path->items ()->add_child (this, "");
  }

  PathCubic::~PathCubic ()
  {
    delete _cx;
    delete _cy;
    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;

    delete _x;
    delete _y;
    delete _x1;
    delete _y1;
    delete _x2;
    delete _y2;
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

  PathArc::PathArc (Process* p, const string &n, double rx, double ry, double rotx, double fl, double swfl,
                    double x, double y) :
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
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _crotx = new Coupling (_rotx, ACTIVATION, update, ACTIVATION);
    _cfl = new Coupling (_fl, ACTIVATION, update, ACTIVATION);
    _cswfl = new Coupling (_swfl, ACTIVATION, update, ACTIVATION);
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    path->items ()->add_child (this, "");
  }

  PathArc::~PathArc ()
  {
    delete _cx;
    delete _cy;
    delete _crotx;
    delete _cfl;
    delete _cswfl;
    delete _crx;
    delete _cry;

    delete _x;
    delete _y;
    delete _rotx;
    delete _fl;
    delete _swfl;
    delete _rx;
    delete _ry;
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

  Path::Path () :
      AbstractGShape ()
  {
    _items = new List (this, "items");
  }

  Path::Path (Process* p, const string &n) :
      AbstractGShape (p, n)
  {
    _items = new List (this, "items");
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
  void
  PathClip::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_path_clip (this);
    }
  }
}
