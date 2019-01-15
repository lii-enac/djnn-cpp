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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/control/coupling.h"
#include "shapes.h"

namespace djnn
{

  RectangleClip::RectangleClip (Process *p, const std::string& n, double x, double y, double w, double h) :
      AbstractGShape (p, n),
      raw_props{.x=x, .y=y, .width=w, .height=h},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr)
  {
    set_origin (x, y);
    Process::finalize ();
  }

  RectangleClip::RectangleClip (double x, double y, double w, double h) :
      AbstractGShape (),
      raw_props{.x=x, .y=y, .width=w, .height=h},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr)
  {
    set_origin (x, y);
  }

  RectangleClip::~RectangleClip ()
  {
    delete _cx; _cx = nullptr;
    delete _cy; _cy = nullptr;
    delete _cwidth; _cwidth = nullptr;
    delete _cheight; _cheight = nullptr;
  }

  Process*
  RectangleClip::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;

    if(name=="x") {
      coupling=&_cx;
      rawp=&raw_props.x;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawp=&raw_props.y;
    } else
    if(name=="width") {
      coupling=&_cwidth;
      rawp=&raw_props.width;
    } else
    if(name=="height") {
      coupling=&_cheight;
      rawp=&raw_props.height;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawp, name);

    return res;

  }

  void
  RectangleClip::activate ()
  {
    AbstractGObj::activate ();
    if (_cx) _cx->enable (_frame);
    if (_cy) _cy->enable (_frame);
    if (_cwidth) _cwidth->enable (_frame);
    if (_cheight) _cheight->enable (_frame);
  }

  void
  RectangleClip::deactivate ()
  {
    AbstractGObj::deactivate ();
    if (_cx) _cx->disable ();
    if (_cy) _cy->disable ();
    if (_cwidth) _cwidth->disable ();
    if (_cheight) _cheight->disable ();
  }

  void
  RectangleClip::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_rect_clip (this);
    }
  }

  void
  RectangleClip::get_properties_values (double &x, double &y, double &w, double &h)
  {
    x = raw_props.x;
    y = raw_props.y;
    w = raw_props.width;
    h = raw_props.height;
  }

  Process*
  RectangleClip::clone() {
    return new RectangleClip (raw_props.x, raw_props.y, raw_props.width, raw_props.height);
  }
} /* namespace djnn */
