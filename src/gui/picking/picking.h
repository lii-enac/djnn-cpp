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

#include "gui/gui-priv.h"
#include "display/display-dev.h" // mouse_button

namespace djnn {

  class AbstractGShape;
  class Window;
  class Picking
  {
  public:
    Picking (Window *win);
    virtual
    ~Picking ();
    virtual void init () = 0;
    virtual AbstractGShape* pick (double x, double y) = 0;

    virtual void add_gobj (AbstractGShape* gobj, bool cache = false) = 0;
    virtual void object_deactivated (AbstractGShape* gobj);

    void common_press_setting (double x, double y, AbstractGShape *s);
    void common_press_notify (AbstractGShape *s);

    bool genericMousePress(double x, double y, mouse_button button);
    bool genericMouseMove(double x, double y);
    bool genericCheckShapeAfterDraw (double x, double y);
    bool genericMouseRelease(double x, double y, mouse_button button);
    bool genericMouseWheel(double x, double y);

    bool genericTouchPress (double x, double y, int id, float pressure);
    bool genericTouchMove (double x, double y, int id, float pressure);
    bool genericTouchRelease (double x, double y, int id, float pressure);

    std::pair<double, double> set_local_coords (AbstractGShape *s, Touch *t, double x, double y, bool is_move);
  protected:

    bool genericEnterLeave (AbstractGShape* gobj);

    Window *_win;
    AbstractGShape *_catched_shape, *_hovered_shape;
    std::map <int, Touch*> _active_touches;
    bool _mouse_released;
  };
}
