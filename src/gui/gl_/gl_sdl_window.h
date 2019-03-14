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

class GLSDLPickingView;

class GLSDLWindow : public SDLWindow
{
  public:
    GLSDLWindow (Window *win, const std::string &title, double x, double y, double w, double h);
    virtual ~GLSDLWindow ();

   // SDLWindow API
    virtual void handle_resized(int w, int h) override;
    virtual void update_hdpi() override;
    virtual void redraw () override;

    void end_redraw(); // useless?
    void update () override;
  protected:
    // Process API
    void activate () override;
    void deactivate () override;

    SDL_GLContext sdl_context() { return _sdl_context; }
    
  	SDL_GLContext _sdl_context;
    GLSDLPickingView * _picking_view;

    int _refresh_rate;
};

}