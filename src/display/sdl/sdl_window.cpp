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

#include "../sdl/sdl_mainloop.h"
#include "../../gui/gui-priv.h"
#include "sdl_window.h"
//#include "../backend.h"
#include "../display.h"

#include "../../core/syshook/syshook.h"
#include "../../core/execution/graph.h"
#include "../../core/syshook/main_loop.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "

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

  SDLWindow::SDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
      _window (win), _sdl_window (nullptr), _cursor (nullptr), _cursor_surface (nullptr), is_activated (false)
  {
    SDL_AddEventWatch (resizingEventWatcher, this);
  }

  void
  SDLWindow::set_cursor (const string &path, int hotX, int hotY)
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

  void
  SDLWindow::handle_event (SDL_Event& e)
  {
    switch (e.type)
      {
      case SDL_KEYDOWN:
        break;
      
      case SDL_USEREVENT:
        {
          if (e.user.code == user_event_awake) {
            // awake
#if 0
            //std::cerr << "SDL awake" << __FL__;
            static Uint32 lastTick=0;
            Uint32 tick = SDL_GetTicks();
            float _refresh_rate=120.f;
            if(tick - lastTick > 1000/_refresh_rate) {
              lastTick = tick;
              redraw();
            }
#else
            redraw ();
#endif
          }
          //redraw(_window);
          break;
        }
        break;
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

    //GRAPH_EXEC;
  }

}