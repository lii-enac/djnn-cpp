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

#include "../backend.h"

#include "qt_context.h"
#include "qt_backend.h"
#include "qt_window.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include <iostream>
#include <cmath>

namespace djnn
{
  void
  QtBackend::draw_rect (AbstractGShape *s, double x, double y, double w, double h, double rx, double ry)
  {
    if (_painter == nullptr)
      return;
    load_drawing_context (x, y, w, h);
    _painter->drawRoundedRect (x, y, w, h, rx, ry);

    if (is_in_picking_view (s)) {
      load_pick_context (s);
      _picking_view->painter ()->drawRoundedRect (x, y, w, h, rx, ry);
    }
  }

  void
  QtBackend::draw_circle (AbstractGShape *s, double cx, double cy, double r)
  {
    if (_painter == nullptr)
      return;
    QRectF rect (cx - r, cy - r, 2 * r, 2 * r);
    load_drawing_context (rect.x (), rect.y (), rect.width (), rect.height ());
    _painter->drawEllipse (rect);

    if (is_in_picking_view (s)) {
      load_pick_context (s);
      _picking_view->painter ()->drawEllipse (rect);
    }
  }

  void
  QtBackend::draw_ellipse (AbstractGShape *s, double cx, double cy, double rx, double ry)
  {
    if (_painter == nullptr)
      return;
    QRect rect (cx - rx, cy - ry, 2 * rx, 2 * ry);
    load_drawing_context (rect.x (), rect.y (), rect.width (), rect.height ());
    _painter->drawEllipse (rect);

    if (is_in_picking_view (s)) {
      load_pick_context (s);
      _picking_view->painter ()->drawEllipse (rect);
    }
  }

