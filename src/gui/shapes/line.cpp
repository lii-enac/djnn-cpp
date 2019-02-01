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
#include "../../core/control/coupling.h"
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx1 = new Coupling (_x1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy1 = new Coupling (_y1, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cx2 = new Coupling (_x2, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    _cy2 = new Coupling (_y2, ACTIVATION, update, ACTIVATION);
    _cx1->disable ();
    set_origin (x1, y1);
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
    if (_cx1) {delete _cx1; _cx1 = nullptr;}
    if (_cy1) {delete _cy1; _cy1 = nullptr;}
    if (_cx2) {delete _cx2; _cx2 = nullptr;}
    if (_cy2) {delete _cy2; _cy2 = nullptr;}
    if (_x1) {delete _x1; _x1 = nullptr;}
    if (_y1) {delete _y1; _y1 = nullptr;}
    if (_x2) {delete _x2; _x2 = nullptr;}
    if (_y2) {delete _y2; _y2 = nullptr;}
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
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_line (this);
    }
  }

  void
  Line::get_properties_values (double &x1, double &y1, double &x2, double &y2)
  {
    x1 = _x1->get_value ();
    y1 = _y1->get_value ();
    x2 = _x2->get_value ();
    y2 = _y2->get_value ();
  }

  Process*
  Line::clone ()
  {
    return new Line (_x1->get_value (), _y1->get_value (), _x2->get_value (), _y2->get_value ());
  }
} /* namespace djnn */
