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

#include "../../core/control/coupling.h"
#include "../backend.h"
#include "../abstract_backend.h"
#include "../window.h"
#include "shapes.h"
namespace djnn
{

  void
  Circle::init_circle (double cx, double cy, double r)
  {
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    _r = new DoubleProperty (this, "r", r);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _cr = new Coupling (_r, ACTIVATION, update, ACTIVATION);
    set_origin (cx, cy);
  }

  Circle::Circle (Process *p, const std::string& n, double cx, double cy, double r) :
      AbstractGShape (p, n), _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    init_circle (cx, cy, r);
    Process::finalize ();
  }

  Circle::Circle (double cx, double cy, double r) :
      AbstractGShape (), _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    init_circle (cx, cy, r);
  }

  Circle::~Circle ()
  {
    delete _ccx;
    delete _ccy;
    delete _cr;
    delete _cx;
    delete _cy;
    delete _r;
  }

  void
  Circle::activate ()
  {
    AbstractGObj::activate ();
    _ccx->enable (_frame);
    _ccy->enable (_frame);
    _cr->enable (_frame);
  }

  void
  Circle::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ccx->disable ();
    _ccy->disable ();
    _cr->disable ();
  }

  void
  Circle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame)
      Backend::instance ()->draw_circle (this, _cx->get_value (), _cy->get_value (), _r->get_value ());
  }
} /* namespace djnn */
