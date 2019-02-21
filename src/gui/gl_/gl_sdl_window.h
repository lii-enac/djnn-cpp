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

#include "../sdl/sdl_window.h"

namespace djnn {

class GLSDLWindow : public SDLWindow
{
  public:
    GLSDLWindow (Window *win, const std::string &title, double x, double y, double w, double h);
    virtual ~GLSDLWindow ();

    void update () override;
    virtual void redraw () override;

  protected:
    // Process
    void activate () override;
    void deactivate () override;

    // SDLWindow
    virtual void handle_resized(int w, int h) override;

    SDL_GLContext sdl_context() { return _sdl_context; }
    // FIXME better name
    void end_redraw();
    void update_hdpi();

  	SDL_GLContext _sdl_context;
    //SDL_Surface * _sdl_surface;
    
    //SDL_Texture * _sdl_texture;
    //SDL_Surface * _sdl_surface_picking;
    //SDLPickingView * _picking_view;

    int _refresh_rate;
};

}