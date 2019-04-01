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

#include "../backend.h"

#include "../../core/syshook/external_source.h"
#include "../../display/window.h"
#include "qt_picking_view.h"

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
    MyQWidget* qwidget() { return _qwidget; }

  protected:
    /* Process */
    void activate () override;
    void deactivate () override;

    friend class MyQWidget;

  private:
    MyQWidget * _qwidget;
    Window* _window;
    bool _please_update;
    friend class MyQWidget;
  };

}
