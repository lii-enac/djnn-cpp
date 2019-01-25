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

#include "my_cairo_surface.h"
#include "../backend.h"

#include "cairo_backend.h"
#include <iostream>
#include <cmath>

namespace djnn
{
  CairoContext*
  CairoContextManager::get_current ()
  {
    if (_context_list.empty ())
      return nullptr;
    return _context_list.back ();
  }

  void
  CairoContextManager::push ()
  {
    CairoBackend::instance ()->save_context ();
    if (_context_list.empty ())
      _context_list.push_back (new CairoContext ());
    else {
      _context_list.push_back (new CairoContext (_context_list.back ()));
    }
  }

  void
  CairoContextManager::pop ()
  {
    if (!_context_list.empty()) {
      delete _context_list.back ();
      _context_list.pop_back ();
    }
    CairoContext * ctx = get_current ();
    if (ctx) {
      cairo_set_fill_rule (cur_cairo_state, ctx->_fillRule);
      cairo_set_line_cap (cur_cairo_state, ctx->_lineCap);
      cairo_set_line_join (cur_cairo_state, ctx->_lineJoin);
      cairo_set_line_width (cur_cairo_state, ctx->_lineWidth);
      cairo_set_miter_limit (cur_cairo_state, ctx->_miterLimit);
      cairo_set_dash (cur_cairo_state, ctx->_dashes, ctx->_dashCount, ctx->_dashOffset);
    }
    CairoBackend::instance ()->restore_context ();
  }

  CairoContext::CairoContext () :
    _fillType (COLOR), _strokeType (NO_SOURCE), _fillPattern (nullptr), _strokePattern (nullptr), _gradientCoordinateMode (
        0), _dashOffset (0), _textAnchor (0), _font_builder (true), _fill_builder (false), _stroke_builder (false)
  {
    _gradientMatrix = (cairo_matrix_t*) malloc (sizeof(cairo_matrix_t));
    _fill.a = 1;
    _fill.r = 0;
    _fill.g = 0;
    _fill.b = 0;
    _stroke.a = 1;
    _fillRule = cairo_get_fill_rule (cur_cairo_state);
    _lineCap = cairo_get_line_cap (cur_cairo_state);
    _lineJoin = cairo_get_line_join (cur_cairo_state);
    _miterLimit = cairo_get_miter_limit (cur_cairo_state);
    _lineWidth = cairo_get_line_width (cur_cairo_state);
    _dashCount = cairo_get_dash_count (cur_cairo_state);
    if (_dashCount) {
      _dashes = (double*) malloc (_dashCount * sizeof(double));
      cairo_get_dash (cur_cairo_state, _dashes, &_dashOffset);
    } else
      _dashes = 0;
    cairo_matrix_init_identity (_gradientMatrix);
    _font = pango_font_description_new ();
    pango_font_description_set_size (_font, 9 * PANGO_SCALE);
    pango_font_description_set_family (_font, "sans");
  }

  CairoContext::CairoContext (CairoContext *cc) :
      CairoContext ()
  {
    _fill = cc->_fill;
    _stroke = cc->_stroke;
    _fillPattern = cc->_fillPattern;
    _strokePattern = cc->_strokePattern;
    _fillType = cc->_fillType;
    _strokeType = cc->_strokeType;
    _dashOffset = cc->_dashOffset;
    _gradientMatrix->xx = cc->_gradientMatrix->xx;
    _gradientMatrix->yx = cc->_gradientMatrix->yx;
    _gradientMatrix->xy = cc->_gradientMatrix->xy;
    _gradientMatrix->yy = cc->_gradientMatrix->yy;
    _gradientMatrix->x0 = cc->_gradientMatrix->x0;
    _gradientMatrix->y0 = cc->_gradientMatrix->y0;
    _gradientCoordinateMode = cc->_gradientCoordinateMode;
    pango_font_description_merge (_font, cc->_font, TRUE);
    _textAnchor = cc->_textAnchor;
  }

  CairoContext::~CairoContext ()
  {
    if (_dashes) {
      delete _dashes;
    }
    if (_font && _font_builder) {
      pango_font_description_free (_font);
      _font = nullptr;
    }
    delete _gradientMatrix;
    if (_fillPattern && _fill_builder) {
      cairo_pattern_destroy (_fillPattern);
      _fillPattern = nullptr;
    }
    if (_strokePattern && _stroke_builder) {
      cairo_pattern_destroy (_strokePattern);
      _strokePattern = nullptr;
    }
  }

  void
  CairoContext::clear_fill_pattern () {
    if (_fillPattern && _fill_builder) {
      cairo_pattern_destroy (_fillPattern);
      _fill_builder = false;
    }
    _fillPattern = nullptr;
  }

  void
  CairoContext::clear_stroke_pattern () {
    if (_strokePattern && _stroke_builder) {
      cairo_pattern_destroy (_strokePattern);
      _stroke_builder = false;
    }
    _strokePattern = nullptr;
  }
} /* namespace djnn */
