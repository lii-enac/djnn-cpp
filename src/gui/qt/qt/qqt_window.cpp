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

#include "gui/backend.h"
#include "gui/qt/qt_backend.h"

#include "core/tree/blank.h"
#include "core/utils/utils-dev.h"

#include "display/qt/qt_window.h"
#include "qqt_window_moc.h"
#include "display/abstract_display.h"
#include "exec_env/qt/qt_mainloop.h"
#include "exec_env/global_mutex.h"
#include "core/utils/ext/remotery/Remotery.h"

#include <QEvent>
#include <QTouchEvent>
#include <QOpenGLWidget>

#include "exec_env/exec_env-dev.h"

#include "core/utils/iostream.h"
#include "core/utils/error.h"
#include "utils/debug.h"

 #include <type_traits> // is_same

#if _DEBUG_SEE_GUI_INFO_PREF
//#include "core/utils/utils-dev.h"

static int draw_counter = 0;
static double draw_total = 0.0;
static double draw_average = 0.0;
#endif

#define test_between_2_paintevents 0

namespace djnn
{
  bool
  MyQQWidget::event (QEvent *event)
  {
    //std::cerr << ">> " << __PRETTY_FUNCTION__ << " " << event->type () << std::endl;
    /* note:
     * Get and release Mutex on each event BUT only the events that 
     * WE manage else we let Qt and QTwidgets dealing with these Events.
     */

    //if(_building) std::cerr << "building" << std::endl;
    //if(!_building)
    //djnn::get_exclusive_access (DBG_GET);

    bool exec_ = false;
    switch (event->type ())
      {
      case QEvent::TouchBegin:
      case QEvent::TouchUpdate:
      case QEvent::TouchEnd:
        {
          //if(!_building)
          djnn::get_exclusive_access (DBG_GET);
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
                  // NO event synthesis on press, release
                  if (exec_ ) GRAPH_EXEC;
                  break;
                }
              case Qt::TouchPointMoved:
                {
                  exec_ |= _picking_view->genericTouchMove (x, y, id, pressure);
                  // event synthesis on move, paint
                  if (exec_) QtMainloop::instance ().set_please_exec (true);
                  break;
                }
              case Qt::TouchPointReleased:
                {
                  exec_ |= _picking_view->genericTouchRelease (x, y, id, pressure);
                  // NO event synthesis on press, release
                  if (exec_ ) GRAPH_EXEC;
                  break;
                }
              }
          }
          //if (exec_) QtMainloop::instance ().set_please_exec (true);
          //if(!_building)
          djnn::release_exclusive_access (DBG_REL);
        }
        break;

      case QEvent::MouseButtonPress:
      case QEvent::MouseButtonDblClick:
      case QEvent::MouseMove:
      case QEvent::MouseButtonRelease:
      case QEvent::Wheel:
      case QEvent::Paint:
      //case QEvent::UpdateRequest:
        if(! (_building)) // || event->spontaneous ())) // spontaneous: could be a screenshot request 
          djnn::get_exclusive_access (DBG_GET);
        exec_ = MyQWidget::event (event);
        if (!_in_screenshot)//(!event->spontaneous ()) // spontaneous: could be a screenshot request )
          _window->refreshed ()->notify_activation ();
        if(! (_building)) // || event->spontaneous ()))
          djnn::release_exclusive_access (DBG_REL);
        //return exec_;
        break;
      case QEvent::Close:
      {
        djnn::get_exclusive_access (DBG_GET);
        event->ignore ();
        FatProcess *p = _window->close();
        if (p != nullptr) {
          p->activate ();
          // we may want to react to close : before closing
          QtMainloop::instance ().set_please_exec (true);
        }
        djnn::release_exclusive_access (DBG_REL);
      }
      break;
      default:
        {
          /* Event not managed by us */
          //djnn::release_exclusive_access (DBG_REL);
          //if(!_building)
          //djnn::get_exclusive_access (DBG_GET);
          exec_ = MyQWidget::event (event);
          //if(!_building)
          //djnn::release_exclusive_access (DBG_REL);
          break;
        }
      }

    //if(!_building)
    //djnn::release_exclusive_access (DBG_REL);
    //if(exec_) event->accept();
    //std::cerr << "<< " << __PRETTY_FUNCTION__ << " " << event->type () << std::endl;
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
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMousePress (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
    if (exec_)
      // QtMainloop::instance ().set_please_exec (true);
      // NO event synthesis on press, release
      GRAPH_EXEC;
  }

  void
  MyQQWidget::mouseMoveEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMouseMove (mouse_pos_x, mouse_pos_y);
    if (exec_)
      // event synthesis for move, paint ...
