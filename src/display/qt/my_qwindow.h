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

#include "syshook/external_source.h"
#include "display/window.h"
#include "display/qt/qt_window.h"

#include <QtWidgets/QWidget>

namespace djnn {

  class Picking;

  // no signal for QWidget events!!!
  class MyQWidget : public QWidget
  {
    Q_OBJECT
    // /usr/local/Cellar/qt5/5.10.1/bin/moc src/display/qt/my_qwindow.h > src/display/qt/moc_MyQWindow.cpp 

  public:
    MyQWidget(Window *w, QtWindow * qtw) : _window (w), _qtwindow (qtw), mouse_pos_x (-1), mouse_pos_y (-1), _updating (false), _building (false) {
      setAttribute(Qt::WA_AcceptTouchEvents, true);
      //_picking_view = new QtPickingView (w);
    }
    virtual ~MyQWidget () { 
      //delete _picking_view;
    }
    virtual Picking* get_picking_view() { return nullptr; }

    virtual void set_building(bool);
  protected:

    virtual bool event (QEvent *event) override;
    virtual void moveEvent (QMoveEvent *event) override;
    virtual void resizeEvent (QResizeEvent * event) override;
    virtual void keyPressEvent (QKeyEvent *event) override;
    virtual void keyReleaseEvent (QKeyEvent *event) override;
    virtual void closeEvent (QCloseEvent *event) override;

    Window * _window;
    QtWindow * _qtwindow;
    int mouse_pos_x, mouse_pos_y;
    bool _updating;
    std::atomic<bool> _building;
  };
}
