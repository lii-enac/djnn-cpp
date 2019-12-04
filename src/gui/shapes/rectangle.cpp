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

#include <iostream>

namespace djnn
{

  Rectangle::Rectangle (Process *parent, const std::string& name, double x, double y, double width, double height, double rx, double ry) :
    AbstractPropRectangle (parent, name, x, y, width, height, rx, ry)
  {
    Process::finalize_construction (parent, name);
  }

  Rectangle::Rectangle (double x, double y, double width, double height, double rx, double ry) :
    AbstractPropRectangle (x, y, width, height, rx, ry)
  {
  }

  void
  Rectangle::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_rectangle (this);
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
    auto & rectangle = raw_props;
    vec2 p = vec2(x + rectangle.x, y + rectangle.y);
    double d;

    vec2 r    = vec2(rectangle.rx, rectangle.ry);
    vec2 size = vec2(rectangle.width, rectangle.height)/2.-r; // /2. why oh why?
    size      = floor(size);
    vec2 p1   = p-size-r;
    d = SDF_round_box(p1, size, r.x); // FIXME ry
    return d;
  }
  
} /* namespace djnn */
