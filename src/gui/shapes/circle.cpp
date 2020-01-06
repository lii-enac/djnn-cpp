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


#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"
#include "gui/shapes/shapes.h"
#include "gui/style/style.h"
#include "core/ontology/coupling.h"

#include "shapes.h"
#include "gui/shapes/sdf.h"

namespace djnn
{
  Circle::Circle (Process *parent, const std::string& name, double cx, double cy, double r)
  : AbstractPropCircle (parent, name, cx, cy, r)
  {
    Process::finalize_construction (parent, name);
  }

  Process*
  Circle::clone ()
  {
    return new Circle (nullptr, get_name (), raw_props.cx, raw_props.cy, raw_props.r);
  }
 
  void
  Circle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_circle (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Circle::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = raw_props.cx - raw_props.r;
    y = raw_props.cy - raw_props.r;
    w = raw_props.r  + raw_props.r;
    h = raw_props.r  + raw_props.r;
  }

  double
  Circle::sdf (double x, double y) const
  {
    auto & circle = raw_props;
    vec2 p = vec2(x + circle.cx, y + circle.cy);
    double d;
    d = SDF_circle(p, circle.r);
    return d;
  }

  
} /* namespace djnn */
