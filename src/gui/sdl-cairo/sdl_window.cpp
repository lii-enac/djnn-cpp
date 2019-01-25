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

#include "sdl_mainloop.h"
#include "sdl_window.h"
#include "../cairo/my_cairo_surface.h"
#include "../backend.h"

#include "../../core/syshook/syshook.h"
#include "../../core/execution/graph.h"
#include "../../core/syshook/main_loop.h"

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include <iostream>

#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "
#define _PERF_TEST 0
#if _PERF_TEST
#include "../../core/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{

  int mouse_tracking; // unused but important for Qt

  SDLWindow::SDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
      _window (win), _sdl_window (nullptr), _sdl_renderer (nullptr), _sdl_texture (nullptr), _sdl_surface (nullptr), _my_cairo_surface (
          nullptr), is_activated (false)
  {
    _picking_view = new CairoPickingView (win);
#if PICKING_DBG
    _pick_sdl_renderer = nullptr;
    _pick_sdl_surface = nullptr;
    _pick_sdl_texture = nullptr;
    _pick_sdl_window = nullptr;
#endif
  }

  SDLWindow::~SDLWindow ()
  {
    if (is_activated)
      deactivate ();
  }

  void
  SDLWindow::activate ()
  {
    int w = (int) (_window->width ()->get_value ());
    int h = (int) (_window->height ()->get_value ());
    _sdl_window = SDL_CreateWindow (_window->title ()->get_value ().c_str (), (int) (_window->pos_x ()->get_value ()),
                                    (int) (_window->pos_y ()->get_value ()), w, h,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (_sdl_window == NULL) {
      std::cerr << "Window could not be created! SDL_Error:" << SDL_GetError () << std::endl;
      return;
    }
    _sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _sdl_renderer = SDL_CreateRenderer (_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _my_cairo_surface = new MyCairoSurface (_window);
    _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);

#if PICKING_DBG
    _pick_sdl_window = SDL_CreateWindow ("pick", (int) (_window->pos_x ()->get_value () + w),
                                        (int) (_window->pos_y ()->get_value ()), w, h,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    _pick_sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _pick_sdl_renderer = SDL_CreateRenderer (_pick_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, w, h);
#endif
    SDLMainloop::instance ().add_window (this);
    is_activated = true;
  }

  void
  SDLWindow::deactivate ()
  {
    SDLMainloop::instance ().remove_window (this);
    delete _my_cairo_surface;
    if (_sdl_surface)
      SDL_FreeSurface (_sdl_surface);
    if (_sdl_renderer)
      SDL_DestroyRenderer (_sdl_renderer);
    if (_sdl_window)
      SDL_DestroyWindow (_sdl_window);
    _my_cairo_surface = nullptr;
    _sdl_surface = nullptr;
    _sdl_window = nullptr;
    is_activated = false;
  }

  void
  SDLWindow::redraw ()
  {
    #if _PERF_TEST
      t1();
    #endif


    cairo_surface_t *drawing_surface, *picking_surface;
    unsigned char *data, *picking_data;
    CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());

    backend->set_window (_window);
    backend->set_picking_view (_picking_view);
    _picking_view->init ();
    drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) _sdl_surface->pixels, CAIRO_FORMAT_ARGB32,
                                                           _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);
    picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, _sdl_surface->w, _sdl_surface->h);

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
  SDLWindow::update ()
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

  void
  SDLWindow::handle_event (__Event& e_)
{
  SDL_Event& e=e_.e;
  switch (e.type)
  {
    case SDL_KEYDOWN:
    break;
    case SDL_MOUSEBUTTONDOWN: {
      _picking_view->genericMousePress (e.button.x,e.button.y,e.button.button);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      _picking_view->genericMouseRelease (e.button.x,e.button.y,e.button.button);
      break;
    }
    case SDL_MOUSEMOTION: {
      _picking_view->genericMouseMove (e.motion.x,e.motion.y);
      break;
    }
    case SDL_MOUSEWHEEL: {
      _picking_view->genericMouseWheel (e.wheel.x,e.wheel.y);
      break;
    }
    case SDL_USEREVENT: {
      if(e.user.code==user_event_awake) {
        // awake
#if 0
      //std::cerr << "SDL awake" << __FL__;
      static Uint32 lastTick=0;
      Uint32 tick = SDL_GetTicks();
      if(tick - lastTick > 1000/_refresh_rate) {
        lastTick = tick;
        redraw();
      }
#else
      redraw();
#endif
    }
    //redraw(_window);
    break;
  }
  break;
  case SDL_WINDOWEVENT: {
    switch (e.window.event) {
      case SDL_WINDOWEVENT_SHOWN:
      case SDL_WINDOWEVENT_EXPOSED: {
        redraw ();
        break;
      }
      case SDL_WINDOWEVENT_MOVED: {
        int x = e.window.data1;
        int y = e.window.data2;
        _window->pos_x ()->set_value (x, true);
        _window->pos_y ()->set_value (y, true);
        break;
      }
      case SDL_WINDOWEVENT_SIZE_CHANGED: {
        int width = e.window.data1;
        int height = e.window.data2;
        if (width == _window->width ()->get_value () && height == _window->height ()->get_value())
        return;
        _window->width ()->set_value (width, true);
        _window->height ()->set_value (height, true);
        if (_sdl_surface) SDL_FreeSurface (_sdl_surface);
        if (_sdl_texture) SDL_DestroyTexture (_sdl_texture);
        _sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
        _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#if PICKING_DBG
        if (_pick_sdl_surface) SDL_FreeSurface (_pick_sdl_surface);
        if (_pick_sdl_texture) SDL_DestroyTexture (_pick_sdl_texture);
        _pick_sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
        _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
        redraw ();

        break;
      }
      case SDL_WINDOWEVENT_CLOSE: {
        e.type = SDL_QUIT;
        SDL_PushEvent (&e);
        break;
      }
      default:
      break;
    }
  }
}
Graph::instance ().exec ();
}

}
