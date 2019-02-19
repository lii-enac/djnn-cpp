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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "../backend.h"

#include "cairo_context.h"
#include "cairo_backend.h"

#include <iostream>
#include <cmath>

#define CACHE_GEOMETRY 0

namespace djnn
{
  static void
  draw_cairo_rect (double x, double y, double w, double h, double rx, double ry, cairo_t* state)
  {
    if (rx == 0 && ry == 0)
      cairo_rectangle (state, x, y, w, h);
    /* taken from http://cairographics.org/cookbook/roundedrectangles/ */
    else {
      double ARC_TO_BEZIER = 0.55228475;
      if (rx > w - rx)
        rx = w / 2;
      if (ry > h - ry)
        ry = w / 2;

      /* approximate (quite close) the arc using a bezier curve */
      double c1 = ARC_TO_BEZIER * rx;
      double c2 = ARC_TO_BEZIER * ry;
      cairo_move_to (state, x + rx, y);
      cairo_rel_line_to (state, w - 2 * rx, 0.0);
      cairo_rel_curve_to (state, c1, 0.0, rx, c2, rx, ry);
      cairo_rel_line_to (state, 0, h - 2 * ry);
      cairo_rel_curve_to (state, 0.0, c2, c1 - rx, ry, -rx, ry);
      cairo_rel_line_to (state, -w + 2 * rx, 0);
      cairo_rel_curve_to (state, -c1, 0, -rx, -c2, -rx, -ry);
      cairo_rel_line_to (state, 0, -h + 2 * ry);
      cairo_rel_curve_to (state, 0.0, -c2, rx - c1, -ry, rx, -ry);
      cairo_close_path (state);
    }
  }

  struct CairoBackend::bounding_box {
    double x, y, w, h;
  };

  bool
  CairoBackend::test_cache(AbstractGShape *s)
  {
    ShapeImpl* cache = (ShapeImpl*) s->impl ();
    return (cache == nullptr || (s->get_damaged () & (notify_damaged_geometry))
        || (_context_manager->get_current ()->get_damaged () & notify_damaged_transform));
  }

  void
  CairoBackend::build_cache(AbstractGShape *s, const std::function <void (bounding_box& bbox)>& get_bbox, const std::function <void ()>& draw)
  {
    ShapeImpl* cache = (ShapeImpl*) s->impl ();
    if (cache) {
      delete cache;
      cache = nullptr;
    }

    double lw = cairo_get_line_width(cur_cairo_state); // stroke width
    double surr = lw/2.; // will be transformed
    double aaw = 1.0; // antialiasing in *pixels*, should not be transformed

    bounding_box bbox;
    get_bbox(bbox);
    bbox.x -= surr; bbox.y -= surr;
    bbox.w += surr*2; bbox.h += surr*2;
    bounding_box tbbox=bbox; // bbox to be transformed

    cairo_matrix_t mm;
    cairo_get_matrix (cur_cairo_state, &mm);
    
    //tbbox.x = bbox.x; tbbox.y = bbox.y;
    cairo_matrix_transform_point (&mm, &tbbox.x, &tbbox.y);
    tbbox.x -= aaw; tbbox.y -= aaw;

    //tbbox.w = bbox.w; tbbox.h = bbox.h;
    cairo_matrix_transform_distance (&mm, &tbbox.w, &tbbox.h);
    tbbox.w += aaw*2; tbbox.h += aaw*2;

    cairo_save (cur_cairo_state);
    cairo_identity_matrix(cur_cairo_state);
    cairo_translate (cur_cairo_state, -tbbox.x, -tbbox.y);
    cairo_transform (cur_cairo_state, &mm);

    cairo_push_group (cur_cairo_state); // FIXME should use surface directly
    draw();
    fill_and_stroke ();

    cairo_pattern_t * pattern;
    pattern = cairo_pop_group (cur_cairo_state);
    cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

    cairo_restore (cur_cairo_state);

    // we keep the non-transformed translation for further drawing, but use the transformed dimensions
    cache = new ShapeImpl (pattern, bbox.x, bbox.y, tbbox.w, tbbox.h);
    s->set_impl (cache);
  }

