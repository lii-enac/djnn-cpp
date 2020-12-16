/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "gui/backend.h"

#include "qt_context.h"
#include "qt_backend.h"
#include "display/qt/qt_window.h"
#include "gui/transformation/transformations.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>

#include <iostream>
// #include <vector> // pch
#include <cmath>

namespace djnn
{
  void
  QtBackend::load_translation (Translation* t)
  {
    double tx, ty;
    t->get_properties_values (tx, ty);
    _context_manager->get_current ()->matrix.translate (tx, ty);
  }

  void
  QtBackend::load_gradient_translation (GradientTranslation* t)
  {
    double tx, ty;
    t->get_properties_values (tx, ty);
    _context_manager->get_current ()->gradientTransform.translate (tx, ty);
  }

  void
  QtBackend::load_rotation (Rotation* r)
  {
    double a, cx, cy;
    r->get_properties_values (a, cx, cy);
    QtContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->matrix.translate (cx, cy, 0);
    cur_context->matrix.rotate (a, 0, 0, 1);
    if (cx || cy)
      cur_context->matrix.translate (-cx, -cy, 0);
  }

  void
  QtBackend::load_gradient_rotation (GradientRotation* r)
  {
    double a, cx, cy;
    r->get_properties_values (a, cx, cy);
    QtContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->gradientTransform.translate (cx, cy);
    cur_context->gradientTransform.rotate (a);
    if (cx || cy)
      cur_context->gradientTransform.translate (-cx, -cy);
  }

  void
  QtBackend::load_scaling (Scaling* s)
  {
    double sx, sy, cx, cy;
    s->get_properties_values (sx, sy, cx, cy);
    QtContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->matrix.translate (cx, cy, 0);
    cur_context->matrix.scale (sx, sy);
    if (cx || cy)
      cur_context->matrix.translate (-cx, -cy, 0);
  }

  void
  QtBackend::load_gradient_scaling (GradientScaling* s)
  {
    double sx, sy, cx, cy;
    s->get_properties_values (sx, sy, cx, cy);
    QtContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->gradientTransform.translate (cx, cy);
    cur_context->gradientTransform.scale (sx, sy);
    if (cx || cy)
      cur_context->gradientTransform.translate (-cx, -cy);
  }

  void
  QtBackend::load_skew_x (SkewX* s)
  {
    double a;
    s->get_properties_values (a);
    QtContext *cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (1, tan (a_rad), 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }

  void
  QtBackend::load_skew_y (SkewY* s)
  {
    double a;
    s->get_properties_values (a);
    QtContext *cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (1, 0, 0, 0, tan (a_rad), 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }

  void
  QtBackend::load_gradient_skew_x (GradientSkewX* s)
  {
    double a;
    s->get_properties_values (a);
    QtContext *cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    cur_context->gradientTransform.shear (tan (a_rad), 0);
  }

  void
  QtBackend::load_gradient_skew_y (GradientSkewY* s)
  {
    double a;
    s->get_properties_values (a);
    QtContext *cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    cur_context->gradientTransform.shear (0, tan (a_rad));
  }

  void
  QtBackend::load_homography (AbstractHomography* h)
  {
    double m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
    h->get_properties_values (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    QtContext *cur_context = _context_manager->get_current ();
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }
  
  void
  QtBackend::load_gradient_homography (AbstractHomography* h)
  {
    double m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
    h->get_properties_values (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    QtContext *cur_context = _context_manager->get_current ();
    QTransform origin = cur_context->gradientTransform;
    QTransform newT (m11, m12, m13, m21, m22, m23, m31, m32, m33);
    QTransform result = origin * newT;
    cur_context->gradientTransform = result;
  }
} /* namespace djnn */
