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
 *
 */

#include "../backend.h"

#include "cairo_backend.h"

#include <iostream>
#include <vector>
#include <cmath>

namespace djnn
{
  static cairo_line_cap_t capStyleArray[3] =
    { CAIRO_LINE_CAP_BUTT, CAIRO_LINE_CAP_ROUND, CAIRO_LINE_CAP_SQUARE };

  static cairo_line_join_t joinStyleArray[3] =
    { CAIRO_LINE_JOIN_MITER, CAIRO_LINE_JOIN_ROUND, CAIRO_LINE_JOIN_BEVEL };

  static double factor[] =
    { .75, 1., 1., 12, 1., .75, 182.88, 1828.8, 72., 1. };

  static djnFillSpread spread_types[] = {djnPadFill, djnRepeatFill, djnReflectFill};

  static PangoStyle fontStyleArray[3] =
    { PANGO_STYLE_NORMAL, PANGO_STYLE_ITALIC, PANGO_STYLE_OBLIQUE };

  void
  CairoBackend::clear_fill_pattern ()
  {
    _context_manager->get_current ()->clear_fill_pattern ();
  }

  void
  CairoBackend::clear_stroke_pattern ()
  {
    _context_manager->get_current ()->clear_stroke_pattern ();
  }

  void
  CairoBackend::set_fill_source ()
  {
    CairoContext *cur = _context_manager->get_current ();
    switch (cur->_fillType)
      {
      case NO_SOURCE:
        break;
      case COLOR:
        if (cur->_fill.a == 1.)
          cairo_set_source_rgb (cur_cairo_state, cur->_fill.r, cur->_fill.g, cur->_fill.b);
        else
          cairo_set_source_rgba (cur_cairo_state, cur->_fill.r, cur->_fill.g, cur->_fill.b, cur->_fill.a);
        break;
      case PATTERN:
        cairo_set_source (cur_cairo_state, cur->_fillPattern);
        break;
      default:
        cerr << "Unknown source type for cairo filling process\n";
      }
  }

  void
  CairoBackend::set_stroke_source ()
  {
    CairoContext *cur = _context_manager->get_current ();
    switch (cur->_strokeType)
      {
      case NO_SOURCE:
        break;
      case COLOR:
        if (cur->_stroke.a == 1.)
          cairo_set_source_rgb (cur_cairo_state, cur->_stroke.r, cur->_stroke.g, cur->_stroke.b);
        else
          cairo_set_source_rgba (cur_cairo_state, cur->_stroke.r, cur->_stroke.g, cur->_stroke.b, cur->_stroke.a);
        break;
      case PATTERN:
        cairo_set_source (cur_cairo_state, cur->_strokePattern);
        break;
      default:
        cerr << "Unknown source type for cairo stroking process\n";
      }
  }

  void
  CairoBackend::fill_and_stroke ()
  {
    CairoContext *cur = _context_manager->get_current ();
    if (cur->_fillType != NO_SOURCE) {
      set_fill_source ();
      if (cur->_fillType == PATTERN) {
        cairo_matrix_t m;
        cairo_matrix_init (&m, cur->_gradientMatrix->xx, cur->_gradientMatrix->yx, cur->_gradientMatrix->xy,
                           cur->_gradientMatrix->yy, cur->_gradientMatrix->x0, cur->_gradientMatrix->y0);
        if (cur->_gradientCoordinateMode == 1) {
          double x1, y1, x2, y2;
          cairo_fill_extents (cur_cairo_state, &x1, &y1, &x2, &y2);
          cairo_matrix_translate (&m, x1, y1);
          cairo_matrix_scale (&m, x2 - x1, y2 - y1);
        }
        if (cairo_matrix_invert (&m) == CAIRO_STATUS_SUCCESS)
          cairo_pattern_set_matrix (cur->_fillPattern, &m);
      }
      cairo_fill_preserve (cur_cairo_state);
    }
    if (cur->_strokeType != NONE) {
      set_stroke_source ();
      cairo_stroke (cur_cairo_state);
    }
   cairo_new_path (cur_cairo_state);
  }

