/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "../backend.h"

#include "cairo_context.h"
#include "cairo_backend.h"

#include <iostream>
#include <vector>
#include <cmath>

namespace djnn
{
  void
  CairoBackend::load_translation (Translation* t)
  {
    double tx, ty;
    t->get_properties_values (tx, ty);
    cairo_translate (cur_cairo_state, tx, ty);
    cairo_translate (cur_cairo_picking_state, tx, ty);
  }

  void
  CairoBackend::load_gradient_translation (GradientTranslation* t)
  {
    double tx, ty;
    t->get_properties_values (tx, ty);
    cairo_matrix_translate (_context_manager->get_current ()->_gradientMatrix, tx, ty);
  }

  static float R = 0.017453292519943;
  void
  CairoBackend::load_rotation (Rotation* r)
  {
    double a, cx, cy;
    r->get_properties_values (a, cx, cy);
    if (cx || cy) {
      cairo_translate (cur_cairo_state, cx, cy);
      cairo_translate (cur_cairo_picking_state, cx, cy);
    }
    cairo_rotate (cur_cairo_state, a * R);
    cairo_rotate (cur_cairo_picking_state, a * R);
    if (cx || cy) {
      cairo_translate (cur_cairo_state, -cx, -cy);
      cairo_translate (cur_cairo_picking_state, -cx, -cy);
    }
  }

  void
  CairoBackend::load_gradient_rotation (GradientRotation* r)
  {
    double a, cx, cy;
    r->get_properties_values (a, cx, cy);
    CairoContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cairo_matrix_translate (cur_context->_gradientMatrix, cx, cy);
    cairo_matrix_rotate (cur_context->_gradientMatrix, a * R);
    if (cx || cy)
      cairo_matrix_translate (cur_context->_gradientMatrix, -cx, -cy);
  }

  void
  CairoBackend::load_scaling (Scaling* s)
  {
    double sx, sy, cx, cy;
    s->get_properties_values (sx, sy, cx, cy);
    if (cx || cy) {
      cairo_translate (cur_cairo_state, cx, cy);
      cairo_translate (cur_cairo_picking_state, cx, cy);
    }
    cairo_scale (cur_cairo_state, sx, sy);
    cairo_scale (cur_cairo_picking_state, sx, sy);
    if (cx || cy) {
      cairo_translate (cur_cairo_state, -cx, -cy);
      cairo_translate (cur_cairo_picking_state, -cx, -cy);
    }
    if (s->get_damaged () & notify_damaged_transform) {
      cairo_matrix_t mm;
      cairo_get_matrix (cur_cairo_state, &mm);
      _context_manager->get_current ()->notify_change (notify_damaged_transform);
      s->reset_damaged ();
    }
  }

  void
  CairoBackend::load_gradient_scaling (GradientScaling* s)
  {
    double sx, sy, cx, cy;
    s->get_properties_values (sx, sy, cx, cy);
    CairoContext *cur_context = _context_manager->get_current ();
    if (cx || cy)
      cairo_matrix_translate (cur_context->_gradientMatrix, cx, cy);
    cairo_matrix_scale (cur_context->_gradientMatrix, sx, sy);
    if (cx || cy)
      cairo_matrix_translate (cur_context->_gradientMatrix, -cx, -cy);
  }

  void
  CairoBackend::load_skew_x (SkewX* s)
  {
    double a;
    s->get_properties_values (a);
    cairo_matrix_t cm;
    cairo_matrix_init_identity (&cm);
    double a_rad = a * R;
    cm.xy = tan (a_rad);
    cairo_transform (cur_cairo_state, &cm);
    cairo_transform (cur_cairo_picking_state, &cm);
  }

  void
  CairoBackend::load_skew_y (SkewY* s)
  {
    double a;
    s->get_properties_values (a);
    cairo_matrix_t cm;
    cairo_matrix_init_identity (&cm);
    double a_rad = a * R;
    cm.yx = tan (a_rad);
    cairo_transform (cur_cairo_state, &cm);
    cairo_transform (cur_cairo_picking_state, &cm);
  }

  void
  CairoBackend::load_gradient_skew_x (GradientSkewX* s)
  {
    double a;
    s->get_properties_values (a);
    CairoContext *cur_context = _context_manager->get_current ();
    double a_rad = a * R; /* convert degrees to radians */
    cairo_matrix_t cm;
    cairo_matrix_init_identity (&cm);
    cm.xy = tan (a_rad);
    cairo_matrix_multiply (cur_context->_gradientMatrix, cur_context->_gradientMatrix, &cm);
  }

  void
  CairoBackend::load_gradient_skew_y (GradientSkewY* s)
  {
    double a;
    s->get_properties_values (a);
    CairoContext *cur_context = _context_manager->get_current ();
    double a_rad = a * R; /* convert degrees to radians */
    cairo_matrix_t cm;
    cairo_matrix_init_identity (&cm);
    cm.yx = tan (a_rad);
    cairo_matrix_multiply (cur_context->_gradientMatrix, cur_context->_gradientMatrix, &cm);
  }

  void
  CairoBackend::load_homography (AbstractHomography* h)
  {
    double m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
    h->get_properties_values (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    cairo_matrix_t matrix;
    cairo_matrix_init_identity (&matrix);
    matrix.xx = m11;
    matrix.yx = m21;
    matrix.xy = m12;
    matrix.yy = m22;
    matrix.x0 = m14;
    matrix.y0 = m24;
    cairo_transform (cur_cairo_state, &matrix);
    cairo_transform (cur_cairo_picking_state, &matrix);
  }

  void
  CairoBackend::load_gradient_homography (AbstractHomography* h)
  {
    CairoContext *cur_context = _context_manager->get_current ();
    double m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
    h->get_properties_values (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    cairo_matrix_t matrix;
    cairo_matrix_init_identity (&matrix);
    matrix.xx = m11;
    matrix.yx = m21;
    matrix.xy = m12;
    matrix.yy = m22;
    matrix.x0 = m14;
    matrix.y0 = m24;

    cairo_matrix_multiply (cur_context->_gradientMatrix, cur_context->_gradientMatrix, &matrix);
  }
} /* namespace djnn */
