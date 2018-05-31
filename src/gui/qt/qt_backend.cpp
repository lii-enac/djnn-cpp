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

#if QT

#include "qt_context.h"
#include "qt_backend.h"
#include "qt_window.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include <iostream>
#include <cmath>

namespace djnn
{
  std::shared_ptr<QtBackend> QtBackend::_instance;
  std::once_flag QtBackend::onceFlag;

  QtBackend*
  QtBackend::instance ()
  {
    std::call_once (QtBackend::onceFlag, [] () {
      _instance.reset(new QtBackend);
    });

    return _instance.get ();
  }

  QtBackend::QtBackend () :
      _painter (nullptr), _picking_view (nullptr)
  {
    _context_manager = make_unique<QtContextManager> ();
  }

  QtBackend::~QtBackend ()
  {
  }

  void
  QtBackend::save_context ()
  {
    if (_painter != nullptr)
      _painter->save ();
  }

  void
  QtBackend::restore_context ()
  {
    if (_painter != nullptr)
      _painter->restore ();
  }
  void
  QtBackend::set_painter (QPainter* p)
  {
    _painter = p;
  }

  void
  QtBackend::set_picking_view (QtPickingView* p)
  {
    _picking_view = p;
  }

  /* Qt context management is imported from djnn v1 */
  void
  QtBackend::load_drawing_context (double tx, double ty, double width, double height)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QMatrix4x4 matrix = cur_context->matrix;
    QTransform transform = matrix.toTransform ();
    /* setup the painting environment of the drawing view */
    QPen tmpPen (cur_context->pen);
    /*
     * with Qt, dash pattern and dash offset are specified
     * in stroke-width unit so we need to update the values accordingly
     */
    if (cur_context->pen.style () == Qt::CustomDashLine) {
      qreal w = cur_context->pen.widthF () || 1;
      QVector<qreal> dashPattern = cur_context->pen.dashPattern ();
      QVector<qreal> tmp (dashPattern.size ());
      for (int i = 0; i < dashPattern.size (); i++)
        tmp[i] = dashPattern.value (i) / w;
      tmpPen.setDashPattern (tmp);
      tmpPen.setDashOffset (cur_context->pen.dashOffset () / w);
    }
    _painter->setRenderHint (QPainter::Antialiasing, true);
    _painter->setPen (tmpPen);
    _painter->setTransform (transform);

    /* If the brush style is gradient and the coordinate mode is ObjectBoundingMode
     * then translate the rotation axis of the gradient transform and transpose
     * the translation values in the global coordinate system*/
    if (cur_context->brush.style () == Qt::LinearGradientPattern
        || cur_context->brush.style () == Qt::RadialGradientPattern) {
      if (cur_context->brush.gradient ()->coordinateMode () == QGradient::ObjectBoundingMode) {
        QTransform origin = cur_context->gradientTransform;
        QTransform newT = QTransform ();
        newT.translate (tx, ty);
        QTransform result = origin * newT;
        result.translate (-tx, -ty);
        double m11 = result.m11 ();
        double m12 = result.m12 ();
        double m13 = result.m13 ();
        double m21 = result.m21 ();
        double m22 = result.m22 ();
        double m23 = result.m23 ();
        double m31 = result.m31 () * width;
        double m32 = result.m32 () * height;
        double m33 = result.m33 ();
        result.setMatrix (m11, m12, m13, m21, m22, m23, m31, m32, m33);
        cur_context->gradientTransform = result;
      }
    }
    cur_context->brush.setTransform (cur_context->gradientTransform);
    _painter->setBrush (cur_context->brush);
  }

  void
  QtBackend::load_pick_context (AbstractGShape *s)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QPen pickPen;
    QBrush pickBrush (_picking_view->pick_color ());
    pickPen.setStyle (Qt::SolidLine);
    pickPen.setColor (_picking_view->pick_color ());
    _picking_view->painter ()->setPen (pickPen);
    _picking_view->painter ()->setBrush (pickBrush);
    _picking_view->painter ()->setTransform (cur_context->matrix.toTransform ());
    _picking_view->add_gobj (s);
  }

  WinImpl*
  QtBackend::create_window (Window *win, const std::string& title, double x, double y, double w, double h)
  {
    return new QtWindow (win, title, x, y, w, h);
  }

  bool
  QtBackend::is_in_picking_view (AbstractGShape *s)
  {
    return is_pickable (s);
    /*return s->press ()->has_coupling () || s->x ()->has_coupling () || s->y ()->has_coupling ()
        || s->move ()->has_coupling () || s->release ()->has_coupling () || s->enter ()->has_coupling ()
        || s->leave ()->has_coupling ();*/
  }
} /* namespace djnn */
#endif

