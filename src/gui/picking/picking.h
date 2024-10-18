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

#include "display/display-dev.h" // mouse_button and stylus type
#include "display/pickui.h"
#include "gui/gui-priv.h"
#include "gui/shape/abstract_gshape.h" // touch

namespace djnn {

class Window;
class Picking
{
  public:
    Picking (Window* win);
    virtual ~Picking ();
    virtual void    init ()                   = 0;
    virtual PickUI* pick (double x, double y) = 0;

    virtual void add_pick_shape (PickUI* pshape, bool cache = false) = 0;
    virtual void object_deactivated (PickUI* gobj);

    void common_press_setting (double x, double y, PickUI* s);
    void common_press_notify (PickUI* s);

    bool genericMousePress (double x, double y, mouse_button button);
    bool genericMouseMove (double x, double y);
    bool genericMouseRelease (double x, double y, mouse_button button);
    bool genericMouseWheel (double dx, double dy, double cursorx, double cursory);

    bool genericCheckShapeAfterDraw (double x, double y);

    bool genericTouchPress (double x, double y, int id, float pressure);
    bool genericTouchMove (double x, double y, int id, float pressure);
    bool genericTouchRelease (double x, double y, int id, float pressure);

    bool genericTabletPress (double x, double y, stylus_type type, mouse_button button, double pressure);
    bool genericTabletMove (double x, double y, stylus_type type, double pressure);
    bool genericTabletRelease (double x, double y, stylus_type type, mouse_button button, double pressure);

  protected:
    bool genericEnterLeave (PickUI* gobj);
    bool tabletEnterLeave (PickUI* picked, stylus_type type);
#ifndef DJNN_NO_DEBUG
    bool check_for_ui (PickUI* gobj);
#else
    bool
    check_for_ui (PickUI* gobj)
    {
        return true;
    }
#endif
    Window*                   _win;
    PickUI *                  _caught_shape, *_hovered_shape;
    djnnstl::map<int, Touch*> _active_touches;
    bool                      _mouse_released;
    bool                      _stylus_released;
};
} // namespace djnn
