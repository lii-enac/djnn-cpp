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

#include "../../display/sdl/sdl_window.h"
#include "my_cairo_surface.h"

namespace djnn {

class CairoGLSDLWindow : public SDLWindow
{
  public:
    CairoGLSDLWindow (Window *win, const std::string& title, double x, double y, double w, double h);
    virtual ~CairoGLSDLWindow ();

    void update () override;
    virtual void redraw () override;

  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;

    // SDLWindow
    virtual void handle_resized(int w, int h) override;

    SDL_GLContext sdl_context() { return _sdl_context; }
    // FIXME better name
    void end_redraw();
    void update_hdpi() override;

  	SDL_GLContext _sdl_context;
    MyCairoSurface *_my_cairo_surface;
    CairoPickingView *_picking_view;

    //SDL_Surface * _sdl_surface;
    
    //SDL_Texture * _sdl_texture;
    //SDL_Surface * _sdl_surface_picking;
    //SDLPickingView * _picking_view;

    int _refresh_rate;
};

}