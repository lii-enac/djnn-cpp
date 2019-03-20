/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2014-2019)
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

#include "../abstract_gshape.h"
#include "../gui-priv.h"

namespace djnn {

  class Window;
  class Picking
  {
  public:
    Picking (Window *win);
    virtual
    ~Picking ();
    virtual void init () = 0;
    virtual AbstractGShape* pick (double x, double y) = 0;
    virtual void add_gobj (AbstractGShape* gobj) = 0;

    void object_deleted (AbstractGShape* gobj);

    bool genericMousePress(double x, double y, mouse_button button);
    bool genericMouseMove(double x, double y);
    bool genericCheckShapeAfterDraw (double x, double y);
    bool genericMouseRelease(double x, double y, mouse_button button);
    bool genericMouseWheel(double x, double y);

    bool genericTouchPress (double x, double y, int id);
    bool genericTouchMove (double x, double y, int id);
    bool genericTouchRelease (double x, double y, int id);

    void set_local_coords (AbstractGShape *s, Touch *t, double x, double y);
  protected:
    Window *_win;
    AbstractGShape *_cur_obj;
    map <int, Touch*> _active_touches;
  };
}
