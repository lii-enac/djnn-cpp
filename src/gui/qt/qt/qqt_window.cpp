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

#include "../../backend.h"
#include "../qt_backend.h"

#include "my_qqwindow.h"
#include "../../../display/display.h"
#include "../../../display/abstract_display.h"
#include "../../../display/qt/my_qwindow.h"
// #include "qt_window.h"
#include "../../../display/qt/qt_mainloop.h"
// #include "../display.h"
// #include "../display-dev.h"
// #include "../abstract_display.h"

// #include "../../core/syshook/main_loop.h"
// #include "../../core/syshook/syshook.h"
// #include "../../core/execution/graph.h"
// #include "../../core/execution/component_observer.h"

// #include <QtCore/QAbstractEventDispatcher>
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QWidget>
// #include <QtGui/QPainter>
// #include <QtGui/QMouseEvent>
// #include <QtGui/QBitmap>
#include <QEvent>
#include <QTouchEvent>

#define DBG //std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
#define DEBUG_PICKING  0

#define _PERF_TEST 0
#if _PERF_TEST
#include "../../core/utils-dev.h"
static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

namespace djnn
{
  bool
  MyQQWidget::event (QEvent *event)
  {

    /* note:
     * Get and release Mutex on each event BUT only the events that 
     * WE manage else we let Qt and QTwidgets dealing with these Events.
     */
    djnn::get_exclusive_access (DBG_GET);
    bool exec_ = false;
    switch (event->type ())
      {
      case QEvent::TouchBegin:
      case QEvent::TouchUpdate:
      case QEvent::TouchEnd:
        {
          QList < QTouchEvent::TouchPoint > touchPoints = static_cast<QTouchEvent *> (event)->touchPoints ();

          for (const auto & touchPoint : touchPoints) {
            const int &id = touchPoint.id ();
            const double &x = touchPoint.pos ().x ();
            const double &y = touchPoint.pos ().y ();
            const double &pressure = touchPoint.pressure ();
            switch (touchPoint.state ())
              {
              case Qt::TouchPointStationary:
                continue;
              case Qt::TouchPointPressed:
                {
                  exec_ |= _picking_view->genericTouchPress (x, y, id, pressure);
                  break;
                }
              case Qt::TouchPointMoved:
                {
                  exec_ |= _picking_view->genericTouchMove (x, y, id, pressure);
                  break;
                }
              case Qt::TouchPointReleased:
                {
                  exec_ |= _picking_view->genericTouchRelease (x, y, id, pressure);
                  break;
                }
              }
          }
          if (exec_)
            QtMainloop::instance ().set_please_exec (true);
        }
        break;

      case QEvent::MouseButtonPress:
      case QEvent::MouseMove:
      case QEvent::MouseButtonRelease:
      case QEvent::Wheel:
        djnn::release_exclusive_access (DBG_REL);
        exec_ = MyQWidget::event (event);
        return exec_;
        break;

      // case QEvent::KeyPress:
      // case QEvent::KeyRelease:
      // case QEvent::Close:
      // case QEvent::Paint:
      //   exec_ = QWidget::event (event);
      //   break;


      default:
        {
          /* Event not managed by us */
          djnn::release_exclusive_access (DBG_REL);
          return MyQWidget::event (event);
        }
      }
    djnn::release_exclusive_access (DBG_REL);
    //if(exec_) event->accept();
    return exec_;
  }


  static mouse_button
  get_button (int n)
  {
    mouse_button button_id = BUTTON_LEFT;
    switch (n)
      {
      case Qt::LeftButton:
        button_id = BUTTON_LEFT;
        break;
      case Qt::RightButton:
        button_id = BUTTON_RIGHT;
        break;
      case Qt::MiddleButton:
        button_id = BUTTON_MIDDLE;
        break;
      default:
        button_id = BUTTON_LEFT;
      }
    return button_id;
  }

  void
  MyQQWidget::mousePressEvent (QMouseEvent *event)
  {
//DBG;
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMousePress (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQQWidget::mouseMoveEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();
    bool exec_ = _picking_view->genericMouseMove (mouse_pos_x, mouse_pos_y);
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQQWidget::mouseReleaseEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMouseRelease (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
    if (exec_)
      QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQQWidget::wheelEvent (QWheelEvent *event)
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
  MyQQWidget::paintEvent (QPaintEvent *event)
  {
    DBG;

    QtBackend* backend = dynamic_cast<QtBackend*> (Backend::instance ());
    DisplayBackend::instance()->set_window (_window);
    QPainter painter (this);
    backend->set_painter (&painter);
    backend->set_picking_view (_picking_view);
    Process *p = _window->holder ();
    _picking_view->init ();
    if (p) {
#if _PERF_TEST
      t1();
#endif
      p->draw ();
#if _PERF_TEST
      // print in RED
      cerr << "\033[1;31m";
      double time = t2 ("DRAW : ");
      draw_counter = draw_counter + 1;
      draw_total = draw_total + time;
      draw_average = draw_total / draw_counter;
      cerr << "DRAW : " << draw_counter << " - avg: " << draw_average << endl;
      cerr << "\033[0m" << endl;
#endif
    }
    if (_picking_view->genericCheckShapeAfterDraw (mouse_pos_x, mouse_pos_y))
      QtMainloop::instance ().set_please_exec (true);

#if DEBUG_PICKING 
    _picking_view->display();
#endif
  }

}
