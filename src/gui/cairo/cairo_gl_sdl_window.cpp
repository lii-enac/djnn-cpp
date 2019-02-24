#include "cairo_gl_sdl_window.h"
#include "../sdl/sdl_mainloop.h"

// cairo gl
#include <SDL2/SDL_syswm.h>
#include <cairo/cairo-gl.h>


//#include "opengl.h"
//#include "gl_dbg.h"

#include <iostream>
#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "


#define _PERF_TEST 0
#if _PERF_TEST
#include "../../core/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn {

  CairoGLSDLWindow::CairoGLSDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
  SDLWindow(win, title, x,y,w,h),
  _my_cairo_surface (nullptr)
  {
    _picking_view = new CairoPickingView (win);
    WinImpl::set_picking_view (_picking_view);
  }

  CairoGLSDLWindow::~CairoGLSDLWindow ()
  {
    if (is_activated)
      deactivate (); 
  }


  static int resizingEventWatcher(void* data, SDL_Event* event);

  void
  CairoGLSDLWindow::handle_resized(int w, int h)
  {
  }

  void
  CairoGLSDLWindow::activate ()
  {
    //std::cerr << _window->title ()->get_value ().c_str() << std::endl;
    double x, y, w, h;
    x = _window->pos_x ()->get_value ();
    y = _window->pos_y ()->get_value ();
    w = _window->width ()->get_value ();
    h = _window->height ()->get_value ();
    const std::string &title = _window->title ()->get_value ();

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

#if USE_GL_2_1
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); //MacOS https://stackoverflow.com/a/13095742
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
#endif

#ifdef USE_GL_ES_VERSION_2_0
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#endif

#if USE_TARP
    if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)<0) {
       std::cerr << "could not set attribute SDL error: " << SDL_GetError() << __FL__;
    }

    if(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4)<0) {
      std::cerr << "could not set attribute SDL error: " << SDL_GetError() << __FL__;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); //MacOS https://stackoverflow.com/a/13095742
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
#endif

    _sdl_window = SDL_CreateWindow (
        _window->title ()->get_value ().c_str(), 
        (int) (_window->pos_x ()->get_value ()),
        (int) (_window->pos_y ()->get_value ()),
        (int) (_window->width ()->get_value ()),
        (int) (_window->height ()->get_value ()),
        SDL_WINDOW_SHOWN
        | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_ALLOW_HIGHDPI
        | SDL_WINDOW_OPENGL
        );

    if( _sdl_window == NULL ) {
      std::cerr <<  "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
      return;
    }

    SDL_DisplayMode mode;
    SDL_GetWindowDisplayMode(_sdl_window, &mode);
    std::cerr << "SDL " << mode.refresh_rate << "hz"<< __FL__;
    _refresh_rate = mode.refresh_rate;

    //int ww, wh;
    //SDL_GetWindowSize(sdl_window(), &ww, &wh);

    _sdl_context = SDL_GL_CreateContext(_sdl_window);
    if (!_sdl_context) {
      fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
      exit(1);
    }

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    //SDL_GL_SetSwapInterval(1);

    _my_cairo_surface = new MyCairoSurface (_window);

    SDLMainloop::instance ().add_window (this);
  }

  void
  CairoGLSDLWindow::deactivate ()
  {
    SDLMainloop::instance ().remove_window (this);
    delete _my_cairo_surface;
    //delete _picking_view;
    SDL_GL_DeleteContext(_sdl_context);
    SDL_DestroyWindow( _sdl_window );

    _my_cairo_surface = nullptr;
    _sdl_context = nullptr;
    _sdl_window = nullptr;
    is_activated = false;
  }

  void
  CairoGLSDLWindow::update ()
  {
    SDLMainloop::instance().wakeup(this);
  }

  // FIXME better name
  void
  CairoGLSDLWindow::end_redraw ()
  {
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); CHKGL;
    SDL_GL_SwapWindow (_sdl_window);
    //SDL_Delay( 1 );
  }

  void
  CairoGLSDLWindow::update_hdpi()
  {
    // FIXME should be a connection
    int dw, dh;
    int ww = _window->width()->get_value ();
    SDL_GL_GetDrawableSize(sdl_window(), &dw, &dh);
    /*
    if(dw != ww) {
      _window->hidpi_scale()->set_value(dw/ww, true); // FIXME not really used elsewhere
      current_context().hidpi_scale() = dw/ww; // this one is used
    } else {
      _window->hidpi_scale()->set_value(1.0, true);
      current_context().hidpi_scale() = 1.0;
    }*/
  }

  void
  CairoGLSDLWindow::redraw ()
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

    //drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) _sdl_surface->pixels, CAIRO_FORMAT_ARGB32,
    //                                                      _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);
    //picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, _sdl_surface->w, _sdl_surface->h);

    // cairo gl
    //drawing_surface = cairo_gl_surface_create (device, CAIRO_CONTENT_COLOR_ALPHA, _sdl_surface->w, _sdl_surface->h);
    //picking_surface = cairo_gl_surface_create (device, CAIRO_CONTENT_COLOR, _sdl_surface->w, _sdl_surface->h);

    #if defined(SDL_VIDEO_DRIVER_X11) && CAIRO_HAS_GLX_FUNCTIONS
    SDL_SysWMinfo info;
    SDL_GetWindowWMInfo(_sdl_window, &info);
    // https://stackoverflow.com/questions/39476501/how-to-obtain-the-glxcontext-in-sdl
    cairo_device_t* device = cairo_glx_device_create (info.x11.Display, (GLXContext) _sdl_context);
    drawing_surface = cairo_gl_surface_create_for_window (device, info.x11.Window, w, h);
    picking_surface = cairo_gl_surface_create_for_window (device, info.x11.Window, w, h);
    #endif
    
    #if CAIRO_HAS_EGL_FUNCTIONS
    cairo_device_t* device = cairo_egl_device_create (eglGetCurrentDisplay(), (EGLContext) _sdl_context);
    drawing_surface = cairo_gl_surface_create_for_egl (device, eglGetCurrentSurface(EGL_DRAW), _window->width ()->get_value (), _window->height ()->get_value ());
    picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, _window->width ()->get_value (), _window->height ()->get_value ());
    #endif
    
    _my_cairo_surface->update (drawing_surface, picking_surface);

    cairo_surface_flush (drawing_surface);
    cairo_surface_flush (picking_surface);

    cairo_gl_surface_swapbuffers(drawing_surface);
    
    //SDL_GL_SwapWindow (_sdl_window);

    //data = cairo_image_surface_get_data (drawing_surface);
    //picking_data = cairo_image_surface_get_data (picking_surface);
    //_picking_view->set_data (picking_data, _sdl_surface->w, _sdl_surface->h,
    //                         cairo_image_surface_get_stride (picking_surface));

    // SDL_UpdateTexture (_sdl_texture, NULL, data, _sdl_surface->pitch);
    // SDL_RenderCopy (_sdl_renderer, _sdl_texture, nullptr, nullptr);
    // SDL_RenderPresent (_sdl_renderer);
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



}
