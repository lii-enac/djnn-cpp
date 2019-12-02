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

#include <vector>
#include "shapes/shapes.h"
#include "transformation/transformations.h"
#include "style/style_types.h"
#include "style/style.h"
//#include "window.h"

namespace djnn
{
  using namespace std;

  class AbstractBackend
  {
  public:
    AbstractBackend ()/* : _window (nullptr)*/ {
    }

    virtual
    ~AbstractBackend ()
    {
    }
    /*
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
    }*/

    /* shapes */
    virtual void
    draw_rectangle (Rectangle *s)
    {
    }
    virtual void
    draw_circle (Circle *s)
    {
    }
    virtual void
    draw_ellipse (Ellipse *s)
    {
    }
    virtual void
    draw_line (Line *s)
    {
    }
    virtual void
    draw_text (Text*)
    {
    }
    virtual double
    get_cursor_from_index (Text*, int)
    {
      return 0;
    }
    virtual std::pair<double,int>
    get_cursor_from_local_x (Text*, double)
    {
      return std::pair<double,int>(0,0);
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
    draw_rectangle_clip (RectangleClip *s)
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

    virtual void
    pick_gshape (AbstractGShape *s)
    {
    }

    /* style */
    virtual void
    load_fill_color (AbstractColor *c)
    {
    }
    virtual void
    load_outline_color (AbstractColor *c)
    {
    }
    virtual void
    load_fill_rule (FillRule *fr)
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
    load_texture (Texture *t)
    {
    }
    virtual void
    load_outline_opacity (OutlineOpacity *oo)
    {
    }
    virtual void
    load_fill_opacity (FillOpacity *fo)
    {
    }
    virtual void
    load_outline_width (OutlineWidth *ow)
    {
    }
    virtual void
    load_outline_cap_style (OutlineCapStyle *ocs)
    {
    }
    virtual void
    load_outline_join_style (OutlineJoinStyle *ojs)
    {
    }
    virtual void
    load_outline_miter_limit (OutlineMiterLimit *oml)
    {
    }
    virtual void
    load_dash_array (DashArray *da)
    {
    }
    virtual void
    load_no_dash_array ()
    {
    }
    virtual void
    load_dash_offset (DashOffset *od)
    {
    }
    virtual void
    load_gradient_stop (GradientStop *gs)
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
    load_font_size (FontSize *fs)
    {
    }
    virtual void
    load_font_weight (FontWeight *fw)
    {
    }
    virtual void
    load_font_style (FontStyle *fs)
    {
    }
    virtual void
    load_font_family (FontFamily *ff)
    {
    }
    virtual void
    load_text_anchor (TextAnchor *a)
    {
    }

    /* transform */
    virtual void
    load_translation (Translation*)
    {
    }
    virtual void
    load_gradient_translation (GradientTranslation*)
    {
    }
    virtual void
    load_rotation (Rotation*)
    {
    }
    virtual void
    load_gradient_rotation (GradientRotation*)
    {
    }
    virtual void
    load_scaling (Scaling*)
    {
    }
    virtual void
    load_gradient_scaling (GradientScaling*)
    {
    }
    virtual void
    load_skew_x (SkewX*)
    {
    }
    virtual void
    load_gradient_skew_x (GradientSkewX*)
    {
    }
    virtual void
    load_skew_y (SkewY*)
    {
    }
    virtual void
    load_gradient_skew_y (GradientSkewY*)
    {
    }
    virtual void
    load_homography (AbstractHomography*)
    {
    }
    virtual void
    load_gradient_homography (AbstractHomography*)
    {
    }
    void
    load_simple_gradient_transform (AbstractHomography* g)
    {
      //get_property
      //load_gradient_homography (g, a, b, 0, c, d, 0, e, f, 1);
    }

    virtual void
    update_text_geometry (Text* text, FontFamily* ff, FontSize* fsz, FontStyle* fs, FontWeight *fw)
    {
    }

    virtual void
    delete_text_impl (Text* text)
    {
    }

    virtual void
    delete_image_impl (Image* image)
    {
    }

    virtual void
    activate_gobj (AbstractGObj*)
    {   
    }

    virtual void
    deactivate_gobj (AbstractGObj*)
    {   
    }

  protected:
    //Window *_window;
  };
}
