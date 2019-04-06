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

//#include "../backend.h"

#include <iostream>
#include <QtWidgets/QWidget>

#include "../../../core/syshook/external_source.h"
#include "../../../display/window.h"
#include "../../../display/qt/qt_window.h"
#include "../../../display/qt/my_qwindow.h"

#include "qt_picking_view.h"

namespace djnn {

  // no signal for QWidget events!!!
  class MyQQWidget : public MyQWidget
  {
    Q_OBJECT
    // /usr/local/Cellar/qt5/5.10.1/bin/moc src/gui/qt/my_qwindow.h > src/gui/qt/moc_MyQWindow.cpp 

  public:
    MyQQWidget(Window *w, QtWindow * qtw) : MyQWidget(w,qtw) {
      setAttribute(Qt::WA_AcceptTouchEvents, true);
      _picking_view = new QtPickingView (w);
    }
    virtual ~MyQQWidget () { 
      delete _picking_view;
    }
  protected:

    virtual bool event (QEvent *event) override;
    virtual void mouseReleaseEvent (QMouseEvent *event) override;
    virtual void mousePressEvent (QMouseEvent *event) override;
    virtual void mouseMoveEvent (QMouseEvent *event) override;
    virtual void wheelEvent (QWheelEvent *event) override;
    virtual void paintEvent (QPaintEvent *event) override;
    #if 0
    virtual void moveEvent (QMoveEvent *event) override;
    virtual void resizeEvent (QResizeEvent * event) override;
    virtual void keyPressEvent (QKeyEvent *event) override;
    virtual void keyReleaseEvent (QKeyEvent *event) override;
    virtual void closeEvent (QCloseEvent *event) override;
    
  #endif
    //Window * _window;
    //QtWindow * _qtwindow;
    QtPickingView *_picking_view;
    //int mouse_pos_x, mouse_pos_y;
    //bool _updating;
  };
}