  void
  CairoBackend::draw_cache(AbstractGShape *s)
  {
    ShapeImpl* cache = (ShapeImpl*) s->impl ();
    cairo_matrix_t mm;
    cairo_surface_t *surface;
    double tx=cache->x(), ty=cache->y();

    cairo_save (cur_cairo_state);
    cairo_get_matrix (cur_cairo_state, &mm);
    cairo_matrix_transform_point (&mm, &tx, &ty); // find current translation for our cache
    cairo_matrix_init_translate (&mm, tx, ty ); // forget other transformations (e.g. scale) and apply current translation
    cairo_set_matrix(cur_cairo_state, &mm);
    cairo_rectangle (cur_cairo_state, 0, 0, cache->w (), cache->h ());
    cairo_pattern_get_surface(cache->pattern (), &surface);
    cairo_set_source_surface(cur_cairo_state, surface, 0,0);
    cairo_fill_preserve (cur_cairo_state);
    cairo_new_path (cur_cairo_state);
    cairo_restore (cur_cairo_state);
  }

  void
  CairoBackend::draw_rect (Rectangle *s)
  {
    if (!cur_cairo_state)
      return;
    double x, y, w, h, rx, ry;
    #if CACHE_GEOMETRY
    if(test_cache(s)) {
      s->get_properties_values (x, y, w, h, rx, ry);
      build_cache (s,
        [&](bounding_box& bbox) {   
          bbox = {x, y, w, h};
        },
        [&]() {
          draw_cairo_rect (x, y, w, h, rx, ry, cur_cairo_state);
        }
      );
    }
    draw_cache(s);
    #else
    s->get_properties_values (x, y, w, h, rx, ry);
    draw_cairo_rect (x, y, w, h, rx, ry, cur_cairo_state);
    fill_and_stroke ();
    #endif

    if (is_in_picking_view (s)) {
      s->get_properties_values (x, y, w, h, rx, ry);
      draw_cairo_rect (x, y, w, h, rx, ry, cur_cairo_picking_state);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (s);
    }
  }

  void
  CairoBackend::draw_circle (Circle *s)
  {
    if (!cur_cairo_state)
      return;
    double cx, cy, r;
    #if CACHE_GEOMETRY
    if(test_cache(s)) {
      s->get_properties_values (cx, cy, r);
      build_cache (s,
        [&](bounding_box& bbox) {
          bbox = {cx-r, cy-r, r*2, r*2};
        },
        [&]() {
          cairo_arc (cur_cairo_state, cx, cy, r, 0., 2 * 3.14159265);
        }
      );
    }
    draw_cache(s);
    #else
    s->get_properties_values (cx, cy, r);
    cairo_arc (cur_cairo_state, cx, cy, r, 0., 2 * 3.14159265);
    fill_and_stroke ();
    #endif

    if (is_in_picking_view (s)) {
      s->get_properties_values (cx, cy, r);
      cairo_arc (cur_cairo_picking_state, cx, cy, r, 0., 2 * 3.14159265);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (s);
    }
  }

  static void
  draw_cairo_ellipse (double cx, double cy, double rx, double ry, cairo_t* state)
  {
    cairo_save (state);
    cairo_translate (state, cx, cy);
    cairo_scale (state, rx, ry);
    cairo_arc (state, 0., 0., 1., 0., 2 * 3.14159265);
    cairo_restore (state);
  }

  void
  CairoBackend::draw_ellipse (Ellipse *s)
  {
    if (!cur_cairo_state)
      return;
    double cx, cy, rx, ry;
    #if CACHE_GEOMETRY
    if(test_cache(s)) {
      s->get_properties_values (cx, cy, rx, ry);
      build_cache (s,
        [&](bounding_box& bbox) {
          bbox = {cx-rx, cy-ry, rx*2, ry*2};
        },
        [&]() {
          draw_cairo_ellipse (cx, cy, rx, ry, cur_cairo_picking_state);
        }
      );
    }
    draw_cache(s);
    #else
    s->get_properties_values (cx, cy, rx, ry);
    draw_cairo_ellipse (cx, cy, rx, ry, cur_cairo_state);
    fill_and_stroke ();
    #endif

    if (is_in_picking_view (s)) {
      s->get_properties_values (cx, cy, rx, ry);
      draw_cairo_ellipse (cx, cy, rx, ry, cur_cairo_picking_state);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (s);
    }
  }

