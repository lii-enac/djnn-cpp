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

#include "../backend.h"

#include "my_qwindow.h"
#include "qt_window.h"
#include "qt_backend.h"
#include "qt_mainloop.h"
#include "../../core/syshook/main_loop.h"
#include "../../core/syshook/syshook.h"
#include "../../core/execution/graph.h"
#include "../../core/execution/component_observer.h"

#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#define DBG //std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DEBUG 0

namespace djnn
{

  int mouse_tracking = 0;

  QtWindow::QtWindow (Window *win, const std::string& title, double x, double y, double w, double h) :
      _qwidget (nullptr), _window (win), _please_update (true)
  {
  }

  QtWindow::~QtWindow ()
  {
    QtMainloop::instance ().remove_window (this);
    if (_qwidget != nullptr)
      delete _qwidget;
  }

  void
  QtWindow::activate ()
  {
    QtMainloop::instance ().add_window (this);
    _qwidget = new MyQWidget (_window, this);
    QRect rect (_window->pos_x ()->get_value (), _window->pos_y ()->get_value (), _window->width ()->get_value (),
                _window->height ()->get_value ());
    if (mouse_tracking)
      _qwidget->setMouseTracking (true);
    _qwidget->setGeometry (rect);
    _qwidget->setWindowTitle (_window->title ()->get_value ().c_str ());
    _qwidget->show ();
  }

  void
  QtWindow::deactivate ()
  {
    QtMainloop::instance ().remove_window (this);
    delete _qwidget;
    _qwidget = nullptr;
  }

  void
  QtWindow::update ()
  {
    DBG
    ;
    if (_qwidget == nullptr)
      return;
    //_qwidget->update (); // won't work since qt is blocked in mainloop
    _please_update = true; // so remind this...
    QtMainloop::instance ().wakeup (); // ... and wake up qt
  }

  void
  QtWindow::check_for_update ()
  {
    if (_please_update) {
      _qwidget->update ();
      _please_update = false;
    }
  }

  void
  MyQWidget::moveEvent (QMoveEvent *event)
  {
    if (_updating)
      return;
    _updating = true;

    const QPoint pos = event->pos ();
    int x = pos.x ();
    int y = pos.y ();
    _window->x ()->set_value (x, 1);
    _window->y ()->set_value (y, 1);

    _updating = false;
    QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::resizeEvent (QResizeEvent * event)
  {
    if (_updating)
      return;
    _updating = true;

    int h = event->size ().height ();
    int w = event->size ().width ();
    _window->height ()->set_value (h, true);
    _window->width ()->set_value (w, true);

    _updating = false;
    QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::mousePressEvent (QMouseEvent *event)
  {
    //DBG;
    double x = event->x ();
    double y = event->y ();
    bool exec_ = _picking_view->genericMousePress(x,y,event->button());
    if (exec_) QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::mouseMoveEvent (QMouseEvent *event)
  {
    double x = event->x ();
    double y = event->y ();
    bool exec_ = _picking_view->genericMouseMove(x,y);
    if (exec_) QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::mouseReleaseEvent (QMouseEvent *event)
  {
    double x = event->x ();
    double y = event->y ();

    bool exec_ = _picking_view->genericMouseRelease(x,y,event->button());
    if (exec_) QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::wheelEvent (QWheelEvent *event)
  {
    QPoint angle = event->angleDelta () / 8; // the angle is in eights of a degree
    bool exec_ = _picking_view->genericMouseWheel(angle.x(), angle.y());
    if (exec_) QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::closeEvent (QCloseEvent *event)
  {
    // close child should be notified but Qt wants the control
    // and will not let it do the job
    _window->close ()->notify_activation ();
  }

  void
  MyQWidget::paintEvent (QPaintEvent *event)
  {
    DBG
    ;
    QtBackend* backend = dynamic_cast<QtBackend*> (Backend::instance ());
    backend->set_window (_window);
    QPainter painter (this);
    backend->set_painter (&painter);
    backend->set_picking_view (_picking_view);
    Process *p = _window->get_parent ();
    _picking_view->init ();
    if (p)
      p->draw ();
#if DEBUG
    _picking_view->display();
#endif
  }

}
