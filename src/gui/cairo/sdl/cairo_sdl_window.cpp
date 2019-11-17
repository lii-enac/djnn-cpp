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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "../../../display/sdl/sdl_mainloop.h"
#include "cairo_sdl_window.h"
#include "../my_cairo_surface.h"
#include "../../backend.h"

#include "../../../display/display.h"
#include "../../../display/abstract_display.h"

#include "../../../core/syshook/syshook.h"
#include "../../../core/execution/graph.h"
#include "../../../core/syshook/main_loop.h"

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/select.h>

#include <iostream>

#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "
#define _PERF_TEST 0
#if _PERF_TEST
#include "../../core/utils/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{

//  int mouse_tracking; // unused but important for Qt

  CairoSDLWindow::CairoSDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
  SDLWindow(win, title, x,y,w,h),
      _sdl_surface (nullptr), _sdl_renderer (nullptr), _sdl_texture (nullptr), _picking_data (nullptr), _my_cairo_surface (nullptr)
  {

#if PICKING_DBG
    _pick_sdl_renderer = nullptr;
    _pick_sdl_surface = nullptr;
    _pick_sdl_texture = nullptr;
    _pick_sdl_window = nullptr;
#endif
  }

  CairoSDLWindow::~CairoSDLWindow ()
  {
    if (is_activated)
      impl_deactivate ();
  }

  void
  CairoSDLWindow::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
    double x, y, w, h;
    x = _window->pos_x ()->get_value ();
    y = _window->pos_y ()->get_value ();
    w = _window->width ()->get_value ();
    h = _window->height ()->get_value ();
    const std::string &title = _window->title ()->get_value ();

    //int w = (int) (_window->width ()->get_value ());
    //int h = (int) (_window->height ()->get_value ());

    if(x<=0) x = SDL_WINDOWPOS_UNDEFINED;
    if(y<=0) y = SDL_WINDOWPOS_UNDEFINED;

    _sdl_window = SDL_CreateWindow (title.c_str (), x, y, w, h,
                                    SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (_sdl_window == NULL) {
      std::cerr << "Window could not be created! SDL_Error:" << SDL_GetError () << std::endl;
      return;
    }
    _sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _sdl_renderer = SDL_CreateRenderer (_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _my_cairo_surface = new MyCairoSurface (_window);
    _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);

    //delete _picking_data;
    int stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, w);
    _picking_data = new unsigned char[stride*((int)h)];

#if PICKING_DBG
    _pick_sdl_window = SDL_CreateWindow ("pick", (int) (x + w),
                                        (int) (y), w, h,
                                        SDL_WINDOW_RESIZABLE);
    _pick_sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _pick_sdl_renderer = SDL_CreateRenderer (_pick_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, w, h);
