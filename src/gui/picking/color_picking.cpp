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
 */
#include "color_picking.h"

namespace djnn
{

  ColorPickingView::ColorPickingView (Window *win) :
      Picking(win)
  {
      init();
  }

  ColorPickingView::~ColorPickingView ()
  {
  }


  double
  ColorPickingView::myrandom()
  {
    seed = (seed*9301+49297) % 233280;
    return seed/(233280.0);
  }

  void
  ColorPickingView::next_color()
  {
#if 1
    _pick_color++;
#else
    // repeatable random, helpful for debugging
    // //(maxed out to 200 to see color on white bg)
    // (mined out to 100 to see color on black bg)
    int r = 100+myrandom()*155;
    int g = 100+myrandom()*155;
    int b = 100+myrandom()*155;
    _pick_color = ((((((0xff << 8) + r) << 8) + g) << 8) + b);
#endif
  }

  void
  ColorPickingView::add_gobj (AbstractGShape *gobj)
  {
    _color_map.insert (pair<unsigned int, AbstractGShape*> (_pick_color, gobj));
    next_color();
  }

  AbstractGShape*
  ColorPickingView::pick (double x, double y)
  {
    int color = get_pixel (x, y);
    auto it = _color_map.find (color);
    if (it != _color_map.end ()) {
      return it->second;
    }
    return nullptr;
  }

  void
  ColorPickingView::init ()
  {
    _pick_color = 0xff000000;
    seed = 0;
    next_color();
    if (!_color_map.empty ())
      _color_map.clear ();
  }


} /* namespace djnn */
