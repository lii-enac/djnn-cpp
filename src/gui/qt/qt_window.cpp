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

#define _PERF_TEST 0
#if _PERF_TEST
#include "../../core/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{

  int mouse_tracking = 0;
  int full_screen = 0;

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
    if (full_screen)
      _qwidget->setWindowState(_qwidget->windowState() ^ Qt::WindowFullScreen);
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

  bool
  MyQWidget::event (QEvent *event)
  {
    switch (event->type ())
      {
      case QEvent::TouchBegin:
      case QEvent::TouchUpdate:
      case QEvent::TouchEnd:
        {
          QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *> (event)->touchPoints ();
          bool exec_ = false;
          for (auto touchPoint : touchPoints) {
            int id = touchPoint.id ();
            double x = touchPoint.pos ().x ();
            double y = touchPoint.pos ().y ();
            switch (touchPoint.state ())
              {
              case Qt::TouchPointStationary:
                continue;
              case Qt::TouchPointPressed:
                {
                  exec_ |= _picking_view->genericTouchPress (x, y, id);
                  break;
                }
              case Qt::TouchPointMoved:
                {
                  exec_ |= _picking_view->genericTouchMove (x, y, id);
                  break;
                }
              case Qt::TouchPointReleased:
                {
                  exec_ |= _picking_view->genericTouchRelease (x, y, id);
                  break;
                }
              }
          }
          if (exec_)
            QtMainloop::instance ().set_please_exec (true);
        }
        break;
      default:
        return QWidget::event (event);
      }
    return true;
  }

  void
  MyQWidget::keyPressEvent (QKeyEvent *event)
  {
    _window->key_pressed ()->set_value (event->key (), 1);
    _window->key_pressed_text ()->set_value (event->text().toStdString (), 1);
    QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::keyReleaseEvent (QKeyEvent *event)
  {
    _window->key_released ()->set_value (event->key (), 1);
    _window->key_released_text ()->set_value (event->text().toStdString (), 1);
    QtMainloop::instance ().set_please_exec (true);
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
    _window->pos_x ()->set_value (x, 1);
    _window->pos_y ()->set_value (y, 1);

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
    _qtwindow->_please_update = false;
  }

  void
  MyQWidget::mousePressEvent (QMouseEvent *event)
  {
//DBG;
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();
    bool exec_ = _picking_view->genericMousePress (mouse_pos_x, mouse_pos_y, event->button ());
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::mouseMoveEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();
    bool exec_ = _picking_view->genericMouseMove (mouse_pos_x, mouse_pos_y);
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::mouseReleaseEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMouseRelease (mouse_pos_x, mouse_pos_y, event->button ());
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::wheelEvent (QWheelEvent *event)
  {
    QPointF fdelta (event->angleDelta ());
    double dx = fdelta.x () / 8;
    double dy = fdelta.y () / 8;
    if (dx == 0 && dy == 0) // some trackpads seem to send a lot of unwanted zero values
      return;
    bool exec_ = _picking_view->genericMouseWheel (dx, dy);  // the angle is in eights of a degree
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
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
    if (p) {
#if _PERF_TEST
      t1();
#endif
      p->draw ();
#if _PERF_TEST
      double time = t2 ("\nDRAW : ");
      draw_counter = draw_counter + 1;
      draw_total = draw_total + time ;
      draw_average = draw_total / draw_counter;
      cerr << "DRAW : " << draw_counter << " - avg: " << draw_average << endl; 
#endif
    }
    if (_picking_view->genericCheckShapeAfterDraw (mouse_pos_x, mouse_pos_y))
      QtMainloop::instance ().set_please_exec (true);
#if DEBUG
    _picking_view->display();
#endif
  }

}
