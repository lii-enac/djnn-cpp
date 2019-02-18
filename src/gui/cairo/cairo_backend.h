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

#pragma once

#include <cairo/cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

#include "cairo_context.h"
#include "cairo_picking_view.h"
#include "../abstract_backend.h"
#include "../backend.h"
#include "../../core/execution/component_observer.h"

namespace djnn
{
  using namespace std;
  extern cairo_t* cur_cairo_state, *cur_cairo_picking_state;

  class TextImpl : public AbstractGObjImpl {
    friend class CairoBackend;
    public:
      TextImpl (int posX, int posY, int width, int height, int b, PangoLayout *layout):
        _posX (posX), _posY (posY), _width (width), _height (height), _b (b), _layout (layout) {}
      virtual ~TextImpl () override { g_object_unref (_layout); }
      int posX () { return _posX; }
      int posY () { return _posY; }
      int width () { return _width; }
      int height () { return _height; }
      int b () { return _b; }
      PangoLayout* layout () { return _layout; }
    private:
      int _posX, _posY, _width, _height, _b;
      PangoLayout* _layout;
  };
  class ShapeImpl : public AbstractGObjImpl {
  public:
    ShapeImpl (cairo_pattern_t *p, int x, int y, int w, int h) : _pattern (p), _x(x), _y(y), _w(w), _h(h){}
    virtual ~ShapeImpl () { cairo_pattern_destroy (_pattern); }
    cairo_pattern_t* pattern () { return _pattern; }
    int x () { return _x; }
    int y () { return _y; }
    int w () { return _w; }
    int h () { return _h; }
  private:
    cairo_pattern_t *_pattern;
    int _x, _y, _w, _h;
  };

  class CairoContextManager;
  class CairoBackend : public AbstractBackend
  {
  public:
    static CairoBackend* instance ();
    virtual
    ~CairoBackend ();
    void
    save_context ();
    void
    restore_context ();
    void
    clear_fill_pattern ();
    void
    clear_stroke_pattern ();
    void
    set_fill_source ();
    void
    set_stroke_source ();
    void
    fill_and_stroke ();
    void
    pick_fill_and_stroke ();
    WinImpl*
    create_window (Window *win, const std::string& title, double x, double y, double w, double h) override;

    //shapes
    void
    draw_rect (Rectangle *s) override;
    void
    draw_circle (Circle *s) override;
    void
    draw_ellipse (Ellipse *s) override;
    void
    draw_line (Line *s) override;
    void
    draw_text (Text *t) override;
    void
    draw_poly (Poly* p) override;
    void
    draw_poly_point (double x, double y) override;
    void
    draw_path (Path *p) override;
    void
    draw_path_move (double x, double y) override;
    void
    draw_path_line (double x, double y) override;
    void
    draw_path_quadratic (double x1, double y1, double x, double y) override;
    void
    draw_path_cubic (double x1, double y1, double x2, double y2, double x, double y) override;
    void
    draw_path_arc (double rx, double ry, double rotx, double fl, double swfl, double x, double y) override;
    void
    draw_path_closure () override;
    void
    draw_rect_clip (RectangleClip *r) override;
    void
    draw_path_clip (Path *p) override;
    void
    draw_image (Image *i) override;

    //style
    void
    load_fill_color (AbstractColor *c) override;
    void
    load_outline_color (AbstractColor *) override;
    void
    load_fill_rule (FillRule *fr) override;
    void
    load_no_outline () override;
    void
    load_no_fill () override;
    void
    load_texture (Texture *t) override;
    void
    load_outline_opacity (OutlineOpacity *oo) override;
    void
    load_fill_opacity (FillOpacity *fo) override;
    void
    load_outline_width (OutlineWidth *ow) override;
    void
    load_outline_cap_style (OutlineCapStyle *ocs) override;
    void
    load_outline_join_style (OutlineJoinStyle *ojs) override;
    void
    load_outline_miter_limit (OutlineMiterLimit *oml) override;
    void
    load_dash_array (DashArray *da) override;
    void
    load_no_dash_array () override;
    void
    load_dash_offset (DashOffset *od) override;
    void
    load_gradient_stop (GradientStop *gs) override;
    void
    load_linear_gradient (LinearGradient *g) override;
    void
    load_radial_gradient (RadialGradient *g) override;
    void
    load_font_size (FontSize *fs) override;
    void
    load_font_weight (FontWeight *fw) override;
    void
    load_font_style (FontStyle *fs) override;
    void
    load_font_family (FontFamily *ff) override;
    void
    load_text_anchor (TextAnchor *a) override;

    //transformations
    void
    load_translation (Translation* t) override;
    void
    load_gradient_translation (GradientTranslation* t) override;
    void
    load_rotation (Rotation* r) override;
    void
    load_gradient_rotation (GradientRotation* r) override;
    void
    load_scaling (Scaling* s) override;
    void
    load_gradient_scaling (GradientScaling* s) override;
    void
    load_skew_x (SkewX* s) override;
    void
    load_gradient_skew_x (GradientSkewX* s) override;
    void
    load_skew_y (SkewY* s) override;
    void
    load_gradient_skew_y (GradientSkewY* s) override;
    void
    load_homography (AbstractHomography* h)
			 override;
    void
    load_gradient_homography (AbstractHomography* h) override;
    void
    update_text_geometry (Text* text, FontFamily* ff, FontSize* fsz, FontStyle* fs, FontWeight *fw) override;
    void
    set_picking_view (CairoPickingView *pick_view) { _pick_view = pick_view; }
  private:
    static CairoBackend *_instance;
    static std::once_flag onceFlag;
    cairo_pattern_t *_cur_cairo_pattern;
    CairoBackend ();
    void
    load_drawing_context (AbstractGShape *s, double tx, double ty, double width, double height);
    void
    load_pick_context (AbstractGShape *s);
    void
    prepare_gradient (AbstractGradient *g);
    bool
    is_in_picking_view (AbstractGShape *s);
    CairoContextManager *_context_manager;
    CairoPickingView *_pick_view;
    PangoFontMap *_PFM;
    PangoContext *_context;
    PangoLayout *_layout;
  };

} /* namespace djnn */
