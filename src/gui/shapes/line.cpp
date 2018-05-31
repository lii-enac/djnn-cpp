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
#include "../window.h"

namespace djnn
{

  void
  Line::init_line (double x1, double y1, double x2, double y2)
  {
    _x1 = new DoubleProperty (this, "x1", x1);
    _y1 = new DoubleProperty (this, "y1", y1);
    _x2 = new DoubleProperty (this, "x2", x2);
    _y2 = new DoubleProperty (this, "y2", y2);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
  }

  Line::Line (Process *p, const std::string& n, double x1, double y1, double x2, double y2) :
      AbstractGShape (p, n), _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    init_line (x1, y1, x2, y2);
    Process::finalize ();
  }

  Line::Line (double x1, double y1, double x2, double y2) :
      AbstractGShape (), _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    init_line (x1, y1, x2, y2);
  }

  Line::~Line ()
  {
    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;
    delete _x1;
    delete _y1;
    delete _x2;
    delete _y2;
  }

  void
  Line::activate ()
  {
    AbstractGObj::activate ();
    _cx1->enable (_frame);
    _cy1->enable (_frame);
    _cx2->enable (_frame);
    _cy2->enable (_frame);
  }

  void
  Line::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx1->disable ();
    _cy1->disable ();
    _cx2->disable ();
    _cy2->disable ();
  }

  void
  Line::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double x1 = _x1->get_value ();
      double y1 = _y1->get_value ();
      double x2 = _x2->get_value ();
      double y2 = _y2->get_value ();
      Backend::instance ()->draw_line (this, x1, y1, x2, y2);
    }
  }
} /* namespace djnn */
