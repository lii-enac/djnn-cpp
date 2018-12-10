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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _ccx = new Coupling (_cx, ACTIVATION, update, ACTIVATION);
    _ccx->disable ();
    _ccy = new Coupling (_cy, ACTIVATION, update, ACTIVATION);
    _ccy->disable ();
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _crx->disable ();
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    _cry->disable ();
    set_origin (cx, cy);
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
    
    if (_ccx) { delete _ccx ; _ccx = nullptr;}
    if (_ccy) { delete _ccy ; _ccy = nullptr;}
    if (_crx) { delete _crx ; _crx = nullptr;}
    if (_cry) { delete _cry ; _cry = nullptr;}

    if (_cx) { delete _cx ; _cx = nullptr;}
    if (_cy) { delete _cy ; _cy = nullptr;}
    if (_rx) { delete _rx ; _rx = nullptr;}
    if (_ry) { delete _ry ; _ry = nullptr;}
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
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_ellipse (this);
    }
  }

  void
  Ellipse::get_properties_values (double &cx, double &cy, double &rx, double &ry)
  {
    cx = _cx->get_value ();
    cy = _cy->get_value ();
    rx = _rx->get_value ();
    ry = _ry->get_value ();
>>>>>>> 7c86dfb2... gui backend change draw API to prepare for caching
  }

  Process*
  Ellipse::clone ()
  {
    return new Ellipse (_cx->get_value (), _cy->get_value (), 
                        _rx->get_value (), _ry->get_value ());
  } 
} /* namespace djnn */
