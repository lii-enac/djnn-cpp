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

//#include "backend.h"

#include "my_qwindow.h"
#include "qt_window.h"
#include "qt_display.h"

#include "utils/debug.h"

#include "exec_env/qt/qt_mainloop.h"
#include "display/display.h"
#include "display/display-dev.h"
#include "display/abstract_display.h"

#include "exec_env/main_loop.h"
#include "exec_env/global_mutex.h"
#include "core/execution/graph.h"

#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QBitmap>

//#include <iostream>


namespace djnn
{
  QtWindow::QtWindow (Window *win, const std::string& title, double x, double y, double w, double h) :
      _qwidget (nullptr), _window (win), _please_update (true)
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
  }

  QtWindow::~QtWindow ()
  {
    //QtMainloop::instance ().remove_window (this);
    QtDisplayBackend::instance ()->remove_window (this);
    if (_qwidget != nullptr)
      delete _qwidget;
  }

  void
  QtWindow::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << std::endl;
    QtDisplayBackend::instance ()->add_window (this);
    _qwidget = dynamic_cast<QtDisplayBackend*>(DisplayBackend::instance())->create_qwidget(_window, this);
    WinImpl::set_picking_view(_qwidget->get_picking_view());

    if (mouse_tracking)
      _qwidget->setMouseTracking (true);
    if (full_screen)
      _qwidget->setWindowState (_qwidget->windowState () ^ Qt::WindowFullScreen);
    if (hide_pointer)
      _qwidget->setCursor(Qt::BlankCursor);

    QRect rect (_window->pos_x ()->get_value (), _window->pos_y ()->get_value (), _window->width ()->get_value (),
                _window->height ()->get_value ());

    _qwidget->set_building(true);
    _qwidget->setGeometry (rect);
    _qwidget->setWindowTitle (_window->title ()->get_value ().c_str ());
    _qwidget->show ();
    _qwidget->set_building(false);
  }

  void
  QtWindow::impl_deactivate ()
  {
    //QtMainloop::instance ().remove_window (this);
    QtDisplayBackend::instance ()->remove_window (this);
    delete _qwidget;
    _qwidget = nullptr;
  }

  void
  QtWindow::update ()
  { //DBG;
    if (_qwidget == nullptr)
      return;
    //_qwidget->update (); // won't work since qt is blocked in mainloop
    _please_update = true; // so remind this...
    QtMainloop::instance ().wakeup (); // ... and wake up qt
  }

  void
  QtWindow::set_cursor (const std::string& path, int hotX, int hotY) {
    if (_qwidget == nullptr)
      return;
    QBitmap bmp (path.c_str ());
    _qwidget->setCursor (QCursor (bmp, hotX, hotY));
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
  MyQWidget::set_building(bool v)
  {
    _building = v;
  }

  bool
  MyQWidget::event (QEvent *event)
  {

    /* note:
     * Get and release Mutex on each event BUT only the events that 
     * WE manage else we let Qt and QTwidgets dealing with these Events.
     */
    
    bool exec_ = false;
    switch (event->type ())
    {
      case QEvent::KeyPress:
      case QEvent::KeyRelease:
      case QEvent::Move:
      case QEvent::Resize:
      case QEvent::Close:
      //case QEvent::Paint:
        if(!_building)
        djnn::get_exclusive_access (DBG_GET);
        exec_ = QWidget::event (event); // should call our callback methods
        if(!_building)
        djnn::release_exclusive_access (DBG_REL);
        break;

      default:
        {
          /* Event not managed by us */      
          exec_ = QWidget::event (event);
          break;
        }
      }
    
    return exec_;
  }

  void
  MyQWidget::keyPressEvent (QKeyEvent *event)
  { //DBG;
    _window->key_pressed ()->set_value (event->key (), 1);
    if (!(event->key() >= 0x1000000 && event->key() <= 0x01020001)) {
      _window->key_pressed_text ()->set_value (event->text ().toStdString (), 1);
    }
    QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::keyReleaseEvent (QKeyEvent *event)
  {
    _window->key_released ()->set_value (event->key (), 1);
    _window->key_released_text ()->set_value (event->text ().toStdString (), 1);
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
   GRAPH_EXEC;
  }

  void
  MyQWidget::closeEvent (QCloseEvent *event)
  {
// close child should be notified but Qt wants the control
// and will not let it do the job
    _window->close ()->notify_activation ();
  }

}
