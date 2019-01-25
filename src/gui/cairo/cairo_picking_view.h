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
 *
 */

#pragma once

#include "../picking/color_picking.h"
#include "../../core/core-dev.h"
#include "../window.h"
#include "../abstract_gshape.h"


namespace djnn
{

  class CairoPickingView : public ColorPickingView
  {
  public:
    CairoPickingView (Window *win);
    virtual
    ~CairoPickingView ();
    virtual void init ();
    virtual int get_pixel(int x, int y);
    void set_data (unsigned char* data, int w, int h, int stride) { _cur_data = data; _w =w; _h = h; _stride = stride;}
  private:
    Window * _win;
    unsigned char* _cur_data;
    int _w, _h, _stride;
  };
} /* namespace djnn */