  void
  CairoBackend::pick_fill_and_stroke ()
  {
    int color = _pick_view->pick_color ();
    double r = (color >> 16 & 0xFF) / 255.0;
    double g = ((color >> 8) & 0xFF) / 255.0;
    double b = (color & 0xFF) / 255.0;
    cairo_set_source_rgb (cur_cairo_picking_state, r, g, b);
    cairo_fill_preserve (cur_cairo_picking_state);
    if (_context_manager->get_current ()->_strokeType != NONE) {
      cairo_stroke (cur_cairo_picking_state);
    }
    cairo_new_path (cur_cairo_picking_state);
  }

  void
  CairoBackend::load_fill_color (AbstractColor *c)
  {
    double r, g, b;
    c->get_properties_values(r, g, b);
    CairoContext *cur = _context_manager->get_current ();
    cairo_color_t *fill = &cur->_fill;
    fill->r = r / 255.0;
    fill->g = g / 255.0;
    fill->b = b / 255.0;
    cur->_fillType = COLOR;
  }

  void
  CairoBackend::load_outline_color (AbstractColor *c)
  {
    double r, g, b;
    c->get_properties_values(r, g, b);
    CairoContext *cur = _context_manager->get_current ();
    cairo_color_t *stroke = &cur->_stroke;
    stroke->r = r / 255.0;
    stroke->g = g / 255.0;
    stroke->b = b / 255.0;
    cur->_strokeType = COLOR;
  }

  void
  CairoBackend::load_fill_rule (FillRule *fr)
  {
    int rule;
    fr->get_properties_values(rule);
    cairo_set_fill_rule (cur_cairo_state, rule == 0 ? CAIRO_FILL_RULE_WINDING : CAIRO_FILL_RULE_EVEN_ODD);
  }

  void
  CairoBackend::load_no_outline ()
  {
    clear_stroke_pattern ();
    _context_manager->get_current ()->_strokeType = NO_SOURCE;
  }

  void
  CairoBackend::load_no_fill ()
  {
    clear_fill_pattern ();
    _context_manager->get_current ()->_fillType = NO_SOURCE;
    ;
  }

  void
  CairoBackend::load_texture (Texture *t)
  {
    //TODO
    cerr << "Warning: texture not yet implemented in Cairo backend\n";
  }

  void
  CairoBackend::load_outline_opacity (OutlineOpacity *oo)
  {
    double a;
    oo->get_properties_values (a);
    CairoContext *cur = _context_manager->get_current ();
    float oldAlpha = cur->_stroke.a;
    cur->_stroke.a = oldAlpha * a;
  }

  void
  CairoBackend::load_fill_opacity (FillOpacity *fo)
  {
    double a;
    fo->get_properties_values(a);
    CairoContext *cur = _context_manager->get_current ();
    float oldAlpha = cur->_fill.a;
    cur->_fill.a = oldAlpha * a;
  }

  void
  CairoBackend::load_outline_width (OutlineWidth *ow)
  {
    double w;
    ow->get_properties_values(w);
    cairo_set_line_width (cur_cairo_state, w);
  }

  void
  CairoBackend::load_outline_cap_style (OutlineCapStyle *ocs)
  {
    int c;
    ocs->get_properties_values(c);
    cairo_set_line_cap (cur_cairo_state, capStyleArray[c]);
  }

  void
  CairoBackend::load_outline_join_style (OutlineJoinStyle *ojs)
  {
    int j;
    ojs->get_properties_values(j);
    cairo_set_line_join (cur_cairo_state, joinStyleArray[j]);
  }

  void
  CairoBackend::load_outline_miter_limit (OutlineMiterLimit *oml)
  {
    int limit;
    oml->get_properties_values(limit);
    cairo_set_miter_limit (cur_cairo_state, limit);
  }

  void
  CairoBackend::load_dash_array (DashArray *da)
  {
    const vector<double>& v_dashes = da->dash_array ();
    int init_sz = v_dashes.size ();
    int num_dashes = init_sz % 2 == 0 ? init_sz : init_sz * 2;
    double dashes[num_dashes];
    for (int i = 0; i < init_sz; ++i) {
      dashes[i] = v_dashes[i];
    }
    if (num_dashes != init_sz) {
      for (int i = init_sz; i < num_dashes; i++)
        dashes[i] = v_dashes[i];
    }
    cairo_set_dash (cur_cairo_state, dashes, num_dashes, _context_manager->get_current ()->_dashOffset);
  }

  void
  CairoBackend::load_no_dash_array ()
  {
    cairo_set_dash (cur_cairo_state, 0, 0, 0);
  }