  void
  CairoBackend::draw_line (Line *s)
  {
    if (!cur_cairo_state)
      return;
    double x1, y1, x2, y2;
    #if 0//CACHE_GEOMETRY
    if(test_cache(s)) {
      
      s->get_properties_values (x1, y1, x2, y2);
      build_cache (s,
        [&](bounding_box& bbox) {
          bbox = {x1<x2?x1:x2, y1<y2?y1:y2, abs(x2-x1), abs(y2-y1)};
        },
        [&]() {
          cairo_move_to (cur_cairo_state, x1, y1);
          cairo_line_to (cur_cairo_state, x2, y2);
        }
      );
    }
    draw_cache(s);
    #else
    s->get_properties_values (x1, y1, x2, y2);
    cairo_move_to (cur_cairo_state, x1, y1);
    cairo_line_to (cur_cairo_state, x2, y2);
    fill_and_stroke ();
    #endif

    if (is_in_picking_view (s)) {
      s->get_properties_values (x1, y1, x2, y2);
      cairo_move_to (cur_cairo_picking_state, x1, y1);
      cairo_line_to (cur_cairo_picking_state, x2, y2);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (s);
    }
  }

  static double factor[] =
    { 1., 1., 1., 16, 1.33, 1., 243.84, 2438.4, 96., 1. };
  static double curTextX = 0.;
  static double curTextY = 0.;
  void
  CairoBackend::draw_text (Text *t)
  {
    if (!cur_cairo_state)
      return;
    TextImpl* cache = (TextImpl*) t->impl ();
    if (cache == nullptr || (t->get_damaged () & (notify_damaged_geometry | notify_damaged_transform))) {
      if (cache)
        delete cache;
      double x, y, dx, dy, fsize;
      int dxU, dyU, width, height, encoding, fstyle, fweight;
      string text, ffamily;
      t->get_properties_values (x, y, dx, dy, fsize, dxU, dyU, width, height, encoding, fstyle, fweight, text, ffamily);
      PangoLayout *layout;
      CairoContext *cur_context = _context_manager->get_current ();
      double dxfactor = factor[dxU];
      double dyfactor = factor[dyU];
      if (dxU == djnPercentLength) {
        int v = (int) _window->width ()->get_value ();
        dx = v * dx / 100;
      }
      if (dyU == djnPercentLength) {
        int v = _window->height ()->get_value ();
        dy = v * dy / 100;
      }

      if (x == -1) {
        x = curTextX;
      }
      if (y == -1) {
        y = curTextY;
      }
      double posX = x + (dx * dxfactor);
      double posY = y + (dy * dyfactor);

      layout = pango_cairo_create_layout (cur_cairo_state);
      pango_layout_set_text (layout, text.c_str (), -1);
      pango_layout_set_font_description (layout, cur_context->_font);
      pango_layout_get_pixel_size (layout, &width, &height);

      int b = (int) (pango_layout_get_baseline (layout) / PANGO_SCALE);

      /* applying alignment attribute */
      switch (cur_context->_textAnchor)
        {
        case djnMiddleAnchor:
          posX = posX - (width / 2.0);
          break;
        case djnEndAnchor:
          posX = posX - width;
          break;
        }
      cache = new TextImpl (posX, posY, width, height, b, layout);
      t->reset_damaged ();
      t->set_impl (cache);
    }

    cairo_save (cur_cairo_state);
    //pango_cairo_update_layout (cur_cairo_state, cache->layout ()); // souhld we update de the layout?
    cairo_move_to (cur_cairo_state, cache->_posX, cache->_posY - cache->_b);
    curTextX = cache->_posX + cache->_width;
    curTextY = cache->_posY;
    set_fill_source ();
    pango_cairo_show_layout (cur_cairo_state, cache->_layout);
    cairo_restore (cur_cairo_state);

    if (is_in_picking_view (t)) {
      draw_cairo_rect (cache->_posX, cache->_posY - cache->_b, cache->_width, cache->_height, 0, 0,
                       cur_cairo_picking_state);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (t);
    }
  }

