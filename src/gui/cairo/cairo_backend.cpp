/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "cairo_context.h"
#include "cairo_backend.h"
#include "gui/backend.h"
#include "gui/transformation/transformations.h"
#include "gui/shape/text.h"
#include "gui/style/text.h"
#include "gui/shape/ui.h"

#include <iostream>
#include <cmath>
#include <locale.h>
#include <atomic>

#if DJNN_SDL
#include "sdl/cairo_sdl_window.h"
//#include "cairo_gl_sdl_window.h"
#endif

namespace djnn
{
  CairoBackend *CairoBackend::_instance;
  //std::once_flag CairoBackend::onceFlag;

  cairo_t* cur_cairo_state = nullptr;
  cairo_t* cur_cairo_picking_state = nullptr;

  void
  p_init_gui () {}

  void
  p_clear_gui () {}

  CairoBackend*
  CairoBackend::instance ()
  {
    static std::atomic_flag onceFlag = ATOMIC_FLAG_INIT;
    if(!onceFlag.test_and_set()) {
      _instance = new CairoBackend();
    }

    // workaround : because Cairo rewrite LC_NUMERIC and after that our parser can't parse any double
    setlocale(LC_NUMERIC, "C");
    
    return _instance;
  }

  CairoBackend::CairoBackend () :
      _cur_cairo_pattern (nullptr), _pick_view (nullptr)
  {
    _context_manager = new CairoContextManager ();
    _PFM = pango_cairo_font_map_new ();
    _context = pango_font_map_create_context (PANGO_FONT_MAP(_PFM));
    _layout = pango_layout_new (_context);
  }

  CairoBackend::~CairoBackend ()
  {
    if (_context_manager) {
      delete _context_manager;
      _context_manager = nullptr;
    }
  }

  void
  CairoBackend::save_context ()
  {
    cairo_save (cur_cairo_state);
    cairo_save (cur_cairo_picking_state);
  }

  void
  CairoBackend::restore_context ()
  {
    cairo_restore (cur_cairo_state);
    cairo_restore (cur_cairo_picking_state);
  }

  /* Cairo context management is imported from djnn v1 */
  void
  CairoBackend::load_drawing_context (AbstractGShape *s, double tx, double ty, double width, double height)
  {
  }

  void
  CairoBackend::load_pick_context (AbstractGShape *s)
  {
  }

  bool
  CairoBackend::is_in_picking_view (AbstractGShape *s)
  {
    return is_pickable (s);
  }

  static double factor[] =
    { .75, 1., 1., 12, 1., .75, 182.88, 1828.8, 72., 1. };
  static PangoStyle fontStyleArray[3] =
    { PANGO_STYLE_NORMAL, PANGO_STYLE_ITALIC, PANGO_STYLE_OBLIQUE };

  void
  CairoBackend::update_text_geometry (Text* text, FontFamily* ff, FontSize* fsz, FontStyle* fs, FontWeight *fw)
  {
    int width, height;

    PangoFontDescription*font = pango_font_description_new ();
    if (fsz) {
      double _sz = factor[(djnLengthUnit) fsz->unit ()->get_value ()] * fsz->size ()->get_value ();
      double isz = _sz - ceil (_sz) < 0.5 ? (int) ceil (_sz) : (int) floor (_sz);
      pango_font_description_set_size (font, (int) (isz * PANGO_SCALE));
    } else {
      pango_font_description_set_size (font, (int) (9 * PANGO_SCALE));
    }
    if (ff)
      pango_font_description_set_family (font, ff->family ()->get_value ().c_str ());
    else
      pango_font_description_set_family (font, "sans");
    if (fs)
      pango_font_description_set_style (font, fontStyleArray[fs->style ()->get_value ()]);
    if (fw) {
      int val = fw->weight ()->get_value ();
      int curVal = pango_font_description_get_weight (font);
      if (val == -1)
        val = curVal - 100 < 100 ? 100 : curVal - 100; /* lighter */
      else if (val == 100)
        val = curVal + 100 > 1000 ? 1000 : curVal + 100; /* bolder */
      else
        val *= 10;
      pango_font_description_set_weight (font, (PangoWeight) val);
    }
    pango_layout_set_text (_layout, text->text ()->get_value ().c_str (), -1);
    pango_layout_set_font_description (_layout, font);
    pango_layout_context_changed(_layout);
    pango_layout_get_pixel_size (_layout, &width, &height);
    pango_font_description_free (font);
    text->set_width (width);
    text->set_height (height);
  }
} /* namespace djnn */