#if test_between_2_paintevents
      GRAPH_EXEC; //remotery test between_2_paintevent part 1
#else
      QtMainloop::instance ().set_please_exec (true);
#endif
      
  }

  void
  MyQQWidget::mouseReleaseEvent (QMouseEvent *event)
  {
    mouse_pos_x = event->x ();
    mouse_pos_y = event->y ();

    bool exec_ = _picking_view->genericMouseRelease (mouse_pos_x, mouse_pos_y, get_button (event->button ()));
    if (exec_)
      // QtMainloop::instance ().set_please_exec (true);
      // NO event synthesis on press, release
      GRAPH_EXEC;
  }

  void
  MyQQWidget::wheelEvent (QWheelEvent *event)
  {
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8; // the angle is in eights of a degree

    bool exec_ = false ;
    if (!numPixels.isNull()) {
        //std::cerr << "WHEEL Pixel " << numPixels.x () << " - " << numPixels.y () << std::endl;
        exec_ = _picking_view->genericMouseWheel (numPixels.x (), numPixels.y ());  
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        //std::cerr << "WHEEL Degree " << numSteps.x () << " - " << numSteps.y () << std::endl;
        exec_ = _picking_view->genericMouseWheel (numSteps.x (), numSteps.y ());
    }

    if (exec_) { GRAPH_EXEC; } 
  }

  /*void
  MyQQWidget::initializeGL ()
  {
    initializeOpenGLFunctions();
  }*/


  //remotery test between_2_paintevent part 1
  #if test_between_2_paintevents
  static bool first_time = true;
  #endif

  void
  MyQQWidget::paintEvent (QPaintEvent *event)
  { //DBG;

#if test_between_2_paintevents
    if (first_time)
      first_time = false ;
    else
      rmt_EndCPUSample (); // end of between 2 paintevent
      
    rmt_BeginCPUSample(between_2_paintEvent, RMTSF_None);
#endif

    rmt_BeginCPUSample(paintEvent, RMTSF_None);
    
    rmt_BeginCPUSample (pre_draw, RMTSF_None);
    QtBackend* backend = dynamic_cast<QtBackend*> (Backend::instance ());
    DisplayBackend::instance()->set_window (_window);
    //glClear (GL_COLOR_BUFFER_BIT);
    QPainter painter (this);
    //if constexpr (QTWIGDET==QOpenGLWidget)
    if constexpr (std::is_same<QTWIDGET,QOpenGLWidget>())
      painter.fillRect(0,0,width(),height(),QColor(0,0,0));
    
    backend->set_painter (&painter);
    backend->set_picking_view (_picking_view);
    FatProcess *p = _window->holder ();
    rmt_EndCPUSample (); // pre_draw

    rmt_BeginCPUSample(picking_view_init, RMTSF_None);
    _picking_view->init ();
    rmt_EndCPUSample ();
    if (p) {

      t1();
      rmt_BeginCPUSample(draw, RMTSF_None);
    
      p->draw ();

      rmt_EndCPUSample ();
      double time = t2 ("");
      _window->mspf ()->set_value(time, true);

#if _DEBUG_SEE_GUI_INFO_PREF
      cerr << "\033[1;36m";
      cerr << "DRAW: " << time << endl; 
      draw_counter = draw_counter + 1;
      draw_total = draw_total + time;
      draw_average = draw_total / draw_counter;
      cerr << "DRAW: " << draw_counter << " - avg: " << draw_average << endl;
      cerr << "\033[0m" << endl;
#endif
    }

    rmt_BeginCPUSample (post_draw, RMTSF_None);
    if (_picking_view->genericCheckShapeAfterDraw (mouse_pos_x, mouse_pos_y))
      // event synthesis or not ??
      QtMainloop::instance ().set_please_exec (true);

    if (_DEBUG_SEE_COLOR_PICKING_VIEW)
      _picking_view->display();
    rmt_EndCPUSample (); //post_draw

    rmt_EndCPUSample (); // paintEvent
  }

}
