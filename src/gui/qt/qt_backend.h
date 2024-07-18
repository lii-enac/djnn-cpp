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

#pragma once

#include <QtGui/QPainterPath>
#include <mutex> // std::once_flag

#include "gui/abstract_backend.h"
#include "gui/qt/qt_context.h"
#include "qt/qt_picking_view.h"

class QWidget;
class QPainter;

namespace djnn {

class QtContextManager;
class QtBackend : public AbstractBackend {
  public:
    static QtBackend* instance ();
    virtual ~QtBackend ();
    void
    save_context ();
    void
    restore_context ();
    void
         set_painter (QPainter* p);
    void set_picking_view (QtPickingView* p);
    // void set_picking_view (Picking *p);
    QPainter* painter () { return _painter; }
    /*
    WinImpl*
    create_window (Window *win, const string& title, double x, double y, double w, double h) override;
    */
    // shape
    void
    draw_rectangle (Rectangle* s) override;
    void
    draw_circle (Circle* s) override;
    void
    draw_ellipse (Ellipse* s) override;
    void
    draw_line (Line* s) override;
    void
    draw_text (Text* t) override;
    int
    draw_simple_text (SimpleText* t, QFontMetrics* fm, int x, int y);
    void
    draw_text_field (TextField*) override;
    int
    get_average_char_width (FontMetricsImpl fm) override;
    void
    draw_simple_text_edit (MultilineEditor*) override;
    void
    open_dialog (FileDialog*) override;
    void
    save_dialog (FileDialog*) override;
    size_t
    compute_index (FontMetricsImpl fm, SimpleText* t, int x) override;
    int
    compute_x (FontMetricsImpl fm, SimpleText* t, size_t index) override;
    int
    compute_text_width (FontMetricsImpl fm, SimpleText* t) override;
    double
    get_cursor_from_index (Text*, int) override;
    pair<double, int>
    get_cursor_from_local_x (Text*, double) override;
    void
    draw_poly (Poly* p) override;
    void
    draw_poly_point (double x, double y) override;
    void
    draw_path (Path* p) override;
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
    draw_path_segment (double xc, double yc, double th0, double th1, double rx, double ry, double xAxisRotation);
    void
    draw_rectangle_clip (RectangleClip* r) override;
    void
    draw_path_clip (Path* p) override;
    void
    draw_image (Image* i) override;
    int
    get_pixel (Image* i, double x, double y) override;
    void
         draw_data_image (DataImage* i) override;
    void draw_layer (Layer* l, children_t _children) override;
    void draw_z_ordered_group (ZOrderedGroup* g) override;
    // style
    void
    load_fill_color (AbstractColor* c) override;
    void
    load_outline_color (AbstractColor* c) override;
    void
    load_fill_rule (FillRule* fr) override;
    void
    load_no_outline () override;
    void
    load_no_fill () override;
    void
    load_no_pick_outline () override;
    void
    load_no_pick_fill () override;
    void
    load_pick_outline () override;
    void
    load_pick_fill () override;
    void
    load_texture (Texture* t) override;
    void
    load_outline_opacity (OutlineOpacity* oo) override;
    void
    load_fill_opacity (FillOpacity* fo) override;
    void
    load_outline_width (OutlineWidth* ow) override;
    void
    load_outline_cap_style (OutlineCapStyle* ocs) override;
    void
    load_outline_join_style (OutlineJoinStyle* ojs) override;
    void
    load_outline_miter_limit (OutlineMiterLimit* oml) override;
    void
    load_dash_array (DashArray* da) override;
    void
    load_no_dash_array () override;
    void
    load_dash_offset (DashOffset* od) override;
    void
    load_gradient_stop (GradientStop* gs) override;
    void
    load_linear_gradient (LinearGradient* g) override;
    void
    load_radial_gradient (RadialGradient* g) override;
    void
    load_font_size (FontSize* fs) override;
    void
    load_font_weight (FontWeight* fw) override;
    void
    load_font_style (FontStyle* fs) override;
    void
    load_font_family (FontFamily* ff) override;
    void
    load_text_anchor (TextAnchor* a) override;

    // transformations
    void
    load_identity (Identity*) override;
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
    load_zorder (int z) override;
    void
    update_text_geometry (Text* text, FontFamily* ff, FontSize* fsz, FontStyle* fs, FontWeight* fw) override;
    void
    delete_text_impl (Text* text) override;
    void
    delete_image_impl (Image* image) override;

  private:
    static QtBackend*     _instance;
    static std::once_flag onceFlag;
    QtBackend ();
    void
    load_drawing_context (AbstractGShape* s, QtContext* context, double tx, double ty, double width, double height);
    void
    load_pick_context (AbstractGShape* s, QtContext* context);
    void
    prepare_gradient (AbstractGradient* g);
    bool
                   is_in_picking_view (AbstractGShape* s);
    QPainter*      _painter;
    QtPickingView* _picking_view;
    bool           _in_cache;
    // Picking * _picking_view;
    QtContextManager* _context_manager;
    QPolygonF         cur_poly;
    QPainterPath      cur_path;
    QLinearGradient   cur_linear_gradient;
    QRadialGradient   cur_radial_gradient;
    QGradient*        cur_gradient;
};

} /* namespace djnn */