  static double dx, dy;
  static cairo_matrix_t mm;
  void
  CairoBackend::draw_poly (Poly* s)
  {
    if (!cur_cairo_state)
      return;

    #if CACHE_GEOMETRY
    if(test_cache(s)) {
      PolyPoint* first_pt = (PolyPoint*) ((Container*) s->points ())->children ()[0];
      build_cache (s,
        [&](bounding_box& bbox) {
          double minx = first_pt->x ()->get_value ();
          double miny = first_pt->y ()->get_value ();
          double maxx = minx;
          double maxy = maxy;
          for (auto pt : ((Container*) s->points ())->children ()) {
            double x = ((PolyPoint*) pt)->x ()->get_value ();
            double y = ((PolyPoint*) pt)->y ()->get_value ();
            if (x < minx)
              minx = x;
            else if(x>maxx)
              maxx = x;
            if (y < miny)
              miny = y;
            else if(y>maxy)
              maxy = y;
          }
          bbox = {minx, miny, maxx-minx, maxy-miny};
          s->set_bounding_box (bbox.x, bbox.y, bbox.w, bbox.h);
        },
        [&]() {
          double init_x = first_pt->x ()->get_value ();
          double init_y = first_pt->y ()->get_value ();
          //cairo_matrix_transform_distance (&mm, &init_x, &init_y);
          cairo_move_to (cur_cairo_state, init_x, init_y);
          s->points ()->draw ();
          if (s->closed ())
            cairo_close_path (cur_cairo_state);
          }
      );
    }
    draw_cache(s);
    #else
    PolyPoint* first_pt = (PolyPoint*) ((Container*) s->points ())->children ()[0];
    double init_x = first_pt->x ()->get_value ();
    double init_y = first_pt->y ()->get_value ();
    //cairo_matrix_transform_distance (&mm, &init_x, &init_y);
    cairo_move_to (cur_cairo_state, init_x, init_y);
    s->points ()->draw ();
    if (s->closed ())
      cairo_close_path (cur_cairo_state);

    double x1, y1, x2, y2;
    cairo_path_extents (cur_cairo_state, &x1, &y1, &x2, &y2);

    int w = x2 - x1;
    int h = y2 - y1;
    s->set_bounding_box (dx, dy, w, h);
    fill_and_stroke ();
    #endif
    
    if (is_in_picking_view (s)) {
      PolyPoint* first_pt = (PolyPoint*) ((Container*) s->points ())->children ()[0];
      cairo_move_to (cur_cairo_picking_state, first_pt->x ()->get_value (), first_pt->y ()->get_value ());
      for (int i = 1; i < ((Container*) s->points ())->children ().size (); ++i) {
        PolyPoint* polypoint = (PolyPoint*) ((Container*) s->points ())->children ()[i];
        cairo_line_to (cur_cairo_picking_state, polypoint->x ()->get_value (), polypoint->y ()->get_value ());
      }
      if (s->closed ())
        cairo_close_path (cur_cairo_picking_state);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (s);
    }
  }

  void
  CairoBackend::draw_poly_point (double x, double y)
  {
    cairo_line_to (cur_cairo_state, x, y);
  }

  static bool in_picking_view = false;
  void
  CairoBackend::draw_path (Path *p)
  {
    if (!cur_cairo_state)
      return;
    in_picking_view = is_in_picking_view (p);
    p->items ()->draw ();
    double x, y, w, h;
    cairo_path_extents (cur_cairo_state, &x, &y, &w, &h);
    p->set_bounding_box (x, y, w, h);
    fill_and_stroke ();

    if (in_picking_view) {
      pick_fill_and_stroke ();
      _pick_view->add_gobj (p);
    }
    in_picking_view = false;
  }

  void
  CairoBackend::draw_path_move (double x, double y)
  {
    cairo_move_to (cur_cairo_state, x, y);
    if (in_picking_view)
      cairo_move_to (cur_cairo_picking_state, x, y);
  }

  void
  CairoBackend::draw_path_line (double x, double y)
  {
    cairo_line_to (cur_cairo_state, x, y);
    if (in_picking_view)
      cairo_line_to (cur_cairo_picking_state, x, y);
  }

