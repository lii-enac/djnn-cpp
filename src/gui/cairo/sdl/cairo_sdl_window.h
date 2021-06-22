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

#include "display/sdl/sdl_window.h"
#include "gui/cairo/my_cairo_surface.h"

namespace djnn {

  class CairoSDLWindow : public SDLWindow
  {
  public:
    CairoSDLWindow (Window *win, const string& title, double x, double y, double w, double h);
    virtual ~CairoSDLWindow ();

    void update () override;

    // SDLWindow API
    virtual void handle_event(SDL_Event&) override;
    virtual void handle_resized(int w, int h) override;
    virtual void update_hidpi() override;
    virtual void redraw () override;
    virtual void perform_screenshot (const string& path) override;
    
  protected:
    // FatProcess
    void impl_activate () override;
    void impl_deactivate () override;

  private:
    SDL_Surface * _sdl_surface;
    SDL_Renderer *_sdl_renderer;
    SDL_Texture *_sdl_texture;
    unsigned char * _picking_data;
    MyCairoSurface *_my_cairo_surface;
    CairoPickingView *_picking_view;

    bool _perform_screenshot;
    string _screenshot_path;
    
#if PICKING_DBG
    SDL_Window *_pick_sdl_window;
    SDL_Renderer *_pick_sdl_renderer;
    SDL_Texture *_pick_sdl_texture;
    SDL_Surface * _pick_sdl_surface;
#endif
  };
}
