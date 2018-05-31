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

#pragma once

#include <vector>
#include "shapes/shapes.h"
#include "style/style.h"
#include "style/style_types.h"
#include "window.h"

namespace djnn
{
  using namespace std;

  class AbstractBackend
  {
  public:
    virtual
    ~AbstractBackend ()
    {
    }
    
    virtual WinImpl*
    create_window (Window *win, const std::string& title, double x, double y, double w, double h) = 0;

    void
    set_window (Window *w)
    {
      _window = w;
    }

    Window*
    window ()
    {
      return _window;
    }

    // shapes
    virtual void
    draw_rect (AbstractGShape *s, double x, double y, double w, double h, double rx, double ry)
    {
    }
    virtual void
    draw_circle (AbstractGShape *s, double cx, double cy, double r)
    {
    }
    virtual void
    draw_ellipse (AbstractGShape *s, double cx, double cy, double rx, double ry)
    {
    }
    virtual void
    draw_line (AbstractGShape *s, double x1, double y1, double x2, double y2)
    {
    }
    virtual void
    draw_text (Text*)
    {
    }
    virtual void
    draw_poly (Poly* p)
    {
    }
    virtual void
    draw_poly_point (double x, double y)
    {
    }
    virtual void
    draw_path (Path *p)
    {
    }
    virtual void
    draw_path_move (double x, double y)
    {
    }
    virtual void
    draw_path_line (double x, double y)
    {
    }
    virtual void
    draw_path_quadratic (double x1, double y1, double x, double y)
    {
    }
    virtual void
    draw_path_cubic (double x1, double y1, double x2, double y2, double x, double y)
    {
    }
    virtual void
    draw_path_arc (double rx, double ry, double rotx, double fl, double swfl, double x, double y)
    {
    }
    virtual void
    draw_path_closure ()
    {
    }
    virtual void
    draw_rect_clip (AbstractGShape *s, double x, double y, double w, double h)
    {
    }
    virtual void
    draw_path_clip (Path *p)
    {
    }
    virtual void
    draw_image (Image *i)
    {
    }

    // style
    virtual void
    load_fill_color (int r, int g, int b)
    {
    }
    virtual void
    load_outline_color (int r, int g, int b)
    {
    }
    virtual void
    load_fill_rule (djnFillRuleType rule)
    {
    }
    virtual void
    load_no_outline ()
    {
    }
    virtual void
    load_no_fill ()
    {
    }
    virtual void
    load_texture (const std::string &path)
    {
    }
    virtual void
    load_outline_opacity (float alpha)
    {
    }
    virtual void
    load_fill_opacity (float alpha)
    {
    }
    virtual void
    load_outline_width (double w)
    {
    }
    virtual void
    load_outline_cap_style (djnCapStyle cap)
    {
    }
    virtual void
    load_outline_join_style (djnJoinStyle join)
    {
    }
    virtual void
    load_outline_miter_limit (int limit)
    {
    }
    virtual void
    load_dash_array (vector<double> dash)
    {
    }
    virtual void
    load_no_dash_array ()
    {
    }
    virtual void
    load_dash_offset (double offset)
    {
    }
    virtual void
    load_gradient_stop (int r, int g, int b, float a, float offset)
    {
    }
    virtual void
    load_linear_gradient (LinearGradient *g)
    {
    }
    virtual void
    load_radial_gradient (RadialGradient *g)
    {
    }
    virtual void
    load_font_size (djnLengthUnit unit, double size)
    {
    }
    virtual void
    load_font_weight (int weight)
    {
    }
    virtual void
    load_font_style (djnFontSlope style)
    {
    }
    virtual void
    load_font_family (const string &family)
    {
    }
    virtual void
    load_text_anchor (djnAnchorType anchor)
    {
    }

    // transform
    virtual void
    load_translation (double tx, double ty)
    {
    }
    virtual void
    load_gradient_translation (double tx, double ty)
    {
    }
    virtual void
    load_rotation (double a, double cx, double cy)
    {
    }
    virtual void
    load_gradient_rotation (double a, double cx, double cy)
    {
    }
    virtual void
    load_scaling (double sx, double sy, double cx, double cy)
    {
    }
    virtual void
    load_gradient_scaling (double sx, double sy, double cx, double cy)
    {
    }
    virtual void
    load_skew_x (double a)
    {
    }
    virtual void
    load_gradient_skew_x (double a)
    {
    }
    virtual void
    load_skew_y (double a)
    {
    }
    virtual void
    load_gradient_skew_y (double a)
    {
    }
    virtual void
    load_homography (double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24,
                     double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44)
    {
    }
    virtual void
    load_gradient_homography (double m11, double m12, double m13, double m21, double m22, double m23, double m31,
                              double m32, double m33)
    {
    }
    void
    load_simple_gradient_transform (double a, double b, double c, double d, double e, double f)
    {
      load_gradient_homography (a, b, 0, c, d, 0, e, f, 1);
    }



  protected:
    Window *_window;
  };
}
