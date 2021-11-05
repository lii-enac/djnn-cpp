/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2021)
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
#include "display/window.h"
#include "display/pickui.h"

namespace djnn {

	class ColorPickingView : public Picking
  {
  public:
    ColorPickingView (Window *win);
    virtual ~ColorPickingView ();

    // Picking
    virtual void init ();
    PickUI* pick (double x, double y);
    void add_pick_shape (PickUI* gobj, bool cache = false);
    void remove_pick_shape (PickUI* gobj);
    virtual int get_pixel(unsigned int x, unsigned int y) = 0;
    virtual void object_deactivated (PickUI* gobj);

    // ColorPicking
    unsigned int pick_color () { return _pick_color; }

  protected:
    unsigned int _pick_color;
    map<unsigned int, PickUI*> _color_map;

    int seed;
    double myrandom();
    void next_color();
  };
}
