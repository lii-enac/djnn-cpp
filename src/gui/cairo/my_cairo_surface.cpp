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

#include "my_cairo_surface.h"
#include "gui/backend.h"
#include "cairo_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DEBUG_PICKING  0

#ifndef DOUBLE_BUFFER
#define DOUBLE_BUFFER 1
#endif

#include "exec_env/exec_env-dev.h"
#define _PERF_TEST 0
#if _PERF_TEST
//#include "core/utils/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{

  MyCairoSurface::MyCairoSurface (Window *win) :
      _window (win)
  {

  }

  MyCairoSurface::~MyCairoSurface ()
  {
  }

  void
  MyCairoSurface::update (cairo_surface_t* drawing_surface, cairo_surface_t* picking_surface, int width, int height)
  {
    if (!drawing_surface)
      return;
    //CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());
    DisplayBackend::instance()->set_window (_window);
    Process *p = _window->holder ();
    if (p) {
#if _PERF_TEST
      t1();
#endif

      int w =  width == 0 ? (int) _window->width ()->get_value() : width;
      int h = height == 0 ? (int) _window->height ()->get_value() : height;
      cairo_surface_t* draw_dest;
      //cairo_surface_t* pick_dest =  cairo_image_surface_create (CAIRO_FORMAT_RGB24, w, h);
#if DOUBLE_BUFFER
      /* create a drawing buffer... */
      cairo_surface_t* buf = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, w, h);
      /* and draw on it */
      draw_dest = buf;
#else
      /* directly draw on the final destination */
      draw_dest = drawing_surface;
#endif
      cur_cairo_state = cairo_create (draw_dest);
      cur_cairo_picking_state = cairo_create (picking_surface);
      if (width == 0 && height == 0) {
        cairo_set_source_rgb (cur_cairo_state, 0.6, 0.6, 0.6);
      } else {
        cairo_set_source_rgb (cur_cairo_state, 0, 0, 0);
      }
      cairo_paint (cur_cairo_state);
      p->draw ();
      cairo_destroy (cur_cairo_state);
      cairo_destroy (cur_cairo_picking_state);
#if DOUBLE_BUFFER
      /* copy the buffer to the final destination */
      cairo_t *cr = cairo_create (drawing_surface);
      cairo_set_source_surface (cr, buf, 0, 0);
      cairo_paint (cr);
      cairo_destroy (cr);

      /* destroy the buffer */
      cairo_surface_destroy (buf);
#endif
#if _PERF_TEST
      // print in RED
      cerr << "\033[1;31m";
      double time = t2 ("DRAW : ");
      draw_counter = draw_counter + 1;
      draw_total = draw_total + time ;
      draw_average = draw_total / draw_counter;
      cerr << "DRAW : " << draw_counter << " - avg: " << draw_average << endl;
      cerr << "\033[0m"  << endl;
#endif
    }
  }

}
