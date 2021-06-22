/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "utils/debug.h"

#include "gui/cairo/drm/cairo_drm_window.h"
#include "gui/cairo/my_cairo_surface.h"
#include "gui/backend.h"

#include "display/update_drawing.h"
#include "display/display.h"
#include "display/abstract_display.h"

#include <stdio.h>

//#include <iostream>

#include "exec_env/exec_env-dev.h"

#if _DEBUG_SEE_GUI_INFO_PREF
//#include "core/utils/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{

  CairoDRMWindow::CairoDRMWindow (djnn::Window* win, const djnn::string& title, double x, double y, double w, double h) :
  DRMWindow(win, title, x, y, w, h),
    _picking_data (nullptr), _my_cairo_surface (nullptr), _picking_view (nullptr), _refresh (this, "refresh"),
    _change_dpy_action (this, "change_dpy_action"),
    _c_vblank (nullptr), _c_display (nullptr)
  {
    int stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, w);
    _picking_data = new unsigned char[stride*((int)h)];
  }

  CairoDRMWindow::~CairoDRMWindow ()
  {
    if (is_activated)
      impl_deactivate ();
    delete _c_vblank;
    delete _c_display;
  }

  void
  CairoDRMWindow::impl_activate ()
  {
    _picking_view = new CairoPickingView (_window);
    WinImpl::set_picking_view (_picking_view);
    _my_cairo_surface = new MyCairoSurface (_window);

    is_activated = true;
    _c_display = new Coupling (_window->find_child_impl ("display"), ACTIVATION, &_change_dpy_action, ACTIVATION, true);
    if (_window->get_display () != nullptr && _window->get_display () != _conn)
      update_dpy_connection();
    else {
      delete _c_vblank;
      _window->init_display (_conn);
      _c_vblank = new Coupling (get_vblank (), ACTIVATION, &_refresh, ACTIVATION, true);
      _c_vblank->disable ();
    }
  }


  void
  CairoDRMWindow::update_dpy_connection ()
  {
    delete _c_vblank;
    DRMConnector* c = dynamic_cast<DRMConnector*> (_window->get_display ());
    if (c == nullptr || !c->is_connected ()) {
      _conn = nullptr;
      return;
    }
    _conn = c;
    _c_vblank = new Coupling (_conn->get_vblank (), ACTIVATION, &_refresh, ACTIVATION, true);
    _c_vblank->disable ();
  }

  void
  CairoDRMWindow::impl_deactivate ()
  {
    delete _picking_view;
    WinImpl::set_picking_view (nullptr);

    delete _my_cairo_surface;
    _my_cairo_surface = nullptr;
    delete _picking_data;
    _picking_data = nullptr;
    is_activated = false;
  }

  void
  CairoDRMWindow::ask_refresh ()
  {
    //UpdateDrawing::instance ()->add_window_for_refresh (_window);
  }

  void
  CairoDRMWindow::update ()
  {
    //DBG;
    #if _DEBUG_SEE_GUI_INFO_PREF
      t1();
    #endif
    
    if (!getBool(_conn->find_child_impl ("connected")))
      return;
    if (is_waiting_for_vblank()) {
      _c_vblank->enable ();
    }
    _c_vblank->disable ();

    CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());
    auto dbackend = DisplayBackend::instance();
    dbackend->set_window (_window);
    backend->set_picking_view (_picking_view);
    _picking_view->init ();
        
    buff* surface = get_next_buff ();

    cairo_surface_t *drawing_surface, *picking_surface;
    drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) surface->map, CAIRO_FORMAT_ARGB32,
                                                           surface->width, surface->height, surface->stride);
    picking_surface = cairo_image_surface_create_for_data ((unsigned char*) _picking_data, CAIRO_FORMAT_ARGB32,
                                                           surface->width, surface->height, surface->stride);
    _my_cairo_surface->update (drawing_surface, picking_surface, surface->width, surface->height);
    cairo_surface_flush (drawing_surface);
    cairo_surface_flush (picking_surface);

    unsigned char *picking_data = cairo_image_surface_get_data (picking_surface);
    _picking_view->set_data (picking_data, surface->width, surface->height,
                             cairo_image_surface_get_stride (picking_surface));

    flip_page ();

    cairo_surface_destroy (drawing_surface);
    cairo_surface_destroy (picking_surface);

    #if _DEBUG_SEE_GUI_INFO_PREF
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
