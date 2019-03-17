#include "gl_sdl_window.h"
#include "../sdl/sdl_mainloop.h"

#include "opengl.h"
#include "gl_dbg.h"

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

  GLSDLWindow::GLSDLWindow (djnn::Window* win, const std::string &title, double x, double y, double w, double h) :
  SDLWindow(win, title, x,y,w,h)
  {

  }

  GLSDLWindow::~GLSDLWindow ()
  {
    
  }


  static int resizingEventWatcher(void* data, SDL_Event* event);

  void
  GLSDLWindow::handle_resized(int w, int h)
  {
    SDLWindow::handle_resized(w,h);
    redraw();
  }

  void
  GLSDLWindow::activate ()
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

#if USE_GL2D
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); //MacOS https://stackoverflow.com/a/13095742
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    // SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

#ifdef USE_GL_ES_VERSION_2_0
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#endif
    
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

    update_hdpi();

    SDL_DisplayMode mode;
    SDL_GetWindowDisplayMode(_sdl_window, &mode);
    std::cout << "SDL " << mode.refresh_rate << "hz"<< __FL__;
    _refresh_rate = mode.refresh_rate;

    _sdl_context = SDL_GL_CreateContext(_sdl_window);
    if (!_sdl_context) {
      fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
      exit(1);
    }

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    //SDL_GL_SetSwapInterval(1);

    SDLMainloop::instance ().add_window (this);
  }

  void
  GLSDLWindow::deactivate ()
  {
    SDLMainloop::instance ().remove_window (this);
    //delete _picking_view;
    SDL_GL_DeleteContext(_sdl_context);
    SDL_DestroyWindow( _sdl_window );

    _sdl_context = nullptr;
    _sdl_window = nullptr;
    is_activated = false;
  }

  void
  GLSDLWindow::update ()
  {
    SDLMainloop::instance().wakeup(this);
  }

  // FIXME better name
  void
  GLSDLWindow::end_redraw ()
  {
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); CHKGL;
    SDL_GL_SwapWindow (_sdl_window);
    //SDL_Delay( 1 );
  }

  void
  GLSDLWindow::update_hdpi()
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
  GLSDLWindow::redraw ()
  {
    // WinImpl * winimpl = _window->win_impl();
    // assert(winimpl);
    // GLSDLWindow * sdlwin = dynamic_cast<GLSDLWindow*>(winimpl); // bizarre pourquoi tout ça ??
    // assert(sdlwin);
    // SDL_GL_MakeCurrent(sdlwin->sdl_window(), sdlwin->sdl_context());

    #if _PERF_TEST
    t1();
    #endif

    glClearColor (1, 1, 1, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	glEnable(GL_BLEND);

  	//__EFL__;
  	CHKGL;

    //GLBackend* backend = dynamic_cast<GLBackend*> (Backend::instance ());
    //backend->start_redraw();
    //backend->set_window (_window);
    Process *p = _window->get_parent ();
    if (p) {
       p->draw ();
    }

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

    //backend->end_redraw();
  }



}
