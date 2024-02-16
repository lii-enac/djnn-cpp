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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include <QtWidgets/QWidget>

#include "display/qt/qt_window.h"
#include "display/window.h"
#include "exec_env/external_source.h"
using QTWIDGET = QWidget;
// #include <QtWidgets/QOpenGLWidget>
// using QTWIDGET = QOpenGLWidget;

namespace djnn {

class Picking;

// no signal for QWidget events!!!
class MyQWidget : public QTWIDGET {
    Q_OBJECT
    // moc src/display/qt/my_qwindow.h > src/display/qt/moc_MyQWindow.cpp

  public:
    MyQWidget (Window* w, QtWindow* qtw);
    virtual ~MyQWidget () {
        // delete _picking_view;
    }
    virtual Picking* get_picking_view () { return nullptr; }

    virtual void set_building (bool);
    void         set_in_screenshot (bool state) { _in_screenshot = state; }

  protected:
    virtual bool event (QEvent* event) override;
    virtual void moveEvent (QMoveEvent* event) override;
    virtual void resizeEvent (QResizeEvent* event) override;
    virtual void keyPressEvent (QKeyEvent* event) override;
    virtual void keyReleaseEvent (QKeyEvent* event) override;
    virtual void closeEvent (QCloseEvent* event) override;

    Window*           _window;
    QtWindow*         _qtwindow;
    int               mouse_pos_x, mouse_pos_y;
    bool              _updating;
    bool              _in_screenshot;
    std::atomic<bool> _building;
};
} // namespace djnn
