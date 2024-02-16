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

#include <QtCore/Qt>

#include "display/window.h"
#include "exec_env/external_source.h"

namespace djnn {

class MyQWidget;

class QtWindow : public WinImpl {
  public:
    QtWindow (Window* win, const djnnstl::string& title, double x, double y, double w, double h);
    virtual ~QtWindow ();
    void update () override;
    void check_for_update ();
    void set_cursor (const djnnstl::string& path, int hotX, int hotY) override;
    void set_cursor (int cursor_shape) override;
    // void set_qwidget(MyQWidget* q) { _qwidget = q; }
    MyQWidget* qwidget () { return _qwidget; }

    virtual void perform_screenshot (const djnnstl::string& path) override;
    virtual void set_opacity (double opacity) override;
    virtual void set_background_opacity_and_color (double is_opaque, int r, int g, int b, bool isFrameless = false) override;
    virtual void set_minimum_size (int w, int h) override;
    virtual void set_minimum_size_for_good ();
    virtual void update_geometry () override;
    virtual void update_geometry_for_good ();

  protected:
    /* FatProcess */
    void impl_activate () override;
    void impl_deactivate () override;

    friend class MyQWidget;

  private:
    static Qt::CursorShape cursor_shapes[22];
    MyQWidget*             _qwidget;
    Window*                _window;
    std::atomic<bool>      _please_update;
    friend class MyQWidget;
};

} // namespace djnn
