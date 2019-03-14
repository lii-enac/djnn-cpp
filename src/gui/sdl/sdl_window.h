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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>

 *
 */

#pragma once

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
    SDL_Window * sdl_window() { return _sdl_window; }

    void handle_event(__Event&);
    enum {
      user_event_awake
    };

    virtual void handle_resized(int w, int h);
    virtual void update_hdpi();
    virtual void redraw ();
    
  protected:
    
    djnn::Window* _window;
    SDL_Window *_sdl_window;
    bool is_activated;
  };

}
//#endif
