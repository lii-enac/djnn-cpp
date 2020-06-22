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
  Ellipse::Ellipse (ParentProcess* parent, const std::string& name, double cx, double cy, double rx, double ry)
  : AbstractPropEllipse (parent, name, cx, cy, rx, ry)
  {
    finalize_construction (parent, name);
  }

  Ellipse*
  Ellipse::clone ()
  {
    return new Ellipse (nullptr, get_name (), raw_props.cx, raw_props.cy, raw_props.rx, raw_props.ry);
  }
 
  void
  Ellipse::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_ellipse (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Ellipse::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = raw_props.cx - raw_props.rx;
    y = raw_props.cy - raw_props.ry;
    w = raw_props.rx + raw_props.rx;
    h = raw_props.ry + raw_props.ry;
  }

  double
  Ellipse::sdf (double x, double y) const
  {
    auto & ellipse = raw_props;
    vec2 p = vec2(x - ellipse.cx, y - ellipse.cy);
    double d;
    d = SDF_ellipse(p, vec2(ellipse.rx, ellipse.ry));
    return d;
  }

  
} /* namespace djnn */
