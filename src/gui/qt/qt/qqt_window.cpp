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

#include <QEvent>
#include <QTouchEvent>
#include <chrono>

#if USE_QOPENGL_WIDGET
#include <QOpenGLWidget>
//#include <type_traits> // is_same
#endif

#include "core/control/blank.h"
#include "core/utils/iostream.h"
#include "core/utils/remotery.h"
#include "core/utils/utils-dev.h"

#include "display/abstract_display.h"
#include "display/background_color.h"
#include "display/qt/qt_window.h"

#include "exec_env/exec_env-dev.h"
#include "exec_env/global_mutex.h"
#include "exec_env/qt/qt_mainloop.h"

#include "gui/backend.h"
#include "gui/gui-dev.h"
#include "gui/qt/qt_backend.h"
#include "gui/qt/qt_context.h"
#include "qqt_window_moc.h"

// #include "core/utils/iostream.h"
#include "core/utils/error.h"
#include "utils/debug.h"

#if _DEBUG_SEE_GUI_INFO_PREF
// #include "core/utils/utils-dev.h"

static int    draw_counter = 0;
static double draw_total   = 0.0;
static double draw_average = 0.0;
#endif

#define test_between_2_paintevents 0

namespace djnn {
using djnnstl::cerr;
using djnnstl::cout;
using djnnstl::endl;


// helper functions

static mouse_button
get_button (int n)
{
    mouse_button button_id = BUTTON_LEFT;
    switch (n) {
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

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))

static stylus_type
get_stylus_type (QPointingDevice::PointerType t)
{
    stylus_type type = STYLUS_PEN;
    switch (t) {
    case QPointingDevice::PointerType::Eraser:
        type = STYLUS_ERASER;
        break;
    case QPointingDevice::PointerType::Pen:
        type = STYLUS_PEN;
        break;
    default:
        type = STYLUS_PEN;
    }
    return type;
}

template <typename X>
QPointF
get_position(const X* event)
{
    return event->position();
}

template <typename X>
const X&
get_position(const QPoint* event)
{
    return *event;
}

const QList<QTouchEvent::TouchPoint>&
get_touch_points(const QTouchEvent* event)
{
    return event->points ();
}

#else

static stylus_type
get_stylus_type (int t)
{
    stylus_type type = STYLUS_PEN;
    switch (t) {
    case QTabletEvent::Eraser:
        type = STYLUS_ERASER;
        break;
    case QTabletEvent::Pen:
        type = STYLUS_PEN;
        break;
    default:
        type = STYLUS_PEN;
    }
    return type;
}




template <typename X>
const X&
get_position(const X* event)
{
    return *event; //->position();
}

QPointF
get_position(const QTouchEvent::TouchPoint* event)
{
    return event->pos();
}

const QList<QTouchEvent::TouchPoint>&
get_touch_points(const QTouchEvent* event)
{
    return event->touchPoints ();
}
#endif

inline
bool
is_move_event(QEvent* event)
{
    switch (event->type ()) {
        case QEvent::MouseMove:
        case QEvent::TabletMove:
        case QEvent::NonClientAreaMouseMove:
            return true;
        case QEvent::TouchUpdate: {
            const auto& touchPoints = get_touch_points(static_cast<QTouchEvent*> (event));

            for (const auto& touchPoint : touchPoints) {
                switch (touchPoint.state ()) {
                    case Qt::TouchPointMoved:
                        return true;
                    default:
                        break;
                }
            }
            return false;
        }
        default:
            return false;
    }
}

bool
MyQQWidget::event (QEvent* event)
{
    // cerr << ">> " << __PRETTY_FUNCTION__ << " " << event->type () << endl;
    /* note:
     * Get and release Mutex on each event BUT only the events that
     * WE manage else we let Qt and QTwidgets dealing with these Events.
     */

    // if(_building) cerr << "building" << endl;
    // if(!_building)
    // djnn::get_exclusive_access (DBG_GET);

    if (!_qwidget_event_call_level) {
        rmt_BeginCPUSample(external_source_gui, 0);
        #ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE_2) {
            if ( !is_move_event(event) || _DEBUG_SEE_ACTIVATION_SEQUENCE_2_MOVE)
                std::cerr << ">> " << "external_source_gui " << event->type () << std::endl;
        }
        extern bool _OUTSIDE_GRAPH_EXE;
        _OUTSIDE_GRAPH_EXE = true;
        #endif
    }
    ++_qwidget_event_call_level;