  void
  QtBackend::draw_line (AbstractGShape *s, double x1, double y1, double x2, double y2)
  {
    if (_painter == nullptr)
      return;
    QLineF line (x1, y1, x2, y2);
    load_drawing_context (x1, y1, sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)), 1);
    _painter->drawLine (line);

    if (is_in_picking_view (s)) {
      load_pick_context (s);
      _picking_view->painter ()->drawLine (line);
    }
  }

  static double curTextX = 0.;
  static double curTextY = 0.;
  void
  QtBackend::draw_text (Text *t)
  {
    double x = t->x ()->get_value ();
    double y = t->y ()->get_value ();
    double dx = t->dx ()->get_value ();
    double dy = t->dy ()->get_value ();
    int dxU = t->dxU ()->get_value ();
    int dyU = t->dyU ()->get_value ();
    int width = t->width ()->get_value ();
    int height = t->height ()->get_value ();
    int encoding = t->encoding ()->get_value ();
    std::string text = t->text ()->get_value ();
    shared_ptr<QtContext> cur_context = _context_manager->get_current ();
    double dxfactor = cur_context->factor[dxU];
    double dyfactor = cur_context->factor[dyU];

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
    QPointF p (posX, posY);

    QString s;
    switch (encoding)
      {
      case djnLatin1:
      case djnAscii:
        s = QString::fromLatin1 (text.c_str ());
        break;
      case djnUtf8:
      default:
        s = QString::fromUtf8 (text.c_str ());
      }

    /* dummy value for width-height parameters because we do not manage
     gradient for text object
     */
    load_drawing_context (x, y, 1, 1);

    /* Qt draws text with the outline color
     but we want it to use the fill color */
    QPen oldPen = cur_context->pen;
    QPen newPen (oldPen);
    newPen.setColor (cur_context->brush.color ());
    if (cur_context->brush.style () == Qt::SolidPattern)
      newPen.setStyle (Qt::SolidLine);
    else
      newPen.setStyle (Qt::NoPen);
    _painter->setPen (newPen);
    _painter->setFont (cur_context->font);

    QFontMetrics fm = _painter->fontMetrics ();
    QRect rect = fm.boundingRect (s);
    if (width != rect.width ())
      t->width ()->set_value (rect.width (), true);
    if (height != rect.height ())
      t->height ()->set_value (rect.height (), true);

    /* applying alignment attribute */
    switch (cur_context->textAnchor)
      {
      case djnMiddleAnchor:
        posX = posX - (rect.width () / 2.0);
        p.setX (posX);
        break;
      case djnEndAnchor:
        posX = posX - rect.width ();
        p.setX (posX);
        break;
      }

    rect.moveTo (posX, posY - rect.height ());
    curTextX = rect.x () + fm.width (s);
    curTextY = rect.y () + fm.height ();

    _painter->drawText (p, s);

    /* Don't forget to reset the old pen color */
    _painter->setPen (oldPen);

    if (is_in_picking_view (t)) {
      load_pick_context (t);
      _picking_view->painter ()->drawRect (rect);
    }
  }

  void
  QtBackend::draw_poly (Poly* p)
  {
    QPainterPath path;
    cur_poly = QPolygonF ();
    p->points ()->draw ();
    path.addPolygon (cur_poly);
    if (p->closed ())
      path.closeSubpath ();
    path.setFillRule (_context_manager->get_current ()->fillRule);
    load_drawing_context (path.boundingRect ().x (), path.boundingRect ().y (), path.boundingRect ().width (),
                          path.boundingRect ().height ());
    _painter->drawPath (path);

    if (is_in_picking_view (p)) {
      load_pick_context (p);
      _picking_view->painter ()->drawPath (path);
    }
  }

  void
  QtBackend::draw_poly_point (double x, double y)
  {
    cur_poly << QPointF (x, y);
  }
  void
  QtBackend::draw_path (Path *p)
  {
    cur_path = QPainterPath ();
    p->items ()->draw ();
    cur_path.setFillRule (_context_manager->get_current ()->fillRule);
    load_drawing_context (cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                          cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());
    _painter->drawPath (cur_path);

    if (is_in_picking_view (p)) {
      load_pick_context (p);
      _picking_view->painter ()->drawPath (cur_path);
    }
  }

  void
  QtBackend::draw_path_move (double x, double y)
  {
    cur_path.moveTo (x, y);
  }

  void
  QtBackend::draw_path_line (double x, double y)
  {
    cur_path.lineTo (x, y);
  }

  void
  QtBackend::draw_path_quadratic (double x1, double y1, double x, double y)
  {
    QPointF c (x1, y1);
    QPointF end (x, y);
    cur_path.quadTo (c, end);
  }

  void
  QtBackend::draw_path_cubic (double x1, double y1, double x2, double y2, double x, double y)
  {
    QPointF c1 (x1, y1);
    QPointF c2 (x2, y2);
    QPointF pt (x, y);
    cur_path.cubicTo (c1, c2, pt);
  }

  /*
   * the arc handling code underneath is from XSVG,
   * reused under the terms reproduced in xsvg.license.terms
   */
