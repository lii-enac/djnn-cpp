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
 *
 */

#pragma once

#include <SDL.h>
#include <memory>
#include <mutex> // std::call_once

#include "exec_env/external_source.h"

namespace djnn {

class SDLWindow;

class SDLMainloop : public ExternalSource {
  public:
    static SDLMainloop& instance ();
    ~SDLMainloop ();

    void add_window (SDLWindow* win);
    void remove_window (SDLWindow* win);

    // ExternalSource
    virtual void please_stop () override;
    virtual void wakeup (SDLWindow* requestingWin);

    void sdl_run_coop ();

  protected:
    // ExternalSource
    virtual void run () override;

  private:
    void sdl_run ();

    void handle_events (SDL_Event&);
    void handle_single_event (SDL_Event&);

    map<Uint32, SDLWindow*> _windows;

    SDLMainloop ();
    static SDLMainloop*   _instance;
    static std::once_flag onceFlag;

    std::atomic<bool> _wakeup_already_triggered;
};

} // namespace djnn
