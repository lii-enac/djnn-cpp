
#include "utils/debug.h"

#include "cairo_gl_sdl_window.h"
#include "display/sdl/sdl_mainloop.h"

#include "opengl.h"
#include "gl_dbg.h"

// cairo gl
#include <SDL_syswm.h>
#include <cairo/cairo-gl.h>

#include <iostream>
#define __FL__ " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define attr(a) #a ":" << a << " "

#include "exec_env/exec_env-dev.h"

#if _DEBUG_SEE_GUI_INFO_PREF
//#include "core/utils/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

static int glad_inited=0;

namespace djnn {

  CairoGLSDLWindow::CairoGLSDLWindow (djnn::Window* win, const std::string& title, double x, double y, double w, double h) :
  SDLWindow(win, title, x,y,w,h),
  _my_cairo_surface (nullptr)
  {
    _picking_view = new CairoPickingView (win);
    WinImpl::set_picking_view (_picking_view);

#ifdef __glad_h_
    if(!glad_inited) {
      int glad_status = gladLoadGL();
      //int glad_status = gladLoadGLLoader(SDL_GL_GetProcAddress);
      if(glad_status) std::cerr << "glad could not open opengl" << __FL__;
      printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
      std::cout << "info: " << glGetString(GL_VENDOR) << " " << glGetString(GL_VERSION) << __FL__;
      glad_inited=1;
    }
#endif
  }

  CairoGLSDLWindow::~CairoGLSDLWindow ()
  {
    if (is_activated)
      impl_deactivate (); 
  }


  static int resizingEventWatcher(void* data, SDL_Event* event);

  void
  CairoGLSDLWindow::handle_resized(int w, int h)
  {
  }

  void
  CairoGLSDLWindow::impl_activate ()
  {
    //std::cerr << _window->title ()->get_value ().c_str() << std::endl;
    double x, y, w, h;
    x = _window->pos_x ()->get_value ();
    y = _window->pos_y ()->get_value ();
    w = _window->width ()->get_value ();
    h = _window->height ()->get_value ();
    const std::string& title = _window->title ()->get_value ();

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

#if USE_GL_VERSION_2_1
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); //MacOS https://stackoverflow.com/a/13095742
#endif

#ifdef USE_GL_ES_VERSION_2_0
    DBG;
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
        (int) x,
        (int) y,
        (int) w,
        (int) h,
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
    //std::cerr << "SDL " << mode.refresh_rate << "hz"<< __FL__;
    _refresh_rate = mode.refresh_rate;

    //int ww, wh;
    //SDL_GetWindowSize(sdl_window(), &ww, &wh);

    _sdl_context = SDL_GL_CreateContext(_sdl_window);
    if (!_sdl_context) {
      fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
      exit(1);
    }

    //std::cerr << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << " " << glGetString(GL_VERSION) << __FL__;


    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    //SDL_GL_SetSwapInterval(1);

    _my_cairo_surface = new MyCairoSurface (_window);

    SDLMainloop::instance ().add_window (this);
  }

  void
  CairoGLSDLWindow::impl_deactivate ()
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
    #if _DEBUG_SEE_GUI_INFO_PREF
    t1();
    #endif

    cairo_surface_t *drawing_surface, *picking_surface;
    unsigned char *data, *picking_data;
    CairoBackend* backend = dynamic_cast<CairoBackend*> (Backend::instance ());

    backend->set_window (_window);
    backend->set_picking_view (_picking_view);
    _picking_view->init ();

    double w = _window->width ()->get_value (), h = _window->height ()->get_value ();

    //drawing_surface = cairo_image_surface_create_for_data ((unsigned char*) _sdl_surface->pixels, CAIRO_FORMAT_ARGB32,
    //                                                      _sdl_surface->w, _sdl_surface->h, _sdl_surface->pitch);
    //picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, _sdl_surface->w, _sdl_surface->h);

    // see https://news.ycombinator.com/item?id=16539446

  #if defined(SDL_VIDEO_DRIVER_X11) && CAIRO_HAS_GLX_FUNCTIONS
    SDL_SysWMinfo info;
    SDL_GetWindowWMInfo(_sdl_window, &info);
    // https://stackoverflow.com/questions/39476501/how-to-obtain-the-glxcontext-in-sdl
    cairo_device_t* device = cairo_glx_device_create (info.info.x11.display, (GLXContext) _sdl_context);
    if(device==nullptr) {
      std::cerr << "could not create cairo glx device" << __FL__;
      exit(1);
    }
    cairo_status_t st = cairo_device_status (device);
    if( st != CAIRO_STATUS_SUCCESS ) {
       std::cerr << cairo_status_to_string (st) << __FL__;
       exit(1);
    }
    drawing_surface = cairo_gl_surface_create_for_window (device, info.info.x11.window, w, h);
    if(drawing_surface==nullptr) {
      std::cerr << "could not create cairo gl surface" << __FL__;
      exit(1);
    }
    st = cairo_surface_status (drawing_surface);
    if( st != CAIRO_STATUS_SUCCESS ) {
       std::cerr << cairo_status_to_string (st) << __FL__;
       exit(1);
    }
    picking_surface = cairo_gl_surface_create_for_window (device, info.info.x11.window, w, h);

  #elif CAIRO_HAS_EGL_FUNCTIONS
    cairo_device_t* device = cairo_egl_device_create (eglGetCurrentDisplay(), (EGLContext) _sdl_context);
    if(device==nullptr) {
      std::cerr << "could not create cairo egl device" << __FL__;
      exit(1);
    }
    cairo_status_t st = cairo_device_status (device);
    if( st != CAIRO_STATUS_SUCCESS ) {
       std::cerr << cairo_status_to_string (st) << __FL__;
       exit(1);
    }
    drawing_surface = cairo_gl_surface_create_for_egl (device, eglGetCurrentSurface(EGL_DRAW), w, h);
    if(drawing_surface==nullptr) {
      std::cerr << "could not create cairo gl surface" << __FL__;
      exit(1);
    }
    st = cairo_surface_status (drawing_surface);
    if( st != CAIRO_STATUS_SUCCESS ) {
       std::cerr << cairo_status_to_string (st) << __FL__;
       exit(1);
    }
    picking_surface = cairo_image_surface_create (CAIRO_FORMAT_RGB24, w, h);
  #endif

#if 1
    _my_cairo_surface->update (drawing_surface, picking_surface);

    cairo_surface_flush (drawing_surface);
    cairo_surface_flush (picking_surface);
    cairo_gl_surface_swapbuffers(drawing_surface);
#endif

#if 0
    #define CHKSDL if(sdlerr<0) std::cerr << SDL_GetError() << __FL__;
    int sdlerr;

    sdlerr = SDL_GL_MakeCurrent(_sdl_window, _sdl_context); CHKSDL;
    glViewport(0, 0, w, h);
    glClearColor(1,0,0,1); CHKGL;
    glClear (GL_COLOR_BUFFER_BIT); CHKGL;

    SDL_GL_SwapWindow (_sdl_window); CHKGL;
    DBG;
#endif
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
#if 1
    cairo_surface_destroy (drawing_surface);
    cairo_surface_destroy (picking_surface);
#endif

#if _DEBUG_SEE_GUI_INFO_PREF
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
