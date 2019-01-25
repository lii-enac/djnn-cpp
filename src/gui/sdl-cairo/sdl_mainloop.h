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

#include "../../core/syshook/external_source.h"

#include <SDL2/SDL.h>
#include <memory>
#include <map>
#include <mutex>

namespace djnn {

  class SDLWindow;

  // cost-free hack to avoid including xlib.h in X11Window.h header when calling handle_event
    struct __Event;

  class SDLMainloop : public ExternalSource {
    public:
      static SDLMainloop& instance ();
      ~SDLMainloop();

      void add_window (SDLWindow* win);
      void remove_window (SDLWindow* win);

      // ExternalSource
      virtual void please_stop () override;
      virtual void wakeup(SDLWindow * requestingWin);
    
    protected:
      // ExternalSource
      //virtual void activate_from_mainloop () override;
      virtual void run () override;

    private:
      void sdl_run () ;
      void handle_event(__Event&);

      std::map<Uint32, SDLWindow*> _windows;

      SDLMainloop();
      static std::shared_ptr<SDLMainloop> _instance;
      static std::once_flag onceFlag;

      bool _wakeup_already_triggered;

  };

}
