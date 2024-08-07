/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include <cairo.h>

#include "cairo_backend.h"
#include "display/window.h"
#include "exec_env/external_source.h"

namespace djnn {

class MyCairoSurface {
  public:
    MyCairoSurface (Window* win);
    virtual ~MyCairoSurface ();
    void update (cairo_surface_t* drawing_surface, cairo_surface_t* picking_surface, int width = 0, int height = 0);

  private:
    Window* _window;
};

} // namespace djnn
