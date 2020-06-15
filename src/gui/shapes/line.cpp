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
  Line::Line (FatProcess *parent, const std::string& name, double x1, double y1, double x2, double y2)
  : AbstractPropLine (parent, name, x1, y1, x2, y2)
  {
    finalize_construction (parent, name);
  }

  Line*
  Line::clone ()
  {
    return new Line (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2);
  }
 
  void
  Line::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_line (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Line::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = min(raw_props.x1,  raw_props.x2);
    y = min(raw_props.y1,  raw_props.y2);
    w = abs(raw_props.x1 - raw_props.x1);
    h = abs(raw_props.y1 - raw_props.y2);
  }

  double
  Line::sdf (double x, double y) const
  {
    //auto & ellipse = raw_props;
    vec2 p = vec2(x, y);
    double d;
    d = SDF_plane(p, vec2(raw_props.x1, raw_props.y1), vec2(raw_props.x2, raw_props.y2));
    return d;
  }

  
} /* namespace djnn */