#endif

    SDLMainloop::instance ().add_window (this);

    _picking_view = new CairoPickingView (_window);
    WinImpl::set_picking_view (_picking_view);

    is_activated = true;
  }

  void
  CairoSDLWindow::impl_deactivate ()
  {
    delete _picking_view;
    WinImpl::set_picking_view (nullptr);

    SDLMainloop::instance ().remove_window (this);
    delete _my_cairo_surface;
    if (_sdl_surface)
      SDL_FreeSurface (_sdl_surface);
    if (_sdl_renderer)
      SDL_DestroyRenderer (_sdl_renderer);
    if (_sdl_window) {
      SDL_DestroyWindow (_sdl_window);
    }
    _my_cairo_surface = nullptr;
    _sdl_surface = nullptr;
    _sdl_window = nullptr;
    delete _picking_data;
    _picking_data = nullptr;
    is_activated = false;
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
  }


  static mouse_button
  get_button (Uint8 n)
  {
    mouse_button button_id = BUTTON_LEFT;
    switch (n)
      {
      case SDL_BUTTON_LEFT:
        button_id = BUTTON_LEFT;
        break;
      case SDL_BUTTON_RIGHT:
        button_id = BUTTON_RIGHT;
        break;
      case SDL_BUTTON_MIDDLE:
        button_id = BUTTON_MIDDLE;
        break;
      default:
        button_id = BUTTON_LEFT;
        break;
      }
    return button_id;
  }

  void
  CairoSDLWindow::handle_event (SDL_Event& e)
  {
    bool handled = false;
    switch (e.type)
      {
      case SDL_MOUSEBUTTONDOWN:
        {
          picking_view ()->genericMousePress (e.button.x, e.button.y, get_button (e.button.button));
          handled = true;
          break;
        }
      case SDL_MOUSEBUTTONUP:
        {
          picking_view ()->genericMouseRelease (e.button.x, e.button.y, get_button (e.button.button));
          handled = true;
          break;
        }
      case SDL_MOUSEMOTION:
        {
          picking_view ()->genericMouseMove (e.motion.x, e.motion.y);
          handled = true;
          break;
        }
      case SDL_FINGERDOWN:
        {
          picking_view ()->genericTouchPress (e.tfinger.x, e.tfinger.y, e.tfinger.fingerId, e.tfinger.pressure);
          handled = true;
          break;
        }
      case SDL_FINGERUP:
        {
          picking_view ()->genericTouchRelease (e.tfinger.x, e.tfinger.y, e.tfinger.fingerId, e.tfinger.pressure);
          handled = true;
          break;
        }
      case SDL_FINGERMOTION:
        {
          picking_view ()->genericTouchMove (e.tfinger.x, e.tfinger.y, e.tfinger.fingerId, e.tfinger.pressure);
          handled = true;
          break;
        }
      case SDL_MOUSEWHEEL:
        {
          picking_view ()->genericMouseWheel (e.wheel.x, e.wheel.y);
          handled = true;
          break;
        }
      }

    if(!handled) SDLWindow::handle_event(e);

    GRAPH_EXEC;
  }

  
  void
  CairoSDLWindow::handle_resized(int width, int height)
  {
    SDLWindow::handle_resized(width, height);
    if (_sdl_surface) SDL_FreeSurface (_sdl_surface);
    if (_sdl_texture) SDL_DestroyTexture (_sdl_texture);
    _sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
    _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    delete _picking_data;
    int stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, width);
    _picking_data = new unsigned char[stride*height];

#if PICKING_DBG
    if (_pick_sdl_surface) SDL_FreeSurface (_pick_sdl_surface);
    if (_pick_sdl_texture) SDL_DestroyTexture (_pick_sdl_texture);
    _pick_sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
    _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
    
    redraw();
  }

  void
  CairoSDLWindow::update_hdpi()
  {
    // FIXME should be a connection
    SDLWindow::update_hdpi();
  }


  void
  CairoSDLWindow::redraw ()
  {
    //DBG;
    #if _PERF_TEST
      t1();
    #endif

    cairo_surface_t *drawing_surface, *picking_surface;
    unsigned char *data, *picking_data;
    CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());

    auto dbackend = DisplayBackend::instance();
    dbackend->set_window (_window);
    
    backend->set_picking_view (_picking_view);
    _picking_view->init ();
    drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) _sdl_surface->pixels, CAIRO_FORMAT_ARGB32,
                                                           _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);

    picking_surface = cairo_image_surface_create_for_data ((unsigned char*) _picking_data, CAIRO_FORMAT_ARGB32,
                                                           _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);

    _my_cairo_surface->update (drawing_surface, picking_surface);

    cairo_surface_flush (drawing_surface);
    cairo_surface_flush (picking_surface);
    data = cairo_image_surface_get_data (drawing_surface);
    picking_data = cairo_image_surface_get_data (picking_surface);
    _picking_view->set_data (picking_data, _sdl_surface->w, _sdl_surface->h,
                             cairo_image_surface_get_stride (picking_surface));

    SDL_UpdateTexture (_sdl_texture, NULL, data, _sdl_surface->pitch);
    SDL_RenderCopy (_sdl_renderer, _sdl_texture, nullptr, nullptr);
    SDL_RenderPresent (_sdl_renderer);
#if PICKING_DBG
    SDL_UpdateTexture (_pick_sdl_texture, NULL, picking_data, _pick_sdl_surface->pitch);
    SDL_RenderCopy (_pick_sdl_renderer, _pick_sdl_texture, nullptr, nullptr);
    SDL_RenderPresent (_pick_sdl_renderer);
#endif
    cairo_surface_destroy (drawing_surface);
    cairo_surface_destroy (picking_surface);

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

  void
  CairoSDLWindow::update ()
  {
    SDLMainloop::instance ().wakeup (this);
  }

  // cost-free hack to avoid including xlib.h in X11Window.h header when calling handle_event
  struct __Event
  {
    __Event (SDL_Event& e_) :
        e (e_)
    {
    }
    SDL_Event& e;
  };

}
