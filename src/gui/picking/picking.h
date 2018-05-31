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

#include "../window.h"
#include "../abstract_gshape.h"

namespace djnn {
  class Picking
  {
  public:
    Picking (Window *win);
    virtual
    ~Picking ();
    virtual void init () = 0;
    virtual AbstractGShape* pick (double x, double y) = 0;
    virtual void add_gobj (AbstractGShape* gobj) = 0;
    virtual int get_pixel(int x, int y) = 0;

    bool genericMousePress(double x, double y, int button);
    bool genericMouseMove(double x, double y);
    bool genericMouseRelease(double x, double y, int button);
    bool genericMouseWheel(double x, double y);

  protected:
    Window *_win;
    map<unsigned int, AbstractGShape*> _color_map;
    AbstractGShape *_cur_obj;
  };
}
