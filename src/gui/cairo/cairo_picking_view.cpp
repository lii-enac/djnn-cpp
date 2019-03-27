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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "../backend.h"

#include <assert.h>
#include "cairo_picking_view.h"

namespace djnn
{

  CairoPickingView::CairoPickingView (Window *win) :
      ColorPickingView (win),  _cur_data (0), _w (0), _h (0), _stride (0)
  {
    assert(win);
  }

  CairoPickingView::~CairoPickingView ()
  {
  }

  int
  CairoPickingView::get_pixel (int x, int y)
  {
    if (x < 0 || y < 0 || x > _w || y > _h)
      return 0;

    //std::cerr << x << " " << y << std::endl;

    unsigned char *px = _cur_data + (y * _stride) + x * 4;
    unsigned int result = (unsigned int) (px[3] << 24 | (px[2] << 16) | (px[1] << 8) | px[0]);
    return result;
  }

  void
  CairoPickingView::init ()
  {
    ColorPickingView::init ();
  }

} /* namespace djnn */

