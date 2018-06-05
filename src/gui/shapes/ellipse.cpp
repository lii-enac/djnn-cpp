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
  Ellipse::init_ellipse (double cx, double cy, double rx, double ry)
  {
    _cx = new DoubleProperty (this, "cx", cx);
    _cy = new DoubleProperty (this, "cy", cy);
    _rx = new DoubleProperty (this, "rx", rx);
    _ry = new DoubleProperty (this, "ry", ry);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
  }

  Ellipse::Ellipse (Process *p, const std::string& n, double cx, double cy, double rx, double ry) :
      AbstractGShape (p, n), _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    init_ellipse (cx, cy, rx, ry);
    Process::finalize ();
  }

  Ellipse::Ellipse (double cx, double cy, double rx, double ry) :
      AbstractGShape (), _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    init_ellipse (cx, cy, rx, ry);
  }

  Ellipse::~Ellipse ()
  {
    delete _ccx;
    delete _ccy;
    delete _crx;
    delete _cry;
    delete _cx;
    delete _cy;
    delete _rx;
    delete _ry;
  }

  void
  Ellipse::activate ()
  {
    AbstractGObj::activate ();
    _ccx->enable (_frame);
    _ccy->enable (_frame);
    _crx->enable (_frame);
    _cry->enable (_frame);
  }

  void
  Ellipse::deactivate ()
  {
    AbstractGObj::deactivate ();
    _ccx->disable ();
    _ccy->disable ();
    _crx->disable ();
    _cry->disable ();
  }

  void
  Ellipse::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame)
      Backend::instance ()->draw_ellipse (this, _cx->get_value (), _cy->get_value (), _rx->get_value (), _ry->get_value ());
  }
} /* namespace djnn */
