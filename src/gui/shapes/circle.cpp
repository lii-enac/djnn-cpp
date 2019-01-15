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

#include "../../core/control/coupling.h"
#include "../backend.h"
#include "../abstract_backend.h"
#include "../window.h"
#include "shapes.h"

namespace djnn
{
  Circle::Circle (Process *p, const std::string& n, double cx, double cy, double r) :
      AbstractGShape (p, n),
      raw_props{.cx=cx, .cy=cy, .r=r},
      _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    set_origin(cx,cy);
    Process::finalize ();
  }

  Circle::Circle (double cx, double cy, double r) :
      AbstractGShape (),
      raw_props{.cx=cx, .cy=cy, .r=r},
      _ccx (nullptr), _ccy (nullptr), _cr (nullptr)
  {
    set_origin(cx,cy);
  }

  Circle::~Circle ()
  {
    delete _ccx; _ccx = nullptr;
    delete _ccy; _ccy = nullptr;
    delete _cr; _cr = nullptr;
  }

  Process*
  Circle::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;

    if(name=="cx") {
      coupling=&_ccx;
      rawp=&raw_props.cx;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp=&raw_props.cy;
    } else
    if(name=="r") {
      coupling=&_cr;
      rawp=&raw_props.r;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name);

    return res;
  }

  void
  Circle::activate ()
  {
    AbstractGObj::activate ();
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
    if(_cr)  _cr->enable (_frame);
  }

  void
  Circle::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
    if(_cr)  _cr->disable ();
  }

  void
  Circle::draw ()
  {

    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_circle (this);
    }
  }

  void
  Circle::get_properties_values (double &cx, double &cy, double &r)
  {
    cx = raw_props.cx;
    cy = raw_props.cy;
    r = raw_props.r;
  }

  Process*
  Circle::clone ()
  {
    return new Circle (raw_props.cx, raw_props.cy, raw_props.r);
  }

} /* namespace djnn */