#define PI 3.14159265359
  void
  QtBackend::draw_path_arc (double rx, double ry, double x_axis_rotation, double large_arc_flag, double sweep_flag,
                            double x, double y)
  {
    double curx = cur_path.currentPosition ().x ();
    double cury = cur_path.currentPosition ().y ();
    double sin_th, cos_th;
    double a00, a01, a10, a11;
    double x0, y0, x1, y1, xc, yc;
    double d, sfactor, sfactor_sq;
    double th0, th1, th_arc;
    int i, n_segs;
    double dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;
    rx = qAbs (rx);
    ry = qAbs (ry);

    sin_th = qSin (x_axis_rotation * (PI / 180.0));
    cos_th = qCos (x_axis_rotation * (PI / 180.0));

    dx = (curx - x) / 2.0;
    dy = (cury - y) / 2.0;
    dx1 = cos_th * dx + sin_th * dy;
    dy1 = -sin_th * dx + cos_th * dy;
    Pr1 = rx * rx;
    Pr2 = ry * ry;
    Px = dx1 * dx1;
    Py = dy1 * dy1;
    /* Spec : check if radii are large enough */
    check = Px / Pr1 + Py / Pr2;
    if (check > 1) {
      rx = rx * qSqrt (check);
      ry = ry * qSqrt (check);
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
    sfactor = qSqrt (sfactor_sq);
    if (sweep_flag == large_arc_flag)
      sfactor = -sfactor;
    xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
    yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);
    /* (xc, yc) is center of the circle. */

    th0 = qAtan2 (y0 - yc, x0 - xc);
    th1 = qAtan2 (y1 - yc, x1 - xc);

    th_arc = th1 - th0;
    if (th_arc < 0 && sweep_flag)
      th_arc += 2 * PI;
    else if (th_arc > 0 && !sweep_flag)
      th_arc -= 2 * PI;

    n_segs = qCeil (qAbs (th_arc / (PI * 0.5 + 0.001)));

    for (i = 0; i < n_segs; i++) {
      draw_path_segment (xc, yc, th0 + i * th_arc / n_segs, th0 + (i + 1) * th_arc / n_segs, rx, ry, x_axis_rotation);
    }
  }

  void
  QtBackend::draw_path_segment (double xc, double yc, double th0, double th1, double rx, double ry,
                                double xAxisRotation)
  {
    double sinTh, cosTh;
    double a00, a01, a10, a11;
    double x1, y1, x2, y2, x3, y3;
    double t;
    double thHalf;

    sinTh = qSin (xAxisRotation * (PI / 180.0));
    cosTh = qCos (xAxisRotation * (PI / 180.0));

    a00 = cosTh * rx;
    a01 = -sinTh * ry;
    a10 = sinTh * rx;
    a11 = cosTh * ry;

    thHalf = 0.5 * (th1 - th0);
    t = (8.0 / 3.0) * qSin (thHalf * 0.5) * qSin (thHalf * 0.5) / qSin (thHalf);
    x1 = xc + qCos (th0) - t * qSin (th0);
    y1 = yc + qSin (th0) + t * qCos (th0);
    x3 = xc + qCos (th1);
    y3 = yc + qSin (th1);
    x2 = x3 + t * qSin (th1);
    y2 = y3 - t * qCos (th1);

    cur_path.cubicTo (a00 * x1 + a01 * y1, a10 * x1 + a11 * y1, a00 * x2 + a01 * y2, a10 * x2 + a11 * y2,
                      a00 * x3 + a01 * y3, a10 * x3 + a11 * y3);

  }

  void
  QtBackend::draw_path_closure ()
  {
    cur_path.closeSubpath ();
  }

  void
  QtBackend::draw_rect_clip (AbstractGShape *s, double x, double y, double w, double h)
  {
    load_drawing_context (x, y, w, h);
    _painter->setClipRect (x, y, w, h);

    if (is_in_picking_view (s)) {
      load_pick_context (s);
      _picking_view->painter ()->setClipRect (x, y, w, h);
    }
  }

  void
  QtBackend::draw_path_clip (Path *p)
  {
    cur_path = QPainterPath ();
    p->items ()->draw ();
    cur_path.setFillRule (_context_manager->get_current ()->fillRule);
    load_drawing_context (cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                          cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());
    _painter->setClipPath (cur_path);

    if (is_in_picking_view (p)) {
      load_pick_context (p);
      _picking_view->painter ()->setClipPath (cur_path);
    }
  }

  void
  QtBackend::draw_image (Image *i)
  {
    double x = i->x ()->get_value ();
    double y = i->y ()->get_value ();
    double w = i->width ()->get_value ();
    double h = i->height ()->get_value ();
    string path = i->path ()->get_value ();
    load_drawing_context (x, y, w, h);
    QRect rect (x, y, w, h);
    QPixmap *pm;
    if (i->invalid_cache ()) {
      if (i->cache () != nullptr) {
        pm = (QPixmap*) (i->cache ());
        delete (pm);
      }
      QFileInfo fi (path.c_str ());
      pm = new QPixmap (fi.absoluteFilePath ());
      i->set_cache (pm);
      i->set_invalid_cache (false);
    } else {
      pm = (QPixmap*) (i->cache ());
    }
    _painter->drawPixmap (rect, *pm);

    if (is_in_picking_view (i)) {
      load_pick_context (i);
      _picking_view->painter ()->drawPixmap (rect, *pm);
    }
  }
} /* namespace djnn */