    bool exec_ = false;
    switch (event->type ()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        // if(!_building)
        djnn::get_exclusive_access (DBG_GET);
        // const auto& touchPoints = static_cast<QTouchEvent*> (event)->touchPoints ();
        // const auto& touchPoints = static_cast<QTouchEvent*> (event)->points ();
        const auto& touchPoints = get_touch_points(static_cast<QTouchEvent*> (event));

        for (const auto& touchPoint : touchPoints) {
            int    id       = touchPoint.id ();
            //double x        = touchPoint.pos ().x ();
            //double y        = touchPoint.pos ().y ();
            //double x        = touchPoint.position ().x ();
            //double y        = touchPoint.position ().y ();
            double x = get_position (&touchPoint).x ();
            double y = get_position (&touchPoint).y ();
            double pressure = touchPoint.pressure ();
            switch (touchPoint.state ()) {
            case Qt::TouchPointStationary:
                break;
            case Qt::TouchPointPressed: {
                if (_ENABLE_TOUCHES == 1) {
                    exec_ |= _picking_view->genericTouchPress (x, y, id, pressure);
                    // NO event synthesis on press, release
                    if (exec_) {
                        GRAPH_EXEC;
                    }
                }
                break;
            }
            case Qt::TouchPointMoved: {
                if (_ENABLE_TOUCHES == 1) {
                    exec_ |= _picking_view->genericTouchMove (x, y, id, pressure);
                    // NO event synthesis on move
                    if (exec_) {
                        GRAPH_EXEC;
                    }
                }
                break;
            }
            case Qt::TouchPointReleased: {
                if (_ENABLE_TOUCHES == 1) {
                    exec_ |= _picking_view->genericTouchRelease (x, y, id, pressure);
                    if (exec_) {
                        GRAPH_EXEC;
                    }
                }
                break;
            }
            }
        }
        // if (exec_) QtMainloop::instance ().set_please_exec (true);
        // if(!_building)
        djnn::release_exclusive_access (DBG_REL);
    } break;

    case QEvent::TabletPress:
    case QEvent::TabletMove:
    case QEvent::TabletRelease:
        djnn::get_exclusive_access (DBG_GET);
        tabletEvent (static_cast<QTabletEvent*> (event));
        event->accept (); // We have to tell Qt that we consumed the event to prevent it from generating a mouse event.
        djnn::release_exclusive_access (DBG_REL);
        break;

    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
    case QEvent::Wheel:
    case QEvent::Paint:
    // case QEvent::UpdateRequest:
        //if (_in_resize) break;
        if (!(_building || _in_resize)) // || event->spontaneous ())) // spontaneous: could be a screenshot request
            djnn::get_exclusive_access (DBG_GET);
        exec_ = MyQWidget::event (event);
        if (!_in_screenshot) //(!event->spontaneous ()) // spontaneous: could be a screenshot request )
            _window->refreshed ()->notify_activation ();
        if (!(_building || _in_resize)) // || event->spontaneous ()))
            djnn::release_exclusive_access (DBG_REL);
        // return exec_;
        break;
    case QEvent::Close: {
        djnn::get_exclusive_access (DBG_GET);
        event->ignore ();
        FatProcess* p = _window->close ();
        if (p != nullptr) {
            p->activate ();
            // we may want to react to close : before closing
            QtMainloop::instance ().set_please_exec (true);
        }
        djnn::release_exclusive_access (DBG_REL);
    } break;
    default: {
        /* Event not managed by us */
        // djnn::release_exclusive_access (DBG_REL);
        // if(!_building)
        // djnn::get_exclusive_access (DBG_GET);
        exec_ = MyQWidget::event (event);
        // if(!_building)
        // djnn::release_exclusive_access (DBG_REL);
        break;
    }
    }

    // if(!_building)
    // djnn::release_exclusive_access (DBG_REL);
    if (exec_)
        event->accept ();
    // cerr << "<< " << __PRETTY_FUNCTION__ << " " << event->type () << endl;

    --_qwidget_event_call_level;
    if (!_qwidget_event_call_level) {
        rmt_EndCPUSample();
        #ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE_2) {
            if ( !is_move_event(event))// || _DEBUG_SEE_ACTIVATION_SEQUENCE_2_MOVE)
                std::cerr << "<< " << "external_source_gui" << std::endl;
        }
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE_2) {
            extern bool _OUTSIDE_GRAPH_EXE;
            _OUTSIDE_GRAPH_EXE = false;
        }
        #endif
    }
    
    return exec_;
}

