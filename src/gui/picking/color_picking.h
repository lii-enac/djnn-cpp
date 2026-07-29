/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2024)
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
#pragma once

#include "display/pickui.h"
#include "display/window.h"
#include "picking.h"

namespace djnn {

class ColorPickingView : public Picking {
  public:
    ColorPickingView (Window* win);
    virtual ~ColorPickingView ();

    // Picking
    virtual void init ();
    PickUI*      pick (double x, double y);
    void         add_pick_shape (PickUI* gobj, bool cache = false);
    void         remove_pick_shape (PickUI* gobj);
    virtual uint  get_pixel (unsigned int x, unsigned int y) = 0;
    virtual void object_deactivated (PickUI* gobj);

    // ColorPicking
    using pick_color_t = unsigned int; 
    pick_color_t pick_color () { return _pick_color; }
    void set_pick_color (pick_color_t c) { _pick_color = c; }

  protected:
    pick_color_t                        _pick_color;
    djnnstl::map<pick_color_t, PickUI*> _color_map;

    int             seed;
    double          myrandom ();
    void            next_color ();
    virtual PickUI* pick_impl (pick_color_t color); // default implementation uses _color_map
};
} // namespace djnn
