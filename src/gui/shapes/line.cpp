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
#include "../window.h"

namespace djnn
{

  Line::Line (Process *p, const std::string& n, double x1, double y1, double x2, double y2) :
      AbstractGShape (p, n),
      raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    set_origin(x1,y1);
    Process::finalize ();
  }

  Line::Line (double x1, double y1, double x2, double y2) :
      AbstractGShape (),
      raw_props{.x1=x1, .y1=y1, .x2=x2, .y2=y2},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
  {
    set_origin(x1,y1);
  }

  Line::~Line ()
  {
    delete _cx1; _cx1 = nullptr;
    delete _cy1; _cy1 = nullptr;
    delete _cx2; _cx2 = nullptr;
    delete _cy2; _cy2 = nullptr;
  }

  Process*
  Line::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    Coupling ** coupling;
    double* rawp;
    int notify_mask = notify_none;

    if(name=="x1") {
      coupling=&_cx1;
      rawp=&raw_props.x1;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y1") {
      coupling=&_cy1;
      rawp=&raw_props.y1;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="x2") {
      coupling=&_cx2;
      rawp=&raw_props.x2;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="y2") {
      coupling=&_cy2;
      rawp=&raw_props.y2;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    DoublePropertyProxy* prop = nullptr; // do not cache
    res = create_GObj_prop(&prop, coupling, rawp, name, notify_mask);

    return res;
  }

  void
  Line::activate ()
  {
    AbstractGObj::activate ();
    if (_cx1) _cx1->enable (_frame);
    if (_cy1) _cy1->enable (_frame);
    if (_cx2) _cx2->enable (_frame);
    if (_cy2) _cy2->enable (_frame);
  }

  void
  Line::deactivate ()
  {
    AbstractGObj::deactivate ();
    if (_cx1) _cx1->disable ();
    if (_cy1) _cy1->disable ();
    if (_cx2) _cx2->disable ();
    if (_cy2) _cy2->disable ();
  }

  void
  Line::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_line ( this );
    }
  }

  void
  Line::get_properties_values (double &x1, double &y1, double &x2, double &y2)
  {
    x1 = raw_props.x1;
    y1 = raw_props.y1;
    x2 = raw_props.x2;
    y2 = raw_props.y2;
  }

  Process*
  Line::clone ()
  {
    return new Line ( raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2 );
  }
} /* namespace djnn */