  void
  CairoBackend::draw_path_quadratic (double x1, double y1, double x2, double y2)
  {
    double x0, y0;
    cairo_get_current_point (cur_cairo_state, &x0, &y0);
    cairo_curve_to (cur_cairo_state, 2.0 / 3.0 * x1 + 1.0 / 3.0 * x0, 2.0 / 3.0 * y1 + 1.0 / 3.0 * y0,
                    2.0 / 3.0 * x1 + 1.0 / 3.0 * x2, 2.0 / 3.0 * y1 + 1.0 / 3.0 * y2, x2, y2);
    if (in_picking_view) {
      cairo_get_current_point (cur_cairo_picking_state, &x0, &y0);
      cairo_curve_to (cur_cairo_picking_state, 2.0 / 3.0 * x1 + 1.0 / 3.0 * x0, 2.0 / 3.0 * y1 + 1.0 / 3.0 * y0,
                      2.0 / 3.0 * x1 + 1.0 / 3.0 * x2, 2.0 / 3.0 * y1 + 1.0 / 3.0 * y2, x2, y2);
    }
  }

  void
  CairoBackend::draw_path_cubic (double x1, double y1, double x2, double y2, double x, double y)
  {
    cairo_curve_to (cur_cairo_state, x1, y1, x2, y2, x, y);
    if (in_picking_view)
      cairo_curve_to (cur_cairo_picking_state, x1, y1, x2, y2, x, y);
  }

  /*
   * the arc handling code underneath is from XSVG,
   * reused under the terms reproduced in xsvg.license.terms
   */
#define PI 3.14159265359
  static void
  draw_path_segment (double xc, double yc, double th0, double th1, double rx, double ry, double xAxisRotation,
                     cairo_t* state)
  {
    double sinTh, cosTh;
    double a00, a01, a10, a11;
    double x1, y1, x2, y2, x3, y3;
    double t;
    double thHalf;

    sinTh = sin (xAxisRotation * (PI / 180.0));
    cosTh = cos (xAxisRotation * (PI / 180.0));

    a00 = cosTh * rx;
    a01 = -sinTh * ry;
    a10 = sinTh * rx;
    a11 = cosTh * ry;

    thHalf = 0.5 * (th1 - th0);
    t = (8.0 / 3.0) * sin (thHalf * 0.5) * sin (thHalf * 0.5) / sin (thHalf);
    x1 = xc + cos (th0) - t * sin (th0);
    y1 = yc + sin (th0) + t * cos (th0);
    x3 = xc + cos (th1);
    y3 = yc + sin (th1);
    x2 = x3 + t * sin (th1);
    y2 = y3 - t * cos (th1);

    cairo_curve_to (state, a00 * x1 + a01 * y1, a10 * x1 + a11 * y1, a00 * x2 + a01 * y2, a10 * x2 + a11 * y2,
                    a00 * x3 + a01 * y3, a10 * x3 + a11 * y3);
  }
  static void
  draw_cairo_path_arc (double rx, double ry, double x_axis_rotation, double large_arc_flag, double sweep_flag, double x,
                       double y, cairo_t* state)
  {
    double curx, cury;
    cairo_get_current_point (state, &curx, &cury);
    double sin_th, cos_th;
    double a00, a01, a10, a11;
    double x0, y0, x1, y1, xc, yc;
    double d, sfactor, sfactor_sq;
    double th0, th1, th_arc;
    int i, n_segs;
    double dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;

    rx = fabs (rx);
    ry = fabs (ry);

    sin_th = sin (x_axis_rotation * (PI / 180.0));
    cos_th = cos (x_axis_rotation * (PI / 180.0));

    dx = (curx - x) / 2.0;
    dy = (cury - y) / 2.0;
    dx1 = cos_th * dx + sin_th * dy;
    dy1 = -sin_th * dx + cos_th * dy;
    Pr1 = rx * rx;
    Pr2 = ry * ry;
    Px = dx1 * dx1;
    Py = dy1 * dy1;
    /* check if radii are large enough */
    check = Px / Pr1 + Py / Pr2;
    if (check > 1) {
      rx = rx * sqrt (check);
      ry = ry * sqrt (check);
    }

    a00 = cos_th / rx;
    a01 = sin_th / rx;
    a10 = -sin_th / ry;
    a11 = cos_th / ry;
    x0 = a00 * curx + a01 * cury;
    y0 = a10 * curx + a11 * cury;
    x1 = a00 * x + a01 * y;
    y1 = a10 * x + a11 * y;
    /* (x0, y0) is current point in transformed coordinate space.
     (x1, y1) is new point in transformed coordinate space.
     The arc fits a unit-radius circle in this space.
     */
    d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
    sfactor_sq = 1.0 / d - 0.25;
    if (sfactor_sq < 0)
      sfactor_sq = 0;
    sfactor = sqrt (sfactor_sq);
    if (sweep_flag == large_arc_flag)
      sfactor = -sfactor;
    xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
    yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);
    /* (xc, yc) is center of the circle. */

