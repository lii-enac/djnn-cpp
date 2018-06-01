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

#include "../backend.h"

#include <iostream>
#include <QtWidgets/QWidget>

#include "../../core/syshook/external_source.h"
#include "../window.h"
#include "../qt/qt_window.h"

namespace djnn {

  // no signal for QWidget events!!!
  class MyQWidget : public QWidget
  {
    Q_OBJECT
    // /usr/local/Cellar/qt5/5.10.1/bin/moc src/gui/qt/my_qwindow.h > src/gui/qt/moc_MyQWindow.cpp 

  public:
    MyQWidget(Window *w, QtWindow * qtw) : _window (w), _qtwindow (qtw) { _picking_view = new QtPickingView (w); }
    virtual ~MyQWidget () { delete _picking_view; }
  protected:

    virtual void moveEvent (QMoveEvent *event) override;
    virtual void resizeEvent (QResizeEvent * event) override;
    virtual void mouseReleaseEvent (QMouseEvent *event) override;
    virtual void mousePressEvent (QMouseEvent *event) override;
    virtual void mouseMoveEvent (QMouseEvent *event) override;
    virtual void wheelEvent (QWheelEvent *event) override;
    virtual void closeEvent (QCloseEvent *event) override;
    virtual void paintEvent (QPaintEvent *event) override;

    Window * _window;
    QtWindow * _qtwindow;
    QtPickingView *_picking_view;
    
    bool _updating;
  };
}
