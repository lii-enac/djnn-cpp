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

#include "../backend.h"
#include "../abstract_backend.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  Rectangle::Rectangle (Process *p, const std::string& n, double x, double y, double w, double h, double rx = 0, double ry = 0) :
      AbstractGShape (p, n),
      raw_props{.x=x, .y=y, .width=w, .height=h, .rx=rx, .ry=ry},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
    Process::finalize ();
  }

  Rectangle::Rectangle (double x, double y, double w, double h, double rx = 0, double ry = 0) :
      AbstractGShape (),
      raw_props{.x=x, .y=y, .width=w, .height=h, .rx=rx, .ry=ry},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (nullptr)
  {
    set_origin (x, y);
  }

  Rectangle::~Rectangle ()
  {
    delete _cx;
    delete _cy;
    delete _cwidth;
    delete _cheight;
    delete _crx;
    delete _cry;
  }

  
  Process*
  Rectangle::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="width") {
      coupling=&_cwidth;
      rawp=&raw_props.width;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="height") {
      coupling=&_cheight;
      rawp=&raw_props.height;
      notify_mask = notify_damaged_geometry;
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
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  Rectangle::get_properties_values (double &x, double &y, double &w, double &h, double &rx, double &ry)
  {
    x = raw_props.x;
    y = raw_props.y;
    w = raw_props.width;
    h = raw_props.height;
    rx = raw_props.rx;
    ry = raw_props.ry;
  }

  void
  Rectangle::activate ()
  {
    AbstractGObj::activate ();
    if(_cx) _cx->enable (_frame);
    if(_cy) _cy->enable (_frame);
    if(_cwidth) _cwidth->enable (_frame);
    if(_cheight) _cheight->enable (_frame);
    if(_crx) _crx->enable (_frame);
    if(_cry) _cry->enable (_frame);
  }

  void
  Rectangle::deactivate ()
  {
    AbstractGObj::deactivate ();
    if(_cx) _cx->disable ();
    if(_cy) _cy->disable ();
    if(_cwidth) _cwidth->disable ();
    if(_cheight) _cheight->disable ();
    if(_crx) _crx->disable ();
    if(_cry) _cry->disable ();
  }

  void
  Rectangle::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_rect (this);
    }
  }


  Process*
  Rectangle::clone ()
  {
    return new Rectangle (raw_props.x, raw_props.y, raw_props.width, raw_props.height, raw_props.rx, raw_props.ry);
  }
} /* namespace djnn */
