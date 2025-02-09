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

#include <SDL3/SDL_video.h>

// #include "sdl_mainloop.h"
#include "sdl_window.h"

#include "core/control/blank.h"
#include "core/control/spike.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/ref_property.h"
#include "core/property/text_property.h"
#include "display/display-dev.h"  // GenericKeyboard
#include "display/display-priv.h" // GUIKeyboard
#include "display/display.h"
// #include "core/property/bool_property.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"
#include "core/utils/remotery.h"
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
// #include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/utils/iostream.h"
#include "utils/debug.h"

// #define attr(a) #a ":" << a << " "

namespace djnn {

// https://stackoverflow.com/questions/32294913/getting-contiunous-window-resize-event-in-sdl-2
static bool
resizingEventWatcher (void* data, SDL_Event* event)
{
    if (event->type == /*SDL_WINDOWEVENT && event->window.event ==*/ SDL_EVENT_WINDOW_RESIZED) {
        SDL_Window* win     = SDL_GetWindowFromID (event->window.windowID);
        SDLWindow*  djnnwin = (SDLWindow*)data;
        if (win == djnnwin->sdl_window ()) {
            // printf("resizing.....\n");
            int w, h;
            SDL_GetWindowSize (win, &w, &h);
            djnnwin->handle_resized (w, h);
        }
    }
    return true;
}

SDLWindow::SDLWindow (djnn::Window* win, const djnnstl::string& title, double x, double y, double w, double h)
    : _window (win), _sdl_window (nullptr), _cursor (nullptr), _cursor_surface (nullptr), is_activated (false)
{
    SDL_AddEventWatch (resizingEventWatcher, this);
}

void
SDLWindow::set_cursor (const djnnstl::string& path, int hotX, int hotY)
{
    if (_cursor)
        SDL_DestroyCursor (_cursor);
    if (_cursor_surface)
        SDL_DestroySurface (_cursor_surface);
#if 0 // use png or jpg loading and get rid of SDL_image...
    _cursor_surface = IMG_Load (path.c_str ());
#else
    std::cerr << "djnn: cannot set cursor with SDL without SDL_image" << __FL__;
    _cursor_surface = nullptr;
#endif
    if (!_cursor_surface)
        return;
    _cursor = SDL_CreateColorCursor (_cursor_surface, hotX, hotY);
    if (!_cursor) {
        SDL_DestroySurface (_cursor_surface);
        return;
    }
    SDL_SetCursor (_cursor);
}

SDLWindow::~SDLWindow ()
{
    if (_cursor_surface)
        SDL_DestroySurface (_cursor_surface);
    if (_cursor)
        SDL_DestroyCursor (_cursor);
}

void
SDLWindow::handle_resized (int w, int h)
{
    _window->width ()->set_value (w, true);
    _window->height ()->set_value (h, true);
    // redraw(); // delegated to inheriting *SDLWindows
}

void
SDLWindow::update_hidpi ()
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
bool
SDLWindow::handle_event (SDL_Event& e)
{
    // std::cerr << e.type << __FL__;
    bool exec = false;
    switch (e.type) {
    case SDL_EVENT_QUIT:
        DBG;
        break;
    case SDL_EVENT_TEXT_INPUT:
        _window->key_pressed ()->set_value ((int)e.key.key, 1); // FIXME_SDL
        _window->key_pressed_text ()->set_value (e.text.text, 1);
        exec = true;
        break;
    case SDL_EVENT_KEY_DOWN:
        _window->key_pressed ()->set_value ((int)e.key.key, 1); // FIXME_SDL
        ((GUIKeyboard*)GenericKeyboard)->key_pressed ()->set_value ((int)e.key.key, 1);
        exec = true;
        break;
    case SDL_EVENT_KEY_UP:
        _window->key_released ()->set_value ((int)e.key.key, 1); // FIXME_SDL
        ((GUIKeyboard*)GenericKeyboard)->key_released ()->set_value ((int)e.key.key, 1);
        exec = true;
        break;

    case SDL_EVENT_USER: {
        switch (e.user.code) {
        case user_event_awake: {
            // std::cerr << "sdl window wakeup" << __FL__;
            rmt_BeginCPUSample (trigger_redraw, RMTSF_Aggregate);
            trigger_redraw ();
            rmt_EndCPUSample ();
            exec = true;
            break;
        }
        case user_event_geometry: {
            update_geometry_for_good ();
            exec = true;
            break;
        } break;
        }
        break;
    }
    //case SDL_WINDOWEVENT: {
    //    switch (e.window.event) {
    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_EXPOSED: {
        trigger_redraw ();
        exec = true;
        break;
    }
    case SDL_EVENT_WINDOW_MOVED: {
        int x = e.window.data1;
        int y = e.window.data2;
        _window->pos_x ()->set_value (x, true);
        _window->pos_y ()->set_value (y, true);
        update_hidpi ();
        exec = true;
        break;
    }
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
        auto scale = _window->hidpi_scale ()->get_value (); // SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(_sdl_window));
        int width  = e.window.data1 / scale;
        int height = e.window.data2 / scale;
        //std::cerr << "******* " << scale << " " << width << " " << _window->width ()->get_value () << __FL__; // SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(_sdl_window))
        
        if (width == _window->width ()->get_value () && height == _window->height ()->get_value ()) {
            // do nothing
        } else {
            //_window->width ()->set_value (width, true);
            //_window->height ()->set_value (height, true);
            update_hidpi ();
            handle_resized (width, height);
            exec = true;
            // redraw ();
        }
        break;
    }
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
        // e.type = SDL_EVENT_QUIT;
        // SDL_PushEvent (&e);
        _window->close ()->activate ();
        exec = true;
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        SDL_StartTextInput (_sdl_window);
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        SDL_StopTextInput (_sdl_window);
        break;
    default:
        break;
    }
    //}
    //}
    return exec;
}

void
SDLWindow::trigger_redraw ()
{
    rmt_BeginCPUSample (trigger_redraw, RMTSF_None);
#if 1
    redraw ();
    _window->refreshed ()->notify_activation ();
#else
    static Uint32 lastTick = 0;
    Uint32        tick     = SDL_GetTicks ();
    if ((tick - lastTick) > 1000 / _refresh_rate) {
        lastTick = tick;
        redraw ();
        _window->refreshed ()->notify_activation ();
    }
#endif
    rmt_EndCPUSample ();
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
    e.type            = SDL_EVENT_USER;
    e.user.code       = SDLWindow::user_event_geometry;
    e.window.windowID = SDL_GetWindowID (_sdl_window);
    SDL_PushEvent (&e);
}

void
SDLWindow::update_geometry_for_good ()
{
    auto x = (int)_window->pos_x ()->get_value ();
    auto y = (int)_window->pos_y ()->get_value ();
    auto w = (int)_window->width ()->get_value ();
    auto h = (int)_window->height ()->get_value ();

    SDL_SetWindowPosition (_sdl_window, x, y);
    SDL_SetWindowSize (_sdl_window, w, h);
}

} // namespace djnn
