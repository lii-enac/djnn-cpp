#include "cairo_sdl_window.h"
#include "../sdl/sdl_mainloop.h"

#include <iostream>
#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "

namespace djnn
{
  CairoSDLWindow::CairoSDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
  SDLWindow(win, title, x,y,w,h),
  _sdl_surface (nullptr), _sdl_renderer (nullptr), _sdl_texture (nullptr), _my_cairo_surface (nullptr)
  {
    _picking_view = new CairoPickingView (win);
    WinImpl::set_picking_view (_picking_view);

#if PICKING_DBG
    _pick_sdl_surface = nullptr;
    _pick_sdl_renderer = nullptr;
    _pick_sdl_texture = nullptr;
    _pick_sdl_window = nullptr;
#endif
  }

  CairoSDLWindow::~CairoSDLWindow ()
  {
    if (is_activated)
      deactivate (); 
  }

  void
  CairoSDLWindow::activate ()
  {
    int w = (int) (_window->width ()->get_value ());
    int h = (int) (_window->height ()->get_value ());
    _sdl_window = SDL_CreateWindow (_window->title ()->get_value ().c_str (), (int) (_window->pos_x ()->get_value ()),
                                    (int) (_window->pos_y ()->get_value ()), w, h,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (_sdl_window == NULL) {
      std::cerr << "Window could not be created! SDL_Error:" << SDL_GetError () << std::endl;
      return;
    }
    _sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _sdl_renderer = SDL_CreateRenderer (_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _my_cairo_surface = new MyCairoSurface (_window);
    _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);

#if PICKING_DBG
    _pick_sdl_window = SDL_CreateWindow ("pick", (int) (_window->pos_x ()->get_value () + w),
                                        (int) (_window->pos_y ()->get_value ()), w, h,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    _pick_sdl_surface = SDL_CreateRGBSurface (0, w, h, 32, 0, 0, 0, 0);
    _pick_sdl_renderer = SDL_CreateRenderer (_pick_sdl_window, -1, SDL_RENDERER_TARGETTEXTURE);
    _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, w, h);
#endif
    SDLMainloop::instance ().add_window (this);
    is_activated = true;
  }

  void
  CairoSDLWindow::deactivate ()
  {
    SDLMainloop::instance ().remove_window (this);
    delete _my_cairo_surface;
    if (_sdl_surface)
      SDL_FreeSurface (_sdl_surface);
    if (_sdl_renderer)
      SDL_DestroyRenderer (_sdl_renderer);
    if (_sdl_window)
      SDL_DestroyWindow (_sdl_window);
    _my_cairo_surface = nullptr;
    _sdl_surface = nullptr;
    _sdl_window = nullptr;
    is_activated = false;
  }

  void
  CairoSDLWindow::redraw ()
  {
    #if _PERF_TEST
      t1();
    #endif

    cairo_surface_t *drawing_surface, *picking_surface;
    unsigned char *data, *picking_data;
    CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());

    backend->set_window (_window);
    backend->set_picking_view (_picking_view);
    _picking_view->init ();

    drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) _sdl_surface->pixels, CAIRO_FORMAT_ARGB32,
                                                          _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);
    picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, _sdl_surface->w, _sdl_surface->h);

    // cairo gl
    // SDL_SysWMinfo info;
    // SDL_GetWindowWMInfo(_sdl_window, &info);
    // // https://stackoverflow.com/questions/39476501/how-to-obtain-the-glxcontext-in-sdl
    // cairo_device_t* device = cairo_glx_device_create (&info.x11.Display, (GLXContext) _sdl_context);
    // drawing_surface = cairo_gl_surface_create (device, CAIRO_CONTENT_COLOR_ALPHA, _sdl_surface->w, _sdl_surface->h);
    // picking_surface = cairo_gl_surface_create (device, CAIRO_CONTENT_COLOR, _sdl_surface->w, _sdl_surface->h);

    _my_cairo_surface->update (drawing_surface, picking_surface);

    cairo_surface_flush (drawing_surface);
    cairo_surface_flush (picking_surface);

    data = cairo_image_surface_get_data (drawing_surface);
    picking_data = cairo_image_surface_get_data (picking_surface);
    _picking_view->set_data (picking_data, _sdl_surface->w, _sdl_surface->h,
                             cairo_image_surface_get_stride (picking_surface));

    SDL_UpdateTexture (_sdl_texture, NULL, data, _sdl_surface->pitch);
    SDL_RenderCopy (_sdl_renderer, _sdl_texture, nullptr, nullptr);
    SDL_RenderPresent (_sdl_renderer);
#if PICKING_DBG
    SDL_UpdateTexture (_pick_sdl_texture, NULL, picking_data, _pick_sdl_surface->pitch);
    SDL_RenderCopy (_pick_sdl_renderer, _pick_sdl_texture, nullptr, nullptr);
    SDL_RenderPresent (_pick_sdl_renderer);
#endif
    cairo_surface_destroy (drawing_surface);
    cairo_surface_destroy (picking_surface);


#if _PERF_TEST
      // print in RED
      cerr << "\033[1;31m";
      double time = t2 ("DRAW : ");
      draw_counter = draw_counter + 1;
      draw_total = draw_total + time ;
      draw_average = draw_total / draw_counter;
      cerr << "DRAW : " << draw_counter << " - avg: " << draw_average << endl;
      cerr << "\033[0m"  << endl;
#endif
  }

  void
  CairoSDLWindow::update ()
  {
    SDLMainloop::instance ().wakeup (this);
  }

  void
  CairoSDLWindow::handle_resized (int width, int height)
  {
    if (_sdl_surface) SDL_FreeSurface (_sdl_surface);
    if (_sdl_texture) SDL_DestroyTexture (_sdl_texture);
    _sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
    _sdl_texture = SDL_CreateTexture (_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#if PICKING_DBG
    if (_pick_sdl_surface) SDL_FreeSurface (_pick_sdl_surface);
    if (_pick_sdl_texture) SDL_DestroyTexture (_pick_sdl_texture);
    _pick_sdl_surface = SDL_CreateRGBSurface (0, width, height, 32, 0, 0, 0, 0);
    _pick_sdl_texture = SDL_CreateTexture (_pick_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
#endif
  }
}