static std::chrono::steady_clock::time_point __lastClickTime;

void
MyQQWidget::mousePressEvent (QMouseEvent* event)
{
    mouse_pos_x = get_position (event).x ();
    mouse_pos_y = get_position (event).y ();
    mouse_pos_y -= 3; // QT bug??

    bool exec_;

    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - __lastClickTime).count();

    // must be generated before the press
    if (delta < _doubleClickIntervalMs && delta > 0) {  // Avoid false positives when delta = 0
            exec_ = _picking_view->genericMouseDoubleClick (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
            __lastClickTime = {};  // Reset to avoid triple-clicks
    } else 
        __lastClickTime = now;

    exec_ = _picking_view->genericMousePress (mouse_pos_x, mouse_pos_y, get_button (event->button ()));

    if (exec_) {
        GRAPH_EXEC;
    }
}

void
MyQQWidget::mouseMoveEvent (QMouseEvent* event)
{
    mouse_pos_x = get_position (event).x ();
    mouse_pos_y = get_position (event).y ();
    mouse_pos_y -= 3; // QT bug??

    bool exec_ = _picking_view->genericMouseMove (mouse_pos_x, mouse_pos_y);
    if (exec_) {
        GRAPH_EXEC;
    }
}

void
MyQQWidget::mouseReleaseEvent (QMouseEvent* event)
{
    mouse_pos_x = get_position (event).x ();
    mouse_pos_y = get_position (event).y ();
    mouse_pos_y -= 3; // QT bug??

    bool exec_ = _picking_view->genericMouseRelease (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
    if (exec_) {
        GRAPH_EXEC;
    }
}

void
MyQQWidget::wheelEvent (QWheelEvent* event)
{
    QPoint numPixels  = event->pixelDelta ();
    QPoint numDegrees = event->angleDelta () / 8; // the angle is in eights of a degree
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    auto mouse_pos_x = event->position ().x ();
    auto mouse_pos_y = event->position ().y ();
#else
    auto mouse_pos_x = event->pos ().x ();
    auto mouse_pos_y = event->pos ().y ();
#endif
    mouse_pos_y -= 3; // QT bug??

    bool exec_ = false;
    if (!numPixels.isNull ()) {
        // cerr << "WHEEL Pixel " << numPixels.x () << " - " << numPixels.y () << endl;
        exec_ = _picking_view->genericMouseWheel (numPixels.x (), numPixels.y (), mouse_pos_x, mouse_pos_y);
    } else if (!numDegrees.isNull ()) {
        QPoint numSteps = numDegrees / 15;
        // cerr << "WHEEL Degree " << numSteps.x () << " - " << numSteps.y () << endl;
        exec_ = _picking_view->genericMouseWheel (numSteps.x (), numSteps.y (), mouse_pos_x, mouse_pos_y);
    }

    if (exec_) {
        GRAPH_EXEC;
    }
}

void
MyQQWidget::tabletEvent (QTabletEvent* event)
{
    auto tablet_pos_x = get_position (event).x ();
    auto tablet_pos_y = get_position (event).y ();
    tablet_pos_y -= 3; // QT bug?? it is the same for stylus
    auto pressure = event->pressure ();

    if (_STYLUS_EMULATE_MOUSE == 1) {
        mouse_pos_x = tablet_pos_x;  // !! mouse_pos_x and mouse_pos_y are use in genericCheckShapeAfterDraw
        mouse_pos_y = tablet_pos_y;
    }

    // dbg
    // std::cerr << "x: " << tablet_pos_x << " - y: " << tablet_pos_y <<  " - pointer type: " << event->pointerType () << "- pressure: " << pressure << std::endl;

    bool exec_ = false;

    switch (event->type ()) {
    case QEvent::TabletPress:
        if (_STYLUS_EMULATE_MOUSE == 1)
            exec_ = _picking_view->genericMousePress (tablet_pos_x, tablet_pos_y, get_button (event->button ()));
        else
            exec_ = _picking_view->genericTabletPress (tablet_pos_x, tablet_pos_y, get_stylus_type (event->pointerType ()), get_button (event->button ()), pressure);
        break;
    case QEvent::TabletMove:
        if (_STYLUS_EMULATE_MOUSE == 1)
            exec_ = _picking_view->genericMouseMove (tablet_pos_x, tablet_pos_y);
        else
            exec_ = _picking_view->genericTabletMove (tablet_pos_x, tablet_pos_y, get_stylus_type (event->pointerType ()), pressure);
        break;
    case QEvent::TabletRelease:
        if (_STYLUS_EMULATE_MOUSE == 1)
            exec_ = _picking_view->genericMouseRelease (tablet_pos_x, tablet_pos_y, get_button (event->button ()));
        else
            exec_ = _picking_view->genericTabletRelease (tablet_pos_x, tablet_pos_y, get_stylus_type (event->pointerType ()), get_button (event->button ()), pressure);
        break;
    default:
        break;
    }

    if (exec_) {
        GRAPH_EXEC;
    }
}

/*void
MyQQWidget::initializeGL ()
{
  initializeOpenGLFunctions();
}*/

// remotery test between_2_paintevent part 1
#if test_between_2_paintevents
static bool first_time = true;
#endif

void
MyQQWidget::paintEvent (QPaintEvent* event)
{ // DBG;

    if (_DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT)
        cerr << endl
             << " PaintEvent " << endl;

#if test_between_2_paintevents
    if (first_time)
        first_time = false;
    else
        rmt_EndCPUSample (); // end of between 2 paintevent

    rmt_BeginCPUSample (between_2_paintEvent, RMTSF_None);
#endif

    rmt_BeginCPUSample (paintEvent, RMTSF_None);

    rmt_BeginCPUSample (pre_draw, RMTSF_None);
    QtBackend* backend = dynamic_cast<QtBackend*> (Backend::instance ());
    DisplayBackend::instance ()->set_window (_window);
    // glClear (GL_COLOR_BUFFER_BIT);
    QPainter painter (this);
    // if constexpr (QTWIGDET==QOpenGLWidget)
    //if constexpr (std::is_same<QTWIDGET, QOpenGLWidget> ()) {
    #if USE_QOPENGL_WIDGET
        painter.fillRect (0, 0, width (), height (), QColor (
            //255, // for testing purpose
            _window->background_color ()->r ()->get_value (),
            _window->background_color ()->g ()->get_value (),
            _window->background_color ()->b ()->get_value ()
            //_window->background_opacity ()->get_value ()
        ));
    #endif
    //}

    backend->set_painter (&painter);
    backend->set_picking_view (_picking_view);
    FatProcess* p = _window->holder ();
    rmt_EndCPUSample (); // pre_draw

    rmt_BeginCPUSample (picking_view_init, RMTSF_None);
    _picking_view->init ();
    rmt_EndCPUSample ();
    if (p) {

        t1 ();
        rmt_BeginCPUSample (draw, RMTSF_None);

        p->draw ();

        rmt_EndCPUSample ();
        double time = t2 ("");
        _window->mspf ()->set_value (time, true);

#if _DEBUG_SEE_GUI_INFO_PREF
        cerr << "\033[1;36m";
        cerr << "DRAW: " << time << endl;
        draw_counter = draw_counter + 1;
        draw_total   = draw_total + time;
        draw_average = draw_total / draw_counter;
        cerr << "DRAW: " << draw_counter << " - avg: " << draw_average << endl;
        cerr << "\033[0m" << endl;
#endif
    }

    rmt_BeginCPUSample (post_draw, RMTSF_None);
    if (_picking_view->genericCheckShapeAfterDraw (mouse_pos_x, mouse_pos_y)) {
        GRAPH_EXEC;
    }

    if (_DEBUG_SEE_COLOR_PICKING_VIEW)
        _picking_view->display ();
    rmt_EndCPUSample (); // post_draw

    rmt_EndCPUSample (); // paintEvent
}

} // namespace djnn
