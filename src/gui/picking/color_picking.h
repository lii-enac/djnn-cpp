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

#pragma once

#include "picking.h"
#include "../window.h"
#include "../abstract_gshape.h"

namespace djnn {
	class ColorPickingView : public Picking
  {
  public:
    ColorPickingView (Window *win);
    virtual ~ColorPickingView ();

    // Picking
    virtual void init ();
    AbstractGShape* pick (double x, double y);
    void add_gobj (AbstractGShape* gobj);
    virtual int get_pixel(unsigned int x, unsigned int y) = 0;

    // ColorPicking
    unsigned int pick_color () { return _pick_color; }

  protected:
    unsigned int _pick_color;
    map<unsigned int, AbstractGShape*> _color_map;

    int seed;
    double myrandom();
    void next_color();

  };
}
