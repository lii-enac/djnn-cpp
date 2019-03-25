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
#include "gl2d/render.hpp"

//#include "opengl.h"
#include "gl_debug.h"

#include <assert.h>

// unsigned int get_next_picking_color(djnn::GLSDLWindow* w)
// {
//   return dynamic_cast<djnn::ColorPickingView*>(w->picking_view())->pick_color();
//   //return 0;
// }

//namespace gl2d {
extern GLubyte* picking_data;
//}

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
//#if defined(GL_VERSION_1_0) || defined(GL_ES_VERSION_3_0)

//#if defined(GL_READ_BUFFER)
#if 0
#ifndef __EMSCRIPTEN__
    glReadBuffer(GL_BACK);//glReadBuffer(GL_FRONT); // if debugging with visible picking view
#else
    glReadBuffer(GL_FRONT); // if debugging with visible picking view
#endif
    double scale = current_context().hidpi_scale();
    unsigned int color;
    glReadPixels(x*scale, (_win->height()->get_value()-y)*scale, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    return color;
#else

    // static int get_pixel_warn=0;
    // if(!get_pixel_warn) std::cout << "warning get pixel unimplemented in GLES2" << std::endl;
    // get_pixel_warn=1;
    // return 0;

    double scale = current_context().hidpi_scale();

    unsigned int w_ = _win->width()->get_value();
    unsigned int h_ = _win->height()->get_value();
    unsigned int x_ = x * scale;
    unsigned int y_ = (h_-y) * scale;
    unsigned index = y_ * w_ + x_;
    unsigned int color=0;
    //assert(picking_data);

    if(!picking_data) {
      gl2d_render_scene_picking_post_get_data(_win, w_, h_, false, current_context().hidpi_scale());
    }
    
    if(picking_data) color = ((unsigned int*) picking_data)[index];
    //BGRA

    //std::cerr << hex << attr(color) << dec << __FL__;
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
