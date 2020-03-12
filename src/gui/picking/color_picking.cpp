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

#include "utils/debug.h"
#include <iostream>

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

  void
  ColorPickingView::init ()
  {
    _pick_color = 0xff000000; // production
    //_pick_color = 0xff203040; // for debugging
    seed = 0;
    next_color();
    //if (!_color_map.empty ())
      _color_map.clear ();
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
    // (maxed out to 200 to see color on white bg)
    // (mined out to 100 to see color on black bg)
    int r = 100+myrandom()*155;
    int g = 100+myrandom()*155;
    int b = 100+myrandom()*155;
    _pick_color = ((((((0xff << 8) + r) << 8) + g) << 8) + b);
#endif
    //std::cerr << hex << _pick_color << dec << __FL__;
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
  ColorPickingView::object_deactivated (AbstractGShape* gobj)
  {

    Picking::object_deactivated (gobj);
    
    // FIXME : should we delete object from picking_view when deactivated
    auto it = _color_map.begin ();
    for ( ; it != _color_map.end () ; ++it) {
      if (it->second == gobj)
        break;
    }
    if (it != _color_map.end ())
      _color_map.erase (it);

  }


} /* namespace djnn */
