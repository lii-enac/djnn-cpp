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

//#include "sdl_mainloop.h"
#include "sdl_window.h"
#include "display/display.h"

#include "exec_env/global_mutex.h"
#include "core/core-dev.h" // graph add/remove edge
#include "exec_env/main_loop.h"

#include "core/utils/ext/remotery/Remotery.h"
#include "core/utils/error.h"

#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

//#define attr(a) #a ":" << a << " "

namespace djnn
{

  // https://stackoverflow.com/questions/32294913/getting-contiunous-window-resize-event-in-sdl-2
  static int
  resizingEventWatcher (void* data, SDL_Event* event)
  {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_Window* win = SDL_GetWindowFromID (event->window.windowID);
      SDLWindow * djnnwin = (SDLWindow*) data;
      if (win == djnnwin->sdl_window ()) {
        //printf("resizing.....\n");
        int w, h;
        SDL_GetWindowSize (win, &w, &h);
        djnnwin->handle_resized (w, h);
      }
    }
    return 0;
  }

  SDLWindow::SDLWindow (djnn::Window* win, const std::string& title, double x, double y, double w, double h) :
      _window (win), _sdl_window (nullptr), _cursor (nullptr), _cursor_surface (nullptr), is_activated (false)
  {
    SDL_AddEventWatch (resizingEventWatcher, this);
  }

  void
  SDLWindow::set_cursor (const std::string& path, int hotX, int hotY)
  {
    if (_cursor)
      SDL_FreeCursor (_cursor);
    if (_cursor_surface)
      SDL_FreeSurface (_cursor_surface);
    _cursor_surface = IMG_Load (path.c_str ());
    if (!_cursor_surface)
      return;
    _cursor = SDL_CreateColorCursor (_cursor_surface, hotX, hotY);
    if (!_cursor) {
      SDL_FreeSurface (_cursor_surface);
      return;
    }
    SDL_SetCursor (_cursor);
  }

  SDLWindow::~SDLWindow ()
  {
    if (_cursor_surface)
      SDL_FreeSurface (_cursor_surface);
    if (_cursor)
      SDL_FreeCursor (_cursor);
  }

  void
  SDLWindow::handle_resized (int w, int h)
  {
    _window->width ()->set_value (w, true);
    _window->height ()->set_value (h, true);
    //redraw(); // delegated to inheriting *SDLWindows
  }

  void
  SDLWindow::update_hdpi ()
  {
  }

  void
  SDLWindow::redraw ()
  {
  }

#if 0
  static mouse_button
  get_button (Uint8 n)
  {
    mouse_button button_id = BUTTON_LEFT;
    switch (n)
      {
      case SDL_BUTTON_LEFT:
        button_id = BUTTON_LEFT;
        break;
      case SDL_BUTTON_RIGHT:
        button_id = BUTTON_RIGHT;
        break;
      case SDL_BUTTON_MIDDLE:
        button_id = BUTTON_MIDDLE;
        break;
      default:
        button_id = BUTTON_LEFT;
      }
    return button_id;
  }
#endif
  void
  SDLWindow::handle_event (SDL_Event& e)
  {
    //std::cerr << e.type << __FL__;
    switch (e.type)
      {
      case SDL_TEXTINPUT:
        _window->key_pressed ()->set_value (e.key.keysym.sym, 1);
        _window->key_pressed_text ()->set_value (e.text.text, 1);
        break;
      case SDL_KEYDOWN:
      //   //DBG;
      //   _window->key_pressed ()->set_value (e.key.keysym.sym, 1);
      //   //if (!(event->key() >= 0x1000000 && event->key() <= 0x01020001)) {
      //     _window->key_pressed_text ()->set_value (SDL_GetKeyName(e.key.keysym.sym), 1);
      //   //}
      //   //redraw ();
      break;
      
      case SDL_USEREVENT:
        {
          switch (e.user.code) {
            case user_event_awake: {
              rmt_BeginCPUSample(SDL_USEREVENT, 0);
              // awake
              #if 1
              //std::cerr << "SDL awake" << __FL__;
              static Uint32 lastTick=0;
              Uint32 tick = SDL_GetTicks();
              //float _refresh_rate=120.f;
              //DBG;
              //std::cerr << (tick - lastTick) << " " << 1000/_refresh_rate <<  __FL__;
              if( (tick - lastTick) > 1000/_refresh_rate) {
                //DBG;
                lastTick = tick;
                //rmt_BeginCPUSample(redraw, 0);
                redraw();
                _window->refreshed ()->notify_activation ();
                //rmt_EndCPUSample();
              } //else DBG;
              #else
              redraw ();
              #endif
              rmt_EndCPUSample();
              break;
            }
            case user_event_geometry: {
              update_geometry_for_good ();
              break;
            }
          break;
        }
        break;
      }
      case SDL_WINDOWEVENT:
        {
          switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_EXPOSED:
              {
                redraw ();
                break;
              }
            case SDL_WINDOWEVENT_MOVED:
              {
                int x = e.window.data1;
                int y = e.window.data2;
                _window->pos_x ()->set_value (x, true);
                _window->pos_y ()->set_value (y, true);
                update_hdpi ();
                break;
              }
            case SDL_WINDOWEVENT_SIZE_CHANGED:
              {
                int width = e.window.data1;
                int height = e.window.data2;
                if (width == _window->width ()->get_value () && height == _window->height ()->get_value ())
                  return;
                //_window->width ()->set_value (width, true);
                //_window->height ()->set_value (height, true);
                update_hdpi ();
                handle_resized (width, height);
                //redraw ();

                break;
              }
            case SDL_WINDOWEVENT_CLOSE:
              {
                e.type = SDL_QUIT;
                SDL_PushEvent (&e);
                break;
              }
            default:
              break;
            }
        }
      }
  }

  void
  SDLWindow::set_opacity (double opacity)
  {
    SDL_SetWindowOpacity (_sdl_window, opacity);
  }

  void
  SDLWindow::update_geometry ()
  {
    // macOS: should be done in GUI thread...
    SDL_Event e;
    e.type = SDL_USEREVENT;
    e.user.code = SDLWindow::user_event_geometry;
    e.window.windowID = SDL_GetWindowID(_sdl_window);
    SDL_PushEvent(&e);
  }

  void
  SDLWindow::update_geometry_for_good ()
  {
    auto x = (int) _window->pos_x()->get_value ();
    auto y = (int) _window->pos_y()->get_value ();
    auto w = (int) _window->width()->get_value ();
    auto h = (int) _window->height()->get_value ();

    SDL_SetWindowPosition (_sdl_window, x, y);
    SDL_SetWindowSize (_sdl_window, w, h);
  }

}
