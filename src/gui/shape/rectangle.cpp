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
#include "display/display-dev.h" // DisplayBackend::instance
#include "gui/shape/sdf.h"

#include "gui/shape/rectangle.h"

namespace djnn
{

  Rectangle::Rectangle (ParentProcess* parent, const djnn::string& name, double x, double y, double width, double height, double rx, double ry) :
    AbstractPropRectangle (parent, name, x, y, width, height, rx, ry)
  {
    finalize_construction (parent, name);
  }

  Rectangle*
  Rectangle::clone ()
  {
    return new Rectangle (nullptr, get_name (), raw_props.x, raw_props.y, raw_props.width, raw_props.height, raw_props.rx, raw_props.ry);
  }

  void
  Rectangle::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_rectangle (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Rectangle::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = raw_props.x;
    y = raw_props.y;
    w = raw_props.width;
    h = raw_props.height;
  }

  double
  Rectangle::sdf (double x, double y) const
  {
    // SVG Rectangle sdf, code by Stéphane Conversy... 
    auto & rectangle = raw_props;
    vec2 p = vec2(x - rectangle.x, y - rectangle.y);
    vec2 r    = vec2(rectangle.rx, rectangle.ry);
    vec2 size = vec2(rectangle.width, rectangle.height)/2.-r; // /2. why oh why?
    size      = floor(size);
    vec2 p1   = p-size-r;

     //... adapted from N. Rougier, https://www.labri.fr/perso/nrougier/python-opengl/#geometrical-primitives
    //itself adapted from Inigo Quilez, https://www.shadertoy.com/view/4llXD7
    double d = SDF_round_box(p1, size, r.x); // FIXME ry

    return d;
  }
  
} /* namespace djnn */
