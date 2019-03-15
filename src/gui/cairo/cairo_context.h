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

#pragma once

#include "cairo_backend.h"
#include "../style/style_types.h"
#include "../../core/execution/component_observer.h"

#include <cairo/cairo.h>
#include <pango/pango.h>

namespace djnn
{
  enum cairo_source_t {
    NO_SOURCE, COLOR, PATTERN
  };

  struct cairo_color_t {
    double r, g, b, a;
  };

  class CairoContext
  {
  public:
    CairoContext ();
    CairoContext (CairoContext* parent);
    virtual ~CairoContext ();
    cairo_source_t _fillType, _strokeType;
    cairo_matrix_t  *_gradientMatrix;
    cairo_color_t _fill, _stroke;
    cairo_pattern_t* _fillPattern;
    cairo_pattern_t* _strokePattern;
    int _gradientCoordinateMode, _dashCount;
    double _dashOffset;
    double* _dashes;
    cairo_fill_rule_t _fillRule;
    cairo_line_cap_t _lineCap;
    cairo_line_join_t _lineJoin;
    double _miterLimit;
    double _lineWidth;
    PangoFontDescription *_font;
    int _textAnchor;
    unsigned int _damaged;
    void update_relative_units ();
    double get_unit_factor (djnLengthUnit unit);
    void clear_fill_pattern ();
    void clear_stroke_pattern ();
    void is_fill_builder () { _fill_builder = true; }
    void notify_change (unsigned int nm) { _damaged |= nm; }
    void reset_damaged () { _damaged = notify_none; }
    unsigned int get_damaged () { return _damaged; }
  private:
    bool _font_builder, _fill_builder, _stroke_builder;
  };

  class CairoContextManager : public ContextManager
  {
  public:
    CairoContextManager () :
      ContextManager ()
    {
      ComponentObserver::instance ().add_draw_context_manager (this);
    }
    ;
    virtual
    ~CairoContextManager () {}
    void pop () override;
    void push () override;
    CairoContext* get_current ();

  private:
    vector<CairoContext*> _context_list;
  };

} /* namespace djnn */
