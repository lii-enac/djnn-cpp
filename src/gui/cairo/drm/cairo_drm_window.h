/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "display/drm/drm_display.h"
#include "display/drm/drm_window.h"
#include "gui/cairo/my_cairo_surface.h"

namespace djnn {

  class CairoDRMWindow : public DRMWindow
  {
  private:
    class RefreshAction : public Action
    {
    public:
      RefreshAction (CairoDRMWindow* win, const std::string& name) : Action (nullptr, name), _win (win) { Process::finalize_construction (nullptr, name); }
      virtual ~RefreshAction () {}
      void impl_activate () { _win->ask_refresh (); }
      void impl_deactivate () {}
    private:
      CairoDRMWindow *_win;
    };
    class ChangeDpyAction : public Action
    {
    public:
      ChangeDpyAction (CairoDRMWindow* win, const std::string& name) : Action (nullptr, name), _win (win) { Process::finalize_construction (nullptr, name); }
      virtual ~ChangeDpyAction () {}
      void impl_activate () { _win->update_dpy_connection (); _win->ask_refresh (); }
      void impl_deactivate () {_win->update_dpy_connection (); }
    private:
      CairoDRMWindow *_win;
    };
  public:
    CairoDRMWindow (Window *win, const std::string& title, double x, double y, double w, double h);
    virtual ~CairoDRMWindow ();
    void ask_refresh ();
    void update () override;
    void update_dpy_connection ();
  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;

  private:
    unsigned char * _picking_data;
    MyCairoSurface *_my_cairo_surface;
    CairoPickingView *_picking_view;
    RefreshAction _refresh;
    ChangeDpyAction _change_dpy_action;
    Coupling *_c_vblank,*_c_display;
  };
}
