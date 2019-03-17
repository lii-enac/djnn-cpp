/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stéphane Chatty <chatty@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "../backend.h"

#include "gl_sdl_picking_view.h"
#include "gl_sdl_window.h"

#include "opengl.h"
#include "gl_debug.h"

#include <assert.h>

// unsigned int get_next_picking_color(djnn::GLSDLWindow* w)
// {
//   return dynamic_cast<djnn::ColorPickingView*>(w->picking_view())->pick_color();
//   //return 0;
// }

namespace djnn
{

  GLSDLPickingView::GLSDLPickingView (Window *win, GLSDLWindow* sdlwin) :
      ColorPickingView(win), _sdlwin(sdlwin)
  {
    assert(sdlwin);
  }

  GLSDLPickingView::~GLSDLPickingView ()
  {
  }

  int
  GLSDLPickingView::get_pixel (int x, int y)
  {
#ifdef __EMSCRIPTEN__
    static int get_pixel_warn=0;
    if(!get_pixel_warn) std::cout << "warning get pixel unimplemented on emscripten" << std::endl;
    get_pixel_warn=1;
    return 0;
#else
    glReadBuffer(GL_BACK);
    unsigned int color;
    double scale = current_context().hidpi_scale();
    glReadPixels(x*scale, (_win->height()->get_value()-y)*scale, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    return color;
#endif
    //glReadBuffer(GL_FRONT); // if debugging with visible picking view
    //std::cerr << attr(x) << attr(y) << attr(_win->height()->get_value()) << __FL__;
    //glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE, rgba);
    //std::cerr << scale << " " << std::hex << color << std::dec << __FL__;
    //return 0;//_image->pixel (x, y);
  }

  void
  GLSDLPickingView::init ()
  {
    ColorPickingView::init();
  }

} /* namespace djnn */
