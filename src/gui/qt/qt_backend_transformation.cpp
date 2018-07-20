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

#include "qt_context.h"
#include "qt_backend.h"
#include "qt_window.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>

#include <iostream>
#include <vector>
#include <cmath>

namespace djnn
{
  void
  QtBackend::load_translation (Translation*, double tx, double ty)
  {
    _context_manager->get_current ()->matrix.translate (tx, ty);
  }

  void
  QtBackend::load_gradient_translation (GradientTranslation*, double tx, double ty)
  {
    _context_manager->get_current ()->gradientTransform.translate (tx, ty);
  }

  void
  QtBackend::load_rotation (Rotation*, double a, double cx, double cy)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->matrix.translate (cx, cy, 0);
    cur_context->matrix.rotate (a, 0, 0, 1);
    if (cx || cy)
      cur_context->matrix.translate (-cx, -cy, 0);
  }

  void
  QtBackend::load_gradient_rotation (GradientRotation*, double a, double cx, double cy)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->gradientTransform.translate (cx, cy);
    cur_context->gradientTransform.rotate (a);
    if (cx || cy)
      cur_context->gradientTransform.translate (-cx, -cy);
  }

  void
  QtBackend::load_scaling (Scaling*, double sx, double sy, double cx, double cy)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->matrix.translate (cx, cy, 0);
    cur_context->matrix.scale (sx, sy, 1);
    if (cx || cy)
      cur_context->matrix.translate (-cx, -cy, 0);
  }

  void
  QtBackend::load_gradient_scaling (GradientScaling*, double sx, double sy, double cx, double cy)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (cx || cy)
      cur_context->gradientTransform.translate (cx, cy);
    cur_context->gradientTransform.scale (sx, sy);
    if (cx || cy)
      cur_context->gradientTransform.translate (-cx, -cy);
  }

  void
  QtBackend::load_skew_x (SkewX*, double a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (1, tan (a_rad), 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }

  void
  QtBackend::load_skew_y (SkewY*, double a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (1, 0, 0, 0, tan (a_rad), 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }

  void
  QtBackend::load_gradient_skew_x (GradientSkewX*, double a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    cur_context->gradientTransform.shear (tan (a_rad), 0);
  }

  void
  QtBackend::load_gradient_skew_y (GradientSkewY*, double a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    double a_rad = a * 0.017453292519943; /* convert degrees to radians */
    cur_context->gradientTransform.shear (0, tan (a_rad));
  }

  void
  QtBackend::load_homography (AbstractHomography*, double m11, double m12, double m13, double m14, double m21, double m22, double m23,
                              double m24, double m31, double m32, double m33, double m34, double m41, double m42,
                              double m43, double m44)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QMatrix4x4 origin = cur_context->matrix;
    QMatrix4x4 newT (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    QMatrix4x4 result = origin * newT;
    cur_context->matrix = result;
  }
  
  void
  QtBackend::load_gradient_homography (AbstractHomography*, double m11, double m12, double m13, double m21, double m22, double m23,
                                       double m31, double m32, double m33)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QTransform origin = cur_context->gradientTransform;
    QTransform newT (m11, m12, m13, m21, m22, m23, m31, m32, m33);
    QTransform result = origin * newT;
    cur_context->gradientTransform = result;
  }
} /* namespace djnn */
