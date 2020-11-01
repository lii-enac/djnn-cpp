/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "exec_env/external_source.h"
#include "display/window.h"

#include <string>

namespace djnn {

  class MyQWidget;

  class QtWindow : public WinImpl
  {
  public:
    QtWindow (Window *win, const std::string& title, double x, double y, double w, double h);
    virtual ~QtWindow ();
    void update () override;
    void check_for_update ();
    void set_cursor (const std::string& path, int hotX, int hotY) override;
    //void set_qwidget(MyQWidget* q) { _qwidget = q; }
    MyQWidget* qwidget() { return _qwidget; }

    virtual void perform_screenshot (const std::string& path) override;
    virtual void set_opacity (double opacity) override;

  protected:
    /* FatProcess */
    void impl_activate () override;
    void impl_deactivate () override;

    friend class MyQWidget;

  private:
    MyQWidget * _qwidget;
    Window* _window;
    std::atomic<bool> _please_update;
    friend class MyQWidget;
  };

}
