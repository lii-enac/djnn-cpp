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

#include <SDL.h>

#include "core/ontology/process.h"
#include "display/window.h"
#include "exec_env/external_source.h"

#define PICKING_DBG 0

namespace djnn {

class SDLPickingView;

class SDLWindow : public WinImpl
{
  public:
    SDLWindow (Window* win, const string& title, double x, double y, double w, double h);
    virtual ~SDLWindow ();
    SDL_Window* sdl_window () { return _sdl_window; }
    void        set_cursor (const string& path, int hotX, int hotY) override;
    void        set_cursor (int cursor_shape) override{};
    enum
    {
        user_event_awake,
        user_event_geometry
    };

    virtual void handle_event (SDL_Event&);
    virtual void handle_resized (int w, int h);
    virtual void update_hidpi ();
    void         trigger_redraw ();
    virtual void redraw ();

    virtual void set_opacity (double opacity) override;
    virtual void update_geometry () override;
    virtual void update_geometry_for_good ();

  protected:
    djnn::Window* _window;
    SDL_Window*   _sdl_window;
    SDL_Cursor*   _cursor;
    SDL_Surface*  _cursor_surface;
    bool          is_activated;
    float         _refresh_rate;
};

} // namespace djnn
