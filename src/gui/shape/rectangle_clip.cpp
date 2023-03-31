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

#include "gui/shape/rectangle_clip.h"
#include "gui/picking/analytical_picking_context.h"

namespace djnn
{

  RectangleClip::RectangleClip (CoreProcess* parent, const string& name, double x, double y, double width, double height) :
    AbstractPropRectangleClip (parent, name, x, y, width, height)
  {
    finalize_construction (parent, name);
  }

  RectangleClip*
  RectangleClip::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new RectangleClip (nullptr, get_name (), raw_props.x, raw_props.y, raw_props.width, raw_props.height);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
  }

  void
  RectangleClip::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_rectangle_clip (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  RectangleClip::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = raw_props.x;
    y = raw_props.y;
    w = raw_props.width;
    h = raw_props.height;
  }

  double
  RectangleClip::sdf (double x, double y) const
  {
    auto & rectangle = raw_props;
    vec2 p = vec2(x + rectangle.x, y + rectangle.y);
    double d;

    vec2 size = vec2(rectangle.width, rectangle.height)/2.; // /2. why oh why?
    size      = floor(size);
    vec2 p1   = p-size;
    d = SDF_box(p1, size); // FIXME ry
    return d;
  }

  AbstractGShape*
  RectangleClip::pick_analytical (PickAnalyticalContext& pac)
  {
    // reversed semantics: if it's in let the enclosing container know
    double x,y,w,h;
    get_bounding_box (x,y,w,h);
    if( ( (x - pac.half_outline_width) <= pac.x &&
            pac.x <= (x + w + pac.half_outline_width*2) &&
           (y - pac.half_outline_width) <= pac.y &&
            pac.y <= (y + h + pac.half_outline_width*2)))
      pac.clipped = true;
    return nullptr;
  }
  
} /* namespace djnn */
