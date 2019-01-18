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
  Ellipse::Ellipse (Process *p, const std::string& n, double cx, double cy, double rx, double ry) :
      AbstractGShape (p, n),
      raw_props{.cx=cx, .cy=cy, .rx=rx, .ry=ry},
      _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin(cx,cy);
    Process::finalize ();
  }

  Ellipse::Ellipse (double cx, double cy, double rx, double ry) :
      AbstractGShape (),
      raw_props{.cx=cx, .cy=cy, .rx=rx, .ry=ry},
      _ccx (nullptr), _ccy (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin(cx,cy);
  }

  Ellipse::~Ellipse ()
  {
    delete _ccx; _ccx = nullptr;
    delete _ccy; _ccy = nullptr;
    delete _crx; _crx = nullptr;
    delete _cry; _cry = nullptr;
  }

  Process*
  Ellipse::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="cx") {
      coupling=&_ccx;
      rawp=&raw_props.cx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="cy") {
      coupling=&_ccy;
      rawp=&raw_props.cy;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="rx") {
      coupling=&_crx;
      rawp=&raw_props.rx;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="ry") {
      coupling=&_cry;
      rawp=&raw_props.ry;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_none);

    return res;
  }

  void
  Ellipse::activate ()
  {
    AbstractGObj::activate ();
    if(_ccx) _ccx->enable (_frame);
    if(_ccy) _ccy->enable (_frame);
    if(_crx) _crx->enable (_frame);
    if(_cry) _cry->enable (_frame);
  }

  void
  Ellipse::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_ccx) _ccx->disable ();
    if(_ccy) _ccy->disable ();
    if(_crx) _crx->disable ();
    if(_cry) _cry->disable ();
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
    cx = raw_props.cx;
    cy = raw_props.cy;
    rx = raw_props.rx;
    ry = raw_props.ry;
  }

  Process*
  Ellipse::clone ()
  {
    return new Ellipse (raw_props.cx, raw_props.cy, raw_props.rx, raw_props.ry);
  }

}

