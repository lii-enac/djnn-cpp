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

#include "qt_backend.h"
#include "qt_window.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>

#include <iostream>
#include <vector>
#include <cmath>

namespace djnn
{
  static Qt::PenCapStyle capStyleArray[3] =
    { Qt::FlatCap, Qt::RoundCap, Qt::SquareCap };

  static Qt::PenJoinStyle joinStyleArray[3] =
    { Qt::MiterJoin, Qt::RoundJoin, Qt::BevelJoin };

  static QGradient::CoordinateMode coordMode[2] =
    { QGradient::LogicalMode, QGradient::ObjectBoundingMode };

  static QGradient::Spread spreadMethod[3] =
    { QGradient::PadSpread, QGradient::RepeatSpread, QGradient::ReflectSpread };

  static QFont::Style fontStyleArray[3] =
    { QFont::StyleNormal, QFont::StyleItalic, QFont::StyleOblique };

  static Qt::FillRule fillRules[2] =
    { Qt::WindingFill, Qt::OddEvenFill };

  void
  QtBackend::load_fill_color (int r, int g, int b)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->brush.setStyle (Qt::SolidPattern);
    int a = cur_context->brush.color ().alpha ();
    cur_context->brush.setColor (QColor (r, g, b, a));
  }

  void
  QtBackend::load_outline_color (int r, int g, int b)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (cur_context->pen.style () == Qt::NoPen) {
      cur_context->pen.setStyle (Qt::SolidLine);
    }
    int curAlpha = cur_context->pen.color ().alpha ();
    cur_context->pen.setColor (QColor (r, g, b, curAlpha));
  }

  void
  QtBackend::load_fill_rule (djnFillRuleType rule)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->fillRule = fillRules[rule];
  }

  void
  QtBackend::load_no_outline ()
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->pen.setStyle (Qt::NoPen);
  }

  void
  QtBackend::load_no_fill ()
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->brush.setStyle (Qt::NoBrush);
  }

  void
  QtBackend::load_texture (const std::string &path)
  {
    QPixmap pic (path.c_str ());
    _context_manager->get_current ()->brush.setTexture (pic);
  }

  void
  QtBackend::load_outline_opacity (float a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QColor c = cur_context->pen.color ();
    float oldAlpha = c.alphaF ();
    c.setAlphaF (oldAlpha * a);
    cur_context->pen.setColor (c);
  }

  void
  QtBackend::load_fill_opacity (float a)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->alpha *= a;
    if (cur_context->brush.style () == Qt::SolidPattern) {
      QColor c = cur_context->brush.color ();
      float oldAlpha = c.alphaF ();
      c.setAlphaF (oldAlpha * a);

      cur_context->brush.setColor (c);
    } else if (cur_context->brush.style () == Qt::RadialGradientPattern
	|| cur_context->brush.style () == Qt::LinearGradientPattern) {
      QGradient g (*cur_context->brush.gradient ());
      QGradientStops stops = cur_context->brush.gradient ()->stops ();
      for (int i = 0; i < stops.size (); ++i)
	stops[i].second.setAlphaF (stops[i].second.alphaF () * a);
      g.setStops (stops);
      cur_context->brush = QBrush (g);
    }
  }

  void
  QtBackend::load_outline_width (double w)
  {
    _context_manager->get_current ()->pen.setWidthF (w);
  }

  void
  QtBackend::load_outline_cap_style (djnCapStyle cap)
  {
    int c = cap;
    if (cap >= (sizeof(capStyleArray) / sizeof(Qt::PenCapStyle))) {
      std::cerr << "Invalid Qt cap style, default will be used\n";
      c = 0;
    }
    _context_manager->get_current ()->pen.setCapStyle (capStyleArray[c]);
  }

  void
  QtBackend::load_outline_join_style (djnJoinStyle join)
  {
    int j = join;
    if (join >= (sizeof(joinStyleArray) / sizeof(Qt::PenJoinStyle))) {
      std::cerr << "Invalid Qt join style, default will be used\n";
      j = 0;
    }
    _context_manager->get_current ()->pen.setJoinStyle (joinStyleArray[j]);
  }

  void
  QtBackend::load_outline_miter_limit (int limit)
  {
    _context_manager->get_current ()->pen.setMiterLimit (limit);
  }

  void
  QtBackend::load_dash_array (vector<double> dash)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    cur_context->pen.setStyle (Qt::CustomDashLine);
    QVector<qreal> vector (0);
    for (auto d : dash)
      vector.append (d);
    cur_context->pen.setDashPattern (vector);
  }

  void
  QtBackend::load_no_dash_array ()
  {
    _context_manager->get_current ()->pen.setStyle (Qt::SolidLine);
  }

  void
  QtBackend::load_dash_offset (double offset)
  {
    _context_manager->get_current ()->pen.setDashOffset (offset);
  }

  void
  QtBackend::load_gradient_stop (int r, int g, int b, float a, float offset)
  {
    QColor color (r, g, b);
    color.setAlphaF (a);
    cur_gradient->setColorAt (offset, color);
  }

  void
  QtBackend::prepare_gradient (AbstractGradient *g)
  {
    cur_gradient->setCoordinateMode (coordMode[g->coords ()->get_value ()]);
    cur_gradient->setSpread (spreadMethod[g->spread ()->get_value ()]);
    _context_manager->get_current ()->gradientTransform = QTransform ();
    g->stops ()->draw ();
    g->transforms ()->draw ();
    QBrush brush (*cur_gradient);
    _context_manager->get_current ()->brush = brush;
  }

  void
  QtBackend::load_linear_gradient (LinearGradient *g)
  {
    double x1 = g->x1 ()->get_value ();
    double y1 = g->y1 ()->get_value ();
    double x2 = g->x2 ()->get_value ();
    double y2 = g->y2 ()->get_value ();
    cur_linear_gradient = QLinearGradient (x1, y1, x2, y2);
    cur_gradient = &cur_linear_gradient;
    prepare_gradient (g);
  }

  void
  QtBackend::load_radial_gradient (RadialGradient *g)
  {
    double cx = g->cx ()->get_value ();
    double cy = g->cy ()->get_value ();
    double r = g->r ()->get_value ();
    double fx = g->fx ()->get_value ();
    double fy = g->fy ()->get_value ();
    cur_radial_gradient = QRadialGradient (cx, cy, r, fx, fy);
    cur_gradient = &cur_radial_gradient;
    prepare_gradient (g);
  }

  void
  QtBackend::load_font_size (djnLengthUnit unit, double size)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (unit == djnPxLength)
      cur_context->font.setPixelSize (size);
    else
      cur_context->font.setPointSizeF (size * cur_context->get_unit_factor (unit));
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_weight (int weight)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    if (weight == -1)
      weight = cur_context->font.weight () - 10 < 0 ? 0 : cur_context->font.weight () - 10; /* lighter */
    if (weight == 100)
      weight = cur_context->font.weight () + 10 > 99 ? 99 : cur_context->font.weight () + 10; /* bolder */
    cur_context->font.setWeight (weight);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_style (djnFontSlope style)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    int val = style;
    if (style >= (int) (sizeof(fontStyleArray) / sizeof(QFont::Style))) {
      std::cerr << "Invalid Qt font style, default will be used\n";
      val = 0;
    }
    cur_context->font.setStyle (fontStyleArray[val]);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_family (const string &family)
  {
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    QString val (family.c_str ());
    cur_context->font.setFamily (val);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_text_anchor (djnAnchorType anchor)
  {
    _context_manager->get_current ()->textAnchor = anchor;
  }
} /* namespace djnn */

#endif
