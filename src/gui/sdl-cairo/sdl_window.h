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

#pragma once

#include "../cairo/my_cairo_surface.h"
#include "../backend.h"
#include "../window.h"
#include "../../core/tree/process.h"
#include "../../core/syshook/external_source.h"

#include <string>

#include <SDL2/SDL.h>

#define PICKING_DBG 0

namespace djnn {

  class SDLPickingView;

  // cost-free hack to avoid including xlib.h in X11Window.h header when calling handle_event
  struct __Event;

  class SDLWindow : public WinImpl
  {
  public:
    SDLWindow (Window *win, const std::string &title, double x, double y, double w, double h);
    virtual ~SDLWindow ();

    void update () override;
    void redraw ();

    SDL_Window * sdl_window() { return _sdl_window; }

    void handle_event(__Event&);
    enum {
      user_event_awake
    };
    
  protected:
    // Process
    void activate () override;
    void deactivate () override;

  private:
    djnn::Window* _window;
    SDL_Window *_sdl_window;
    SDL_Renderer *_sdl_renderer;
    SDL_Texture *_sdl_texture;
    SDL_Surface * _sdl_surface;
    MyCairoSurface *_my_cairo_surface;
    CairoPickingView *_picking_view;
    bool is_activated;
#if PICKING_DBG
    SDL_Window *_pick_sdl_window;
    SDL_Renderer *_pick_sdl_renderer;
    SDL_Texture *_pick_sdl_texture;
    SDL_Surface * _pick_sdl_surface;
#endif
  };

}
//#endif
