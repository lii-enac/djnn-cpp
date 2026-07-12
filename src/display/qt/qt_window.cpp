/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#include <iostream>

#include <QtCore/QAbstractEventDispatcher>
#include <QtGui/QMouseEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QOpenGLWidget>
//#include <type_traits> // is_same

#include "qt_window.h"

#include "core/control/blank.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/text_property.h"
#include "core/utils/error.h"
#include "display/display-priv.h"
#include "display/display.h" // mouse_tracking full_screen hide_pointer
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
#include "exec_env/qt/qt_mainloop.h"
#include "qt_display.h"
#include "qt_window_moc.h"
#include "utils/debug.h"

namespace djnn {

class djnnQEvent : public QEvent
{
  public:
    static const QEvent::Type user_event_geometry     = static_cast<QEvent::Type> (QEvent::User);
    static const QEvent::Type user_event_minimum_size = static_cast<QEvent::Type> (QEvent::User + 1);
};

QtWindow::QtWindow (Window* win, const string& title, double x, double y, double w, double h)
    : _qwidget (nullptr), _window (win), _please_update (true)
{
}

QtWindow::~QtWindow ()
{
    QtDisplayBackend::instance ()->remove_window (this);
    if (_qwidget != nullptr) {
        delete _qwidget;
    }
}

Qt::CursorShape QtWindow::cursor_shapes[22] = {Qt::ArrowCursor,
                                               Qt::UpArrowCursor,
                                               Qt::CrossCursor,
                                               Qt::WaitCursor,
                                               Qt::IBeamCursor,
                                               Qt::SizeVerCursor,
                                               Qt::SizeHorCursor,
                                               Qt::SizeBDiagCursor,
                                               Qt::SizeFDiagCursor,
                                               Qt::SizeAllCursor,
                                               Qt::BlankCursor,
                                               Qt::SplitVCursor,
                                               Qt::SplitHCursor,
                                               Qt::PointingHandCursor,
                                               Qt::ForbiddenCursor,
                                               Qt::OpenHandCursor,
                                               Qt::ClosedHandCursor,
                                               Qt::WhatsThisCursor,
                                               Qt::BusyCursor,
                                               Qt::DragMoveCursor,
                                               Qt::DragCopyCursor,
                                               Qt::DragLinkCursor};

void
QtWindow::impl_activate ()
{
    QtDisplayBackend::instance ()->add_window (this);
    _qwidget = dynamic_cast<QtDisplayBackend*> (DisplayBackend::instance ())->create_qwidget (_window, this);
    if (_qwidget) {
        WinImpl::set_picking_view (_qwidget->get_picking_view ());

        if (mouse_tracking)
            _qwidget->setMouseTracking (true);
        if (full_screen)
            _qwidget->setWindowState (_qwidget->windowState () ^ Qt::WindowFullScreen);
        if (hide_pointer)
            _qwidget->setCursor (Qt::BlankCursor);

        if (_window->pos_x ()->get_value () != -1 && _window->pos_y ()->get_value () != -1) {
            _qwidget->move(_window->pos_x ()->get_value (), _window->pos_y ()->get_value ());
        }
        if (_window->width ()->get_value () != -1 && _window->height ()->get_value () != -1) {
            _qwidget->resize(_window->width ()->get_value (), _window->height()->get_value ());
        }
        _window->set_background_transparency_and_color ();
        _qwidget->setParent (0); // Create TopLevel-Widget
        _qwidget->setWindowTitle (_window->title ()->get_value ().c_str ());
        _qwidget->set_building (true);
        _qwidget->show ();
        _qwidget->set_building (false);
    } else
        djnn_error (nullptr, "QtWindow::impl_activate () - Unable to create QtWindow");
}

uint32_t
QtWindow::get_pixel_color(int x, int y)
{
    if (_qwidget) {
        djnn::release_exclusive_access (DBG_GET);
        QPixmap pixmap = _qwidget->grab(QRect(x, y, 1, 1));
        djnn::get_exclusive_access (DBG_GET);
        QImage image = pixmap.toImage();
        QColor color = image.pixelColor(0, 0); 
        return color.rgba();   
    }
    return 0;
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
    _please_update = true;             // so remember it...
    QtMainloop::instance ().wakeup (); // ... and wake up qt
}

void
QtWindow::set_cursor (const string& path, int hotX, int hotY)
{
    if (_qwidget == nullptr) {
        return;
    }
    QPixmap bmp (path.c_str ());
    _qwidget->setCursor (QCursor (bmp, hotX, hotY));
}

void
QtWindow::set_cursor (int cursor_shape)
{
    if (_qwidget == nullptr) {
        return;
    }
    QCursor cursor (cursor_shapes[cursor_shape]);
    _qwidget->setCursor (cursor);
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
    if (_qwidget == nullptr) {
        return;
    }
    djnn::release_exclusive_access (DBG_GET); // => QEvent::spontaneous
    _qwidget->set_in_screenshot (true);
    _qwidget->grab ().save (QString ((path + ".png").c_str ()));
    _qwidget->set_in_screenshot (false);
    djnn::get_exclusive_access (DBG_GET);
}

void
QtWindow::set_opacity (double opacity)
{
    _qwidget->setWindowOpacity (opacity);
}

void
QtWindow::set_background_transparency_and_color (bool isTransparent, int r, int g, int b, bool isFrameless)
{
    // Note: If is_transparent is true, the color parameter is ignored 
    // and isFrameless is automatically set to true.

    if (isTransparent) {
        isFrameless = true ;
    }

    if (isFrameless)
        _qwidget->setWindowFlags (Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    else
        _qwidget->setWindowFlags (Qt::Widget);

    if (isTransparent) {
        // set fulltransparency 
        _qwidget->setAttribute (Qt::WA_TranslucentBackground, true);
    }
    else {
        // or set plain color
        QString str = QString("background-color: rgb(%1, %2, %3); border: none;")
                      .arg(r).arg(g).arg(b); 
        _qwidget->setStyleSheet(str);
    }
}

void
QtWindow::set_minimum_size (int w, int h)
{
    auto* evt = new QEvent (djnnQEvent::user_event_minimum_size);
    QtMainloop::instance ().get_QApplication ()->postEvent (_qwidget, evt);
}

void
QtWindow::update_geometry ()
{
    auto* evt = new QEvent (djnnQEvent::user_event_geometry);
    QtMainloop::instance ().get_QApplication ()->postEvent (_qwidget, evt);
}

void
QtWindow::update_geometry_for_good ()
{
    // If the UI is currently being updated by the user (dragging), ignore incoming backend commands.
    if (_qwidget->_updating_from_ui) return ;

    auto x = (int)_window->pos_x ()->get_value ();
    auto y = (int)_window->pos_y ()->get_value ();
    auto w = (int)_window->width ()->get_value ();
    auto h = (int)_window->height ()->get_value ();

   /*  Optimization: Check if the window is already at the requested geometry.
    * We compare against pos() (frame geometry) because move() targets the frame.
    */
    if (_qwidget->pos().x() == x && _qwidget->pos().y() == y &&
        _qwidget->width() == w && _qwidget->height() == h) {
        return;
    }

    _qwidget->_updating_from_backend_change = true;
    /* On macOS, move() targets the top-left corner of the window decorations (title bar),
     * while resize() targets the client area. This combination is more stable than setGeometry.
     */
    _qwidget->move(x, y);
    _qwidget->resize(w, h);
    _qwidget->_updating_from_backend_change = false;
}

void
QtWindow::set_minimum_size_for_good ()
{
    auto w = (int)_window->min_width ()->get_value ();
    auto h = (int)_window->min_height ()->get_value ();

    // TODO macOS: should be done in GUI thread...
    _qwidget->setMinimumSize (w, h);
}

// MyQWidget

MyQWidget::MyQWidget (Window* w, QtWindow* qtw)
    : QTWIDGET (nullptr), _window (w), _qtwindow (qtw), mouse_pos_x (-1), mouse_pos_y (-1), _building (false), _updating_from_ui (false), _updating_from_backend_change (false)
{
    setAttribute (Qt::WA_AcceptTouchEvents, true);

    //_picking_view = new QtPickingView (w);
    _in_screenshot = false;
}

void
MyQWidget::set_building (bool v)
{
    _building = v;
}

bool
MyQWidget::event (QEvent* event)
{

    /* note:
     * Get and release mutex on each event BUT only the events that WE manage
     * For other events we let Qt and QtWidgets dealing with them
     */

    bool exec_ = false;
    switch (event->type ()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::Move:
    case QEvent::Close:
        // case QEvent::Paint:
        if (!_building)
            djnn::get_exclusive_access (DBG_GET);
        exec_ = QTWIDGET::event (event); // should call our callback methods
        if (!_building)
            djnn::release_exclusive_access (DBG_REL);
        break;
    case QEvent::Resize:
        if (!_building)
            djnn::get_exclusive_access (DBG_GET);
        _in_resize=true;
        resizeEvent((QResizeEvent*)event);
        _in_resize=false;
        exec_ = true;
        if (!_building)
            djnn::release_exclusive_access (DBG_REL);
        break;
    case djnnQEvent::user_event_geometry:
        _qtwindow->update_geometry_for_good ();
        break;
    case djnnQEvent::user_event_minimum_size:
        _qtwindow->set_minimum_size_for_good ();
        break;

    default: {
        /* Event not managed by us */
        exec_ = QTWIDGET::event (event);
        break;
    }
    }

    return exec_;
}

void
MyQWidget::keyPressEvent (QKeyEvent* event)
{
    _window->key_pressed ()->set_value (event->key (), 1);
    ((GUIKeyboard*)GenericKeyboard)->key_pressed ()->set_value (event->key (), 1);
    if (!(event->key () >= 0x1000000 && event->key () <= 0x01020001)) {
        _window->key_pressed_text ()->set_value (event->text ().toStdString ().c_str (), 1);
        ((GUIKeyboard*)GenericKeyboard)->key_pressed_text ()->set_value (event->text ().toStdString ().c_str (), 1);
    }
    // QtMainloop::instance ().set_please_exec (true);
    // NO event synthesis on press, release
    GRAPH_EXEC;
}

void
MyQWidget::keyReleaseEvent (QKeyEvent* event)
{
    _window->key_released ()->set_value (event->key (), 1);
    ((GUIKeyboard*)GenericKeyboard)->key_released ()->set_value (event->key (), 1);
    _window->key_released_text ()->set_value (event->text ().toStdString ().c_str (), 1);
    ((GUIKeyboard*)GenericKeyboard)->key_released_text ()->set_value (event->text ().toStdString ().c_str (), 1);
    // QtMainloop::instance ().set_please_exec (true);
    //  NO event synthesis on press, release
    GRAPH_EXEC;
}

void
MyQWidget::moveEvent (QMoveEvent* event)
{
    // If the move was triggered by our own backend logic, ignore it to prevent infinite feedback loops.
    if (_updating_from_backend_change) return;

    _updating_from_ui = true;

    /* We use this->pos() instead of event->pos() because event->pos() can be ambiguous on some platforms 
     * (referring to the client area). this->pos() is guaranteed to return the frame's top-left corner, 
     * which remains consistent with the coordinates used by the move() method. 
     */
    const QPoint frame_pos = this->pos(); 
    
    int x = frame_pos.x ();
    int y = frame_pos.y ();

    _window->pos_x ()->set_value (x, 1);
    _window->pos_y ()->set_value (y, 1);

    _window->hidpi_scale ()->set_value (devicePixelRatioF (), true);

    _updating_from_ui = false;
    // event synthesis on move paint ...
    QtMainloop::instance ().set_please_exec (true);
    // GRAPH_EXEC;
}

void
MyQWidget::resizeEvent (QResizeEvent* event)
{

    #if USE_QOPENGL_WIDGET
        QOpenGLWidget::resizeEvent (event);
    #endif

    if (_updating_from_backend_change) return;
    _updating_from_ui = true;

    int w = event->size ().width ();
    int h = event->size ().height ();
    
    _window->width ()->set_value (w, true);
    _window->height ()->set_value (h, true);
    
    _updating_from_ui = false;
    GRAPH_EXEC;
}

void
MyQWidget::closeEvent (QCloseEvent* event)
{
    // close child should be notified but Qt wants the control
    // and will not let it do the job
    _window->close ()->schedule_activation ();
}

} // namespace djnn
