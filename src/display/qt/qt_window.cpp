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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "qt_window_moc.h"
#include "qt_window.h"
#include "qt_display.h"

#include "utils/debug.h"

#include "core/tree/blank.h"

#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"

#include "exec_env/qt/qt_mainloop.h"
#include "display/display.h" // mouse_tracking full_screen hide_pointer
#include "display/display-dev.h"
#include "display/abstract_display.h"

#include "exec_env/main_loop.h"
#include "exec_env/global_mutex.h"
#include "core/core-dev.h" // graph add/remove edge

#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QBitmap>

#include "core/utils/error.h"

namespace djnn
{

  enum {
    user_event_geometry = QEvent::User
  };

  QtWindow::QtWindow (Window *win, const string& title, double x, double y, double w, double h) :
      _qwidget (nullptr), _window (win), _please_update (true)
  {
  }

  QtWindow::~QtWindow ()
  {
    QtDisplayBackend::instance ()->remove_window (this);
    if (_qwidget != nullptr) {
      delete _qwidget;
    }
  }

  void
  QtWindow::impl_activate ()
  {
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
    _qwidget->setGeometry (rect);
    _window->set_background_opacity_and_color();
     _qwidget->setParent(0); // Create TopLevel-Widget
    _qwidget->setWindowTitle (_window->title ()->get_value ().c_str ());
    _qwidget->set_building(true);
    _qwidget->show ();
    _qwidget->set_building(false);
  }

  void
  QtWindow::impl_deactivate ()
  {
    QtDisplayBackend::instance ()->remove_window (this);
    delete _qwidget;
    _qwidget = nullptr;
  }

  void
  QtWindow::update ()
  {
    if (_qwidget == nullptr) {
      return;
    }
    //_qwidget->update (); // won't work since qt is blocked in mainloop
    _please_update = true; // so remember it...
    QtMainloop::instance ().wakeup (); // ... and wake up qt
  }

  void
  QtWindow::set_cursor (const string& path, int hotX, int hotY) {
    if (_qwidget == nullptr) { return; }
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
  QtWindow::perform_screenshot (const string& path)
  {
    if (_qwidget == nullptr) { return; }
    djnn::release_exclusive_access (DBG_GET); // => QEvent::spontaneous
    _qwidget->set_in_screenshot (true);
    _qwidget->grab().save(QString((path+".png").c_str()));
    _qwidget->set_in_screenshot (false);
    djnn::get_exclusive_access (DBG_GET);
  }

  void
  QtWindow::set_opacity (double opacity)
  {
    _qwidget->setWindowOpacity (opacity);
  }

  void
  QtWindow::set_background_opacity_and_color (double opacity, int r, int g, int b)
  {
    if( opacity != -1 ) {  // if not the init value
      opacity = opacity * 255;
      _qwidget->setWindowFlags ( Qt::Widget | Qt::FramelessWindowHint );
        //_qwidget->setAttribute (Qt::WA_TranslucentBackground, is_transparent); //useless
      
      QString str = QString ("background-color: rgba(%1, %2, %3, %4)").arg (QString::number(r), QString::number(g), QString::number(b), QString::number(opacity));
      //_qwidget->setStyleSheet("background-color: rgba(255, 255, 255, 0)");
      //std::cout << str.toStdString () << endl;
      _qwidget->setStyleSheet(str);
      
      _qwidget->show ();  // if we want it interactive
    }
  }

  void
  QtWindow::update_geometry ()
  {
    auto * evt = new QEvent((QEvent::Type)user_event_geometry);
    QtMainloop::instance ().get_QApplication ()->postEvent (_qwidget, evt);
  }

  void
  QtWindow::update_geometry_for_good ()
  {
    auto x = (int) _window->pos_x()->get_value ();
    auto y = (int) _window->pos_y()->get_value ();
    auto w = (int) _window->width()->get_value ();
    auto h = (int) _window->height()->get_value ();

    // TODO macOS: should be done in GUI thread...
    _qwidget->setGeometry (x,y,w,h);
  }



  // MyQWidget

  void
  MyQWidget::set_building(bool v)
  {
    _building = v;
  }

  bool
  MyQWidget::event (QEvent *event)
  {

    /* note:
     * Get and release mutex on each event BUT only the events that WE manage
     * For other events we let Qt and QtWidgets dealing with them
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
      case user_event_geometry:
        _qtwindow->update_geometry_for_good ();
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
  {
    _window->key_pressed ()->set_value (event->key (), 1);
    if (!(event->key() >= 0x1000000 && event->key() <= 0x01020001)) {
      _window->key_pressed_text ()->set_value (event->text ().toStdString ().c_str (), 1);
    }
    // QtMainloop::instance ().set_please_exec (true);
    // NO event synthesis on press, release
    GRAPH_EXEC;
  }

  void
  MyQWidget::keyReleaseEvent (QKeyEvent *event)
  {
    _window->key_released ()->set_value (event->key (), 1);
    _window->key_released_text ()->set_value (event->text ().toStdString ().c_str(), 1);
    //QtMainloop::instance ().set_please_exec (true);
    // NO event synthesis on press, release
    GRAPH_EXEC;
  }

  void
  MyQWidget::moveEvent (QMoveEvent *event)
  {
    if (_updating) {
      return;
    }
    _updating = true;

    const QPoint pos = event->pos ();
    int x = pos.x ();
    int y = pos.y ();
    _window->pos_x ()->set_value (x, 1);
    _window->pos_y ()->set_value (y, 1);

    _updating = false;
    // event synthesis on move paint ...
    QtMainloop::instance ().set_please_exec (true);
  }

  void
  MyQWidget::resizeEvent (QResizeEvent * event)
  {
    if (_updating) { return; }
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
