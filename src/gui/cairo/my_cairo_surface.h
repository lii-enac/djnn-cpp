/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "cairo_backend.h"
#include "../../core/syshook/external_source.h"
#include "../window.h"

#include <string>

#include <cairo/cairo.h>

namespace djnn {

  class MyCairoSurface
  {
  public:
    MyCairoSurface (Window *win);
    virtual ~MyCairoSurface ();
    void update (cairo_surface_t* drawing_surface, cairo_surface_t* picking_surface);

  private:
    Window* _window;
  };

}
