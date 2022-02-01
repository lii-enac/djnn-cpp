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


#include "core/tree/list.h"
#include "display/qt/qt_window.h"
#include "gui/backend.h"
#include "gui/style/style.h"

#include "qt_backend.h"

#include <QtWidgets/QWidget>

#include "core/utils/iostream.h"
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
  QtBackend::load_fill_color (AbstractColor *c)
  {
    double r, g, b;
    c->get_properties_values(r, g, b);
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->brush.setStyle (Qt::SolidPattern);
    int a = cur_context->alpha * 255;
    cur_context->brush.setColor (QColor (r, g, b, a));
  }

  void
  QtBackend::load_outline_color (AbstractColor *c)
  {
    double r, g, b;
    c->get_properties_values(r, g, b);
    QtContext *cur_context = _context_manager->get_current ();
    if (cur_context->pen.style () == Qt::NoPen) {
      cur_context->pen.setStyle (Qt::SolidLine);
    }
    int curAlpha = cur_context->pen.color ().alpha ();
    cur_context->pen.setColor (QColor (r, g, b, curAlpha));
  }

  void
  QtBackend::load_no_pick_outline ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->set_pick_outline (false);
  }

  void
  QtBackend::load_no_pick_fill ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->set_pick_fill (false);
  }

  void
  QtBackend::load_pick_outline ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->set_pick_outline (true);
  }

  void
  QtBackend::load_pick_fill ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->set_pick_fill (true);
  }

  void
  QtBackend::load_fill_rule (FillRule *fr)
  {
    int rule;
    fr->get_properties_values(rule);
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->fillRule = fillRules[rule];
  }

  void
  QtBackend::load_no_outline ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->pen.setStyle (Qt::NoPen);
  }

  void
  QtBackend::load_no_fill ()
  {
    QtContext *cur_context = _context_manager->get_current ();
    cur_context->brush.setStyle (Qt::NoBrush);
  }

  void
  QtBackend::load_texture (Texture *t)
  {
    string path;
    t->get_properties_values(path);
    QPixmap pic (path.c_str ());
    _context_manager->get_current ()->brush.setTexture (pic);
  }

  void
  QtBackend::load_outline_opacity (OutlineOpacity *oo)
  {
    double a;
    oo->get_properties_values (a);
    QtContext *cur_context = _context_manager->get_current ();
    QColor c = cur_context->pen.color ();
    float oldAlpha = c.alphaF ();
    c.setAlphaF (oldAlpha * a);
    cur_context->pen.setColor (c);
  }

  void
  QtBackend::load_fill_opacity (FillOpacity *fo)
  {
    double a;
    fo->get_properties_values(a);
    QtContext *cur_context = _context_manager->get_current ();
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
  QtBackend::load_outline_width (OutlineWidth *ow)
  {
    double w;
    ow->get_properties_values(w);
    _context_manager->get_current ()->pen.setWidthF (w);
  }

  void
  QtBackend::load_outline_cap_style (OutlineCapStyle *ocs)
  {
    int c;
    ocs->get_properties_values(c);
    if (static_cast<unsigned int>(c) >= (sizeof(capStyleArray) / sizeof(Qt::PenCapStyle))) {
      std::cerr << "Invalid Qt cap style, default will be used\n";
      c = 0;
    }
    _context_manager->get_current ()->pen.setCapStyle (capStyleArray[c]);
  }

  void
  QtBackend::load_outline_join_style (OutlineJoinStyle *ojs)
  {
    int j;
    ojs->get_properties_values(j);
    if (static_cast<unsigned int>(j) >= (sizeof(joinStyleArray) / sizeof(Qt::PenJoinStyle))) {
      std::cerr << "Invalid Qt join style, default will be used\n";
      j = 0;
    }
    _context_manager->get_current ()->pen.setJoinStyle (joinStyleArray[j]);
  }

  void
  QtBackend::load_outline_miter_limit (OutlineMiterLimit *oml)
  {
    int limit;
    oml->get_properties_values(limit);
    _context_manager->get_current ()->pen.setMiterLimit (limit);
  }

  void
  QtBackend::load_dash_array (DashArray *da)
  {
    const vector<double>& dash = da->dash_array ();
    QtContext *cur_context = _context_manager->get_current ();
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
  QtBackend::load_dash_offset (DashOffset *od)
  {
    double offset;
    od->get_properties_values(offset);
    _context_manager->get_current ()->pen.setDashOffset (offset);
  }

  void
  QtBackend::load_gradient_stop (GradientStop *gs)
  {
    double r, g, b;
    double a, offset;
    gs->get_properties_values(r, g, b, a, offset);
    QColor color (r, g, b);
    color.setAlphaF (a * _context_manager->get_current ()->brush.color ().alphaF ());
    cur_gradient->setColorAt (offset, color);
  }

  void
  QtBackend::prepare_gradient (AbstractGradient *g)
  {
    cur_gradient->setCoordinateMode (coordMode[g->coords ()->get_value ()]);
    cur_gradient->setSpread (spreadMethod[g->spread ()->get_value ()]);
    _context_manager->get_current ()->gradientTransform = QTransform ();
    for (auto s: g->stops ()->children ()) {
      s->draw ();
    }
    for (auto t: g->transforms ()->children ()) {
      t->draw ();
    }
    QBrush brush (*cur_gradient);
    _context_manager->get_current ()->brush = brush;
  }

  void
  QtBackend::load_linear_gradient (LinearGradient *g)
  {
    double x1, y1, x2, y2;
    int spread, coords;
    g->get_properties_values (x1, y1, x2, y2, spread, coords);
    cur_linear_gradient = QLinearGradient (x1, y1, x2, y2);
    cur_gradient = &cur_linear_gradient;
    prepare_gradient (g);
  }

  void
  QtBackend::load_radial_gradient (RadialGradient *g)
  {
    double cx, cy, r, fx, fy;
    int spread, coords;
    g->get_properties_values (cx, cy, r, fx, fy, spread, coords);
    cur_radial_gradient = QRadialGradient (cx, cy, r, fx, fy);
    cur_gradient = &cur_radial_gradient;
    prepare_gradient (g);
  }

  void
  QtBackend::load_font_size (FontSize *fs)
  {
    int unit;
    double size;
    fs->get_properties_values(unit, size);
    QtContext *cur_context = _context_manager->get_current ();
    if (unit == DJN_PX)
      cur_context->font.setPixelSize (size);
    else
      cur_context->font.setPointSizeF (size * cur_context->get_unit_factor ((djnn::djnLengthUnit)unit));
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_weight (FontWeight *fw)
  {
    int weight;
    fw->get_properties_values(weight);
    QtContext *cur_context = _context_manager->get_current ();
    if (weight == -1)
      weight = cur_context->font.weight () - 10 < 0 ? 0 : cur_context->font.weight () - 10; /* lighter */
    if (weight == 100)
      weight = cur_context->font.weight () + 10 > 99 ? 99 : cur_context->font.weight () + 10; /* bolder */
    cur_context->font.setWeight (weight);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_style (FontStyle *fs)
  {
    int style;
    fs->get_properties_values(style);
    QtContext *cur_context = _context_manager->get_current ();
    int val = style;
    if (style >= (int) (sizeof(fontStyleArray) / sizeof(QFont::Style))) {
      std::cerr << "Invalid Qt font style, default will be used\n";
      val = 0;
    }
    cur_context->font.setStyle (fontStyleArray[val]);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_font_family (FontFamily *ff)
  {
    string family;
    ff->get_properties_values(family);
    QtContext *cur_context = _context_manager->get_current ();
    //FIXME: is there really a leak with QString - we have tested family.c_str () and it doesn't seems to be in cause
    QString val (family.c_str ());
    cur_context->font.setFamily (val);
    cur_context->update_relative_units ();
  }

  void
  QtBackend::load_text_anchor (TextAnchor *a)
  {
    int anchor;
    a->get_properties_values(anchor);
    _context_manager->get_current ()->textAnchor = anchor;
  }
} /* namespace djnn */
