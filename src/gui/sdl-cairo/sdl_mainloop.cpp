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

#include "../backend.h"

#include "sdl_mainloop.h"
#include "sdl_window.h"

#include "../../core/syshook/syshook.h"
#include "../../core/syshook/main_loop.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn {

    SDLMainloop* SDLMainloop::_instance;
    std::once_flag SDLMainloop::onceFlag;

    SDLMainloop&
    SDLMainloop::instance ()
    {
      std::call_once (SDLMainloop::onceFlag, [] () {
          _instance = new SDLMainloop ();
      });

      return *(_instance);
    }

  SDLMainloop::SDLMainloop()
  : _wakeup_already_triggered(false)
  {
    MainLoop::instance ().set_another_source_wants_to_be_mainloop (this);
  }

  SDLMainloop::~SDLMainloop()
  {
  }

  void
  SDLMainloop::add_window (SDLWindow* win)
  {
    _windows[SDL_GetWindowID(win->sdl_window())] = win;
  }
    
    void
    SDLMainloop::remove_window (SDLWindow* win)
    {
      _windows.erase(SDL_GetWindowID(win->sdl_window()));
    }

#ifdef __EMSCRIPTEN__
    std::function<void()> loop;
  //typedef void(*mainloop_fun_t) (void);
    //mainloop_fun_t loop;

  static void main_loop() { loop(); }
#endif

  void
    //SDLMainloop::activate_from_mainloop ()
    SDLMainloop::run ()
    {
      //set_please_stop (false);
#ifdef __EMSCRIPTEN__
      loop = [&]{sdl_run();};
      emscripten_set_main_loop(main_loop, 0, true);
#else
      sdl_run();
#endif
    }

    void
    SDLMainloop::wakeup (SDLWindow * requestingWin)
    {
      if(_wakeup_already_triggered) return;
      //DBG;
      // push a dummy event into the queue so that the event loop has a chance to stop
      SDL_Event e;
      e.type = SDL_USEREVENT;
      e.user.code = SDLWindow::user_event_awake;
      if (requestingWin)
        e.window.windowID = SDL_GetWindowID(requestingWin->sdl_window());
      //SDL_PeepEvents (&e, 1, SDL_ADDEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
      SDL_PushEvent(&e);
      _wakeup_already_triggered = true;
    }

    void
  SDLMainloop::please_stop ()
  {
      ExternalSource::please_stop();
      wakeup(nullptr); // wakeup
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
  SDLMainloop::sdl_run ()
  {
    // wait for official mainloop to finish initing
    //djnn::get_exclusive_access (DBG_GET);
    //djnn::release_exclusive_access (DBG_REL);

    launch_mutex_unlock();
    
    #if 1
      while (!get_please_stop ()) {
        SDL_Event e;
        _wakeup_already_triggered=false;
        SDL_WaitEvent (&e); // blocking call
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        __Event e_ (e); // cost-free hack to avoid including xlib.h in X11Window.h header when calling handle_event

  #if 1 // slightly more efficient loop: handle all events in the queue
        if (!get_please_stop ()) { handle_event (e_); }
        const unsigned int max_events = 10;
        SDL_Event es[max_events];
        int pending = SDL_PeepEvents (es, max_events, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
        //std::cerr << "pending events: " << pending << __FL__;
        if (pending && !get_please_stop ()) {
             SDL_Event &e = es[max_events-pending];
             bool redraw_awake=false;
             SDL_Event redraw_event;
           do {
             SDL_WaitEvent (&e); // should be non-blocking call, since there are pending events
             if(e.type==SDL_USEREVENT && e.user.code==SDLWindow::user_event_awake) {
              redraw_awake=true;
              redraw_event=e;
              continue;
             }
             __Event e_ (e);
             handle_event (e_);
             if (get_please_stop ()) break;
           } while (--pending);

           if(redraw_awake) {
             __Event e_ (redraw_event);
             handle_event (e_);
           }
        } else {
          //djnn::release_exclusive_access (DBG_REL);
          //this_thread::yield();
        }
  #else // simple loop: handle one event at a time, might be costly to acquire mutex each time
        if (!get_please_stop ()) handle_event (e_);
  #endif
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
  #endif

      _windows.clear ();
      set_please_stop (false);
      SDL_Quit();
  }

  void
    SDLMainloop::handle_event (__Event& e_)
    {
      SDL_Event& e=e_.e;
      switch (e.type)
      {
        //case Expose:
        //case EnterNotify:
        //case SDL_KEYDOWN:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEWHEEL:
        case SDL_WINDOWEVENT:
        case SDL_USEREVENT: // redraw
        {
          SDLWindow * w = _windows[e.window.windowID];
          if(w) w->handle_event(e_);
          break;
        }
        case SDL_QUIT:
            std::cout << "Shutting down now!!!" << std::endl;
            please_stop ();
            break;
      }
    }

}

//#endif