    th0 = atan2 (y0 - yc, x0 - xc);
    th1 = atan2 (y1 - yc, x1 - xc);

    th_arc = th1 - th0;
    if (th_arc < 0 && sweep_flag)
      th_arc += 2 * PI;
    else if (th_arc > 0 && !sweep_flag)
      th_arc -= 2 * PI;

    n_segs = ceil (fabs (th_arc / (PI * 0.5 + 0.001)));

    for (i = 0; i < n_segs; i++)
      draw_path_segment (xc, yc, th0 + i * th_arc / n_segs, th0 + (i + 1) * th_arc / n_segs, rx, ry, x_axis_rotation,
                         state);
  }

  void
  CairoBackend::draw_path_arc (double rx, double ry, double x_axis_rotation, double large_arc_flag, double sweep_flag,
                               double x, double y)
  {
    draw_cairo_path_arc (rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x, y, cur_cairo_state);
    if (in_picking_view)
      draw_cairo_path_arc (rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x, y, cur_cairo_picking_state);
  }

  void
  CairoBackend::draw_path_closure ()
  {
    cairo_close_path (cur_cairo_state);
  }

  void
  CairoBackend::draw_rect_clip (RectangleClip *s)
  {
    if (!cur_cairo_state)
      return;
    double x, y, w, h;
    s->get_properties_values (x, y, w, h);
    cairo_rectangle (cur_cairo_state, x, y, w, h);
    cairo_clip (cur_cairo_state);
    if (is_in_picking_view (s)) {
      cairo_rectangle (cur_cairo_picking_state, x, y, w, h);
    }
  }

  void
  CairoBackend::draw_path_clip (Path *p)
  {
    in_picking_view = is_in_picking_view (p);
    if (!cur_cairo_state)
      return;
    p->items ()->draw ();
    double x, y, w, h;
    cairo_path_extents (cur_cairo_state, &x, &y, &w, &h);
    p->set_bounding_box (x, y, w, h);
    cairo_clip (cur_cairo_state);

    if (in_picking_view) {
      pick_fill_and_stroke ();
      _pick_view->add_gobj (p);
    }
    in_picking_view = false;
  }

  void
  CairoBackend::draw_image (Image *i)
  {
    double x, y, w, h;
    string path;
    i->get_properties_values (path, x, y, w, h);
    cairo_surface_t* image;
    if (i->invalid_cache ()) {
      if (i->cache () != nullptr) {
        cairo_surface_destroy ((cairo_surface_t*) i->cache ());
      }
      image = cairo_image_surface_create_from_png (path.c_str ());
      i->set_cache (image);
      i->set_invalid_cache (false);
    } else {
      image = (cairo_surface_t*) (i->cache ());
    }
    cairo_save (cur_cairo_state);
    cairo_translate (cur_cairo_state, x, y);
    double src_w = cairo_image_surface_get_width (image);
    double src_h = cairo_image_surface_get_height (image);
    cairo_scale (cur_cairo_state, w / src_w, h / src_h);
    cairo_set_source_surface (cur_cairo_state, image, 0, 0);
    cairo_paint (cur_cairo_state);
    cairo_restore (cur_cairo_state);

    if (is_in_picking_view (i)) {
      draw_cairo_rect (x, y, w, h, 0, 0, cur_cairo_picking_state);
      pick_fill_and_stroke ();
      _pick_view->add_gobj (i);
    }
  }
} /* namespace djnn */