  void
  CairoBackend::load_dash_offset (DashOffset *od)
  {
    double offset;
    od->get_properties_values(offset);
    int num_dashes = cairo_get_dash_count (cur_cairo_state);
    _context_manager->get_current ()->_dashOffset = offset;
    if (num_dashes) {
      double dashes[num_dashes];
      double old_offset;
      cairo_get_dash (cur_cairo_state, dashes, &old_offset);
      cairo_set_dash (cur_cairo_state, dashes, num_dashes, offset);
    }
  }

  void
  CairoBackend::load_gradient_stop (GradientStop *gs)
  {
    double r, g, b;
    double a, offset;
    gs->get_properties_values(r, g, b, a, offset);
    cairo_pattern_add_color_stop_rgba (_cur_cairo_pattern, offset, r/255.0, g/255.0, b/255.0, a);
  }

  void
  CairoBackend::prepare_gradient (AbstractGradient *g)
  {
    CairoContext* cur = _context_manager->get_current ();
    djnFillSpread spread = spread_types[g->spread ()->get_value ()];
    switch (spread)
      {
      case djnPadFill:
        cairo_pattern_set_extend (_cur_cairo_pattern, CAIRO_EXTEND_PAD);
        break;
      case djnRepeatFill:
        cairo_pattern_set_extend (_cur_cairo_pattern, CAIRO_EXTEND_REPEAT);
        break;
      case djnReflectFill:
        cairo_pattern_set_extend (_cur_cairo_pattern, CAIRO_EXTEND_REFLECT);
        break;
      }
    clear_fill_pattern ();
    cur->_fillPattern = _cur_cairo_pattern;
    cur->_fillType = PATTERN;
    cur->_gradientCoordinateMode = g->coords ()->get_value ();
    g->stops ()->draw ();
    g->transforms ()->draw ();
    _context_manager->get_current ()->is_fill_builder ();
  }

  void
  CairoBackend::load_linear_gradient (LinearGradient *g)
  {
    double x1, y1, x2, y2;
    g->get_properties_values (x1, y1, x2, y2);
    _cur_cairo_pattern = cairo_pattern_create_linear (x1, y1, x2, y2);
    prepare_gradient (g);
  }

  void
  CairoBackend::load_radial_gradient (RadialGradient *g)
  {
    double cx, cy, r, fx, fy;
    g->get_properties_values (cx, cy, r, fx, fy);
    _cur_cairo_pattern = cairo_pattern_create_radial (cx, cy, r, fx, fy, 0);
    prepare_gradient (g);
  }

  void
  CairoBackend::load_font_size (FontSize *fs)
  {
    int unit;
    double size;
    fs->get_properties_values(unit, size);
    double _sz = factor[unit] * size;
    double isz = _sz - ceil (_sz) < 0.5 ? (int) ceil (_sz) : (int) floor (_sz);
    pango_font_description_set_size (_context_manager->get_current ()->_font, (int) (isz * PANGO_SCALE));
  }

  void
  CairoBackend::load_font_weight (FontWeight *fw)
  {
    int weight;
    fw->get_properties_values(weight);
    CairoContext *cur_context = _context_manager->get_current ();
    int val = weight;
    int curVal = pango_font_description_get_weight (cur_context->_font);
    if (val == -1)
      val = curVal - 100 < 100 ? 100 : curVal - 100; /* lighter */
    else if (val == 100)
      val = curVal + 100 > 1000 ? 1000 : curVal + 100; /* bolder */
    else
      val *= 10;
    pango_font_description_set_weight (cur_context->_font, (PangoWeight) val);
  }

  void
  CairoBackend::load_font_style (FontStyle *fs)
  {
    int style;
    fs->get_properties_values(style);
    pango_font_description_set_style (_context_manager->get_current ()->_font,
                fontStyleArray[style]);
  }

  void
  CairoBackend::load_font_family (FontFamily *ff)
  {
    string family;
    ff->get_properties_values(family);
    pango_font_description_set_family (_context_manager->get_current ()->_font, family.c_str ());
  }

  void
  CairoBackend::load_text_anchor (TextAnchor *a)
  {
    int anchor;
    a->get_properties_values(anchor);
    _context_manager->get_current ()->_textAnchor = anchor;
  }
} /* namespace djnn */
