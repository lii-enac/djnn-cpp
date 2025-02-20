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
#include "display/qt/qt_window_moc.h"
#include "display/window.h"
#include "exec_env/external_source.h"
#include "gui/picking/analytical_picking.h"
#include "qt_picking_view.h"
// #include <QTabletEvent>
// #include <QOpenGLFunctions>

namespace djnn {

// no signal for QWidget events!!!
class MyQQWidget : public MyQWidget //, protected QOpenGLFunctions
{
    Q_OBJECT
    // moc src/gui/qt/my_qwindow.h > src/gui/qt/moc_MyQWindow.cpp

  public:
    MyQQWidget (Window* w, QtWindow* qtw)
        : MyQWidget (w, qtw) {
        setAttribute (Qt::WA_AcceptTouchEvents, true);
        _picking_view = new QtPickingView (w, this);
        //_picking_view = new AnalyticalPicking (w);
    }
    virtual Picking* get_picking_view () override { return _picking_view; }
    virtual ~MyQQWidget () {
        delete _picking_view;
    }

    virtual bool event (QEvent* event) override;
    virtual void mouseReleaseEvent (QMouseEvent* event) override;
    virtual void mousePressEvent (QMouseEvent* event) override;
    virtual void mouseMoveEvent (QMouseEvent* event) override;
    virtual void wheelEvent (QWheelEvent* event) override;

  protected:
    virtual void paintEvent (QPaintEvent* event) override;
    virtual void tabletEvent (QTabletEvent* event) override;
    // virtual void paintGL () override;
    // virtual void initializeGL () override;

    QtPickingView* _picking_view;
    // AnalyticalPicking *_picking_view;
};
} // namespace djnn
