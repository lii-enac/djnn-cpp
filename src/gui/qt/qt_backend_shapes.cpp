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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <QPicture>
#include <QtCore/QFileInfo>
#include <QtCore/QtMath>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
#include <cmath>

#include "core/tree/list.h"
#include "core/utils/algorithm.h"
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "core/utils/remotery.h"
#include "display/abstract_display.h"
#include "display/qt/qt_window.h"
#include "display/qt/qt_window_moc.h"
#include "exec_env/exec_env-dev.h"
#include "gui/backend.h"
#include "gui/gui-dev.h"
#include "gui/layer.h"
#include "gui/shape/shapes.h"
#include "qt_backend.h"
#include "qt_context.h"
#include "utils/debug.h"

#if _DEBUG_SEE_GUI_INFO_PREF
/* define in gui.cpp */
extern int __nb_Drawing_object;
extern int __nb_Drawing_object_picking;
#endif

namespace djnn {

using djnnstl::cerr;
using djnnstl::cout;
using djnnstl::endl;

bool
compare_z_order (QtVectorShapeToDraw* i, QtVectorShapeToDraw* j)
{
    return (i->z_order () < j->z_order ());
}

void
QtBackend::draw_rectangle (Rectangle* s)
{
    rmt_BeginCPUSample (draw_rectangle, RMTSF_Aggregate);
    if (_painter == nullptr)
        return;
    double x, y, w, h, rx, ry;

    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (s, _context);
        return;
    }

    s->get_properties_values (x, y, w, h, rx, ry);
    load_drawing_context (s, _context, x, y, w, h);
    _painter->drawRoundedRect (QRectF (x, y, w, h), rx, ry, Qt::AbsoluteSize);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (s)) {
        rmt_BeginCPUSample (draw_rectangle_picking, RMTSF_Aggregate);
        load_pick_context (s, _context);
        _picking_view->painter ()->drawRoundedRect (QRectF (x, y, w, h), rx, ry, Qt::AbsoluteSize);

#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

void
QtBackend::draw_z_ordered_group (ZOrderedGroup* g)
{
    rmt_BeginCPUSample (draw_z_ordered_group, RMTSF_Aggregate);
    if (_painter == nullptr)
        return;
    cur_context           = nullptr;
    z_processing_step     = 1;
    GUIStructureHolder* h = gui_structure_observer->find_holder (g);
    if (h == nullptr) {
        return;
    }
    for (auto c : h->children ()) {
        c.first->draw ();
    }
    djnnstl::sort (shapes_vectors.begin (), shapes_vectors.end (), compare_z_order);
    z_processing_step = 2;
    for (auto v : shapes_vectors) {
        for (auto item : v->shapes ()) {
            cur_context = &item.second;
            item.first->draw ();
        }
        delete v;
    }
    shapes_vectors.clear ();
    z_processing_step = 0;
    cur_context       = nullptr;
    rmt_EndCPUSample ();
}

void
QtBackend::draw_circle (Circle* s)
{
    rmt_BeginCPUSample (draw_circle, RMTSF_Aggregate);

    if (_painter == nullptr)
        return;
    double cx, cy, r;

    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();

    if (z_processing_step == 1) {
        add_shape (s, _context);
        return;
    }
    s->get_properties_values (cx, cy, r);
    QRectF rect (cx - r, cy - r, 2 * r, 2 * r);
    load_drawing_context (s, _context, rect.x (), rect.y (), rect.width (), rect.height ());
    _painter->drawEllipse (rect);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (s)) {
        rmt_BeginCPUSample (draw_circle_picking, RMTSF_Aggregate);
        load_pick_context (s, _context);
        _picking_view->painter ()->drawEllipse (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

void
QtBackend::draw_ellipse (Ellipse* s)
{
    rmt_BeginCPUSample (draw_ellipse, RMTSF_Aggregate);

    if (_painter == nullptr)
        return;
    double     cx, cy, rx, ry;
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();

    if (z_processing_step == 1) {
        add_shape (s, _context);
        return;
    }

    s->get_properties_values (cx, cy, rx, ry);
    QRectF rect (cx - rx, cy - ry, 2 * rx, 2 * ry);
    load_drawing_context (s, _context, rect.x (), rect.y (), rect.width (), rect.height ());
    _painter->drawEllipse (rect);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (s)) {
        rmt_BeginCPUSample (draw_ellipse_picking, RMTSF_Aggregate);
        load_pick_context (s, _context);
        _picking_view->painter ()->drawEllipse (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }
    rmt_EndCPUSample ();
}

void
QtBackend::draw_line (Line* s)
{
    rmt_BeginCPUSample (draw_line, RMTSF_Aggregate);

    if (_painter == nullptr)
        return;
    double     x1, y1, x2, y2;
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    s->get_properties_values (x1, y1, x2, y2);

    if (z_processing_step == 1) {
        add_shape (s, _context);
        return;
    }

    load_drawing_context (s, _context, x1, y1, sqrt ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)), 1);
    QLineF line (x1, y1, x2, y2);
    _painter->drawLine (line);
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (s)) {
        rmt_BeginCPUSample (draw_line_picking, RMTSF_Aggregate);
        load_pick_context (s, _context);
        _picking_view->painter ()->drawLine (line);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

void
QtBackend::draw_poly (Poly* p)
{
    rmt_BeginCPUSample (draw_poly, RMTSF_Aggregate);
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (p, _context);
        return;
    }
    QPainterPath path;
    cur_poly = QPolygonF ();

    // HACK use GUIStructureHolder::draw instead of container/list::draw
    // p->points ()->draw ();
    p->points_GH ()->draw ();

    path.addPolygon (cur_poly);
    if (p->closed ())
        path.closeSubpath ();
    path.setFillRule (_context->fillRule);
    p->set_bounding_box (path.boundingRect ().x (), path.boundingRect ().y (), path.boundingRect ().width (),
                         path.boundingRect ().height ());
    load_drawing_context (p, _context, path.boundingRect ().x (), path.boundingRect ().y (), path.boundingRect ().width (),
                          path.boundingRect ().height ());
    _painter->drawPath (path);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (p)) {
        rmt_BeginCPUSample (draw_poly_picking, RMTSF_Aggregate);
        load_pick_context (p, _context);
        _picking_view->painter ()->drawPath (path);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

void
QtBackend::draw_poly_point (double x, double y)
{
    cur_poly << QPointF (x, y);
}
void
QtBackend::draw_path (Path* p)
{
    rmt_BeginCPUSample (draw_path, RMTSF_Aggregate);
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (p, _context);
        return;
    }
    cur_path = QPainterPath ();

    // HACK use GUIStructureHolder::draw instead of container/list::draw
    // p->items ()->draw ();
    p->items_GH ()->draw ();

    cur_path.setFillRule (_context->fillRule);
    p->set_bounding_box (cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                         cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());
    load_drawing_context (p, _context, cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                          cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());
    _painter->drawPath (cur_path);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    if (is_in_picking_view (p)) {
        load_pick_context (p, _context);
        rmt_BeginCPUSample (draw_path_picking, RMTSF_Aggregate);
        _picking_view->painter ()->drawPath (cur_path);

#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_move (double x, double y)
{
    rmt_BeginCPUSample (draw_path_add_move, RMTSF_Aggregate);
    cur_path.moveTo (x, y);
    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_line (double x, double y)
{
    rmt_BeginCPUSample (draw_path_add_line, RMTSF_Aggregate);
    cur_path.lineTo (x, y);
    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_quadratic (double x1, double y1, double x, double y)
{
    rmt_BeginCPUSample (draw_path_add_quadratic, RMTSF_Aggregate);
    QPointF c (x1, y1);
    QPointF end (x, y);
    cur_path.quadTo (c, end);
    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_cubic (double x1, double y1, double x2, double y2, double x, double y)
{
    rmt_BeginCPUSample (draw_path_add_cubic, RMTSF_Aggregate);
    QPointF c1 (x1, y1);
    QPointF c2 (x2, y2);
    QPointF end (x, y);
    cur_path.cubicTo (c1, c2, end);
    rmt_EndCPUSample ();
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
    rmt_BeginCPUSample (draw_path_add_arc, RMTSF_Aggregate);

    double curx = cur_path.currentPosition ().x ();
    double cury = cur_path.currentPosition ().y ();
    double sin_th, cos_th;
    double a00, a01, a10, a11;
    double x0, y0, x1, y1, xc, yc;
    double d, sfactor, sfactor_sq;
    double th0, th1, th_arc;
    int    i, n_segs;
    double dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;
    rx = qAbs (rx);
    ry = qAbs (ry);

    sin_th = qSin (x_axis_rotation * (PI / 180.0));
    cos_th = qCos (x_axis_rotation * (PI / 180.0));

    dx  = (curx - x) / 2.0;
    dy  = (cury - y) / 2.0;
    dx1 = cos_th * dx + sin_th * dy;
    dy1 = -sin_th * dx + cos_th * dy;
    Pr1 = rx * rx;
    Pr2 = ry * ry;
    Px  = dx1 * dx1;
    Py  = dy1 * dy1;
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
    x0  = a00 * curx + a01 * cury;
    y0  = a10 * curx + a11 * cury;
    x1  = a00 * x + a01 * y;
    y1  = a10 * x + a11 * y;
    /* (x0, y0) is current point in transformed coordinate space.
     (x1, y1) is new point in transformed coordinate space.
     The arc fits a unit-radius circle in this space.
     */
    d          = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
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

    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_segment (double xc, double yc, double th0, double th1, double rx, double ry,
                              double xAxisRotation)
{
    rmt_BeginCPUSample (draw_path_add_segment, RMTSF_Aggregate);

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
    t      = (8.0 / 3.0) * qSin (thHalf * 0.5) * qSin (thHalf * 0.5) / qSin (thHalf);
    x1     = xc + qCos (th0) - t * qSin (th0);
    y1     = yc + qSin (th0) + t * qCos (th0);
    x3     = xc + qCos (th1);
    y3     = yc + qSin (th1);
    x2     = x3 + t * qSin (th1);
    y2     = y3 - t * qCos (th1);

    cur_path.cubicTo (a00 * x1 + a01 * y1, a10 * x1 + a11 * y1, a00 * x2 + a01 * y2, a10 * x2 + a11 * y2,
                      a00 * x3 + a01 * y3, a10 * x3 + a11 * y3);

    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_closure ()
{
    rmt_BeginCPUSample (draw_path_add_closure, RMTSF_Aggregate);
    cur_path.closeSubpath ();
    rmt_EndCPUSample ();
}

void
QtBackend::draw_rectangle_clip (RectangleClip* s)
{
    rmt_BeginCPUSample (draw_rectangle_clip, RMTSF_Aggregate);

    double x, y, w, h;
    s->get_properties_values (x, y, w, h);
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (s, _context);
        QPainterPath clip;
        clip.addRect (x, y, w, h);
        _context->clipTransform = QTransform (_context->matrix.toTransform ());
        _context->clip          = clip;
        return;
    }
    if (z_processing_step == 2) {
        return;
    }
    load_drawing_context (s, _context, x, y, w, h);
    QPainterPath clip;
    clip.addRect (x, y, w, h);
    _painter->setClipPath (clip, Qt::IntersectClip);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    rmt_BeginCPUSample (draw_rectangle_clip_picking, RMTSF_Aggregate);
    load_pick_context (s, _context_manager->get_current ());
    _picking_view->painter ()->setClipPath (clip, Qt::IntersectClip);
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object_picking++;
#endif
    rmt_EndCPUSample ();

    rmt_EndCPUSample ();
}

void
QtBackend::draw_path_clip (Path* p)
{
    rmt_BeginCPUSample (draw_path_clip, RMTSF_Aggregate);
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    cur_path            = QPainterPath ();
    p->items_GH ()->draw ();
    if (z_processing_step == 1) {
        add_shape (p, _context);
        QPainterPath clip (cur_path);
        _context->clipTransform = QTransform (_context->matrix.toTransform ());
        _context->clip          = clip;
        return;
    }
    if (z_processing_step == 2) {
        return;
    }
    cur_path.setFillRule (_context->fillRule);

    p->set_bounding_box (cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                         cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());
    load_drawing_context (p, _context, cur_path.boundingRect ().x (), cur_path.boundingRect ().y (),
                          cur_path.boundingRect ().width (), cur_path.boundingRect ().height ());

    _painter->setClipPath (cur_path, Qt::IntersectClip); // could be Qt::ReplaceClip

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    rmt_BeginCPUSample (draw_path_clip_picking, RMTSF_Aggregate);
    load_pick_context (p, _context);
    _picking_view->painter ()->setClipPath (cur_path, Qt::IntersectClip);
#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object_picking++;
#endif
    rmt_EndCPUSample ();

    rmt_EndCPUSample ();
}

void
QtBackend::draw_image (Image* i)
{
    rmt_BeginCPUSample (draw_image, RMTSF_Aggregate);

    double     x, y, w, h;
    string     path;
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (i, _context);
        return;
    }
    i->get_properties_values (path, x, y, w, h);
    load_drawing_context (i, _context, x, y, w, h);
    QRect    rect (x, y, w, h);
    QPixmap* pm;
    if (i->invalid_cache ()) {
        if (i->cache () != nullptr) {
            pm = (QPixmap*)(i->cache ());
            delete (pm);
            pm = nullptr;
        }
        QFileInfo fi (path.c_str ());

        if (!fi.exists ())
            warning (nullptr, " Image Loading - file NOT found - path used: " + path);

        pm = new QPixmap (fi.absoluteFilePath ());

        i->set_cache (pm);
        i->set_invalid_cache (false);
    } else {
        pm = (QPixmap*)(i->cache ());
    }

    /* manage opacity on image */
    qreal old_opacity = _painter->opacity ();

    QColor c = _context->brush.color ();
    _painter->setOpacity (c.alphaF ());
    //_painter->setRenderHint (QPainter::SmoothPixmapTransform); // do not need anymore if djnn::Layer is used correctly
    _painter->drawPixmap (rect, *pm);

#if _DEBUG_SEE_GUI_INFO_PREF
    __nb_Drawing_object++;
#endif

    /* reset opacity */
    _painter->setOpacity (old_opacity);

    if (is_in_picking_view (i)) {
        rmt_BeginCPUSample (draw_image_picking, RMTSF_Aggregate);
        load_pick_context (i, _context);
        _picking_view->painter ()->drawRect (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object_picking++;
#endif
        rmt_EndCPUSample ();
    }

    rmt_EndCPUSample ();
}

int
QtBackend::get_pixel (Image* i, double x, double y)
{
    QPixmap* pm = (QPixmap*)(i->cache ());
    if (pm) {
        QImage img = pm->toImage ();
        if (!img.isNull ())
            return img.pixel (x, y);
    }
    return 0;
}

void
QtBackend::draw_data_image (DataImage* i)
{
    rmt_BeginCPUSample (draw_data_image, RMTSF_Aggregate);
    double     x, y, w, h;
    string*    data;
    int        format;
    QtContext* _context = z_processing_step == 2 ? cur_context : _context_manager->get_current ();
    if (z_processing_step == 1) {
        add_shape (i, _context);
        return;
    }

    i->get_properties_values (data, format, x, y, w, h);
    // cerr << data << " " << w << " " << h << __FL__;
    load_drawing_context (i, _context, x, y, w, h);
    QRect rect (x, y, w, h);
    // QPixmap *pm = reinterpret_cast<QPixmap*>(i->cache());
    auto* pm = reinterpret_cast<QImage*> (i->cache ());
    // if(pm == nullptr) {
    //   //pm = new QPixmap (w,h);
    //   pm = new QImage ();
    //   i->set_cache (pm);
    //   i->set_invalid_cache (true);
    // }
    if (i->invalid_cache ()) {
        if (data) {
            delete pm;
            if (format < 0)
                pm->loadFromData (reinterpret_cast<const uchar*> (data->c_str ()), data->length ());
            else
                pm = new QImage (reinterpret_cast<const uchar*> (data->c_str ()), w, h, (QImage::Format)format); // QImage::Format_BGR888);
            i->set_cache (pm);
        }
        // else
        i->set_invalid_cache (false);
    }

    if (pm) {

        /* manage opacity on image */
        qreal  old_opacity = _painter->opacity ();
        QColor c           = _context->pen.color ();
        _painter->setOpacity (c.alphaF ());

        //_painter->setRenderHint (QPainter::SmoothPixmapTransform); // do not need anymore if djnn::Layer is used correctly
        //_painter->drawPixmap (rect, *pm);
        _painter->drawImage (rect, *pm);
#if _DEBUG_SEE_GUI_INFO_PREF
        __nb_Drawing_object++;
#endif

        /* reset opacity */
        _painter->setOpacity (old_opacity);

        if (is_in_picking_view (i)) {
            rmt_BeginCPUSample (draw_data_image_picking, RMTSF_Aggregate);
            load_pick_context (i, _context);
            _picking_view->painter ()->drawRect (rect);
#if _DEBUG_SEE_GUI_INFO_PREF
            __nb_Drawing_object_picking++;
#endif
            rmt_EndCPUSample ();
        }
    }

    rmt_EndCPUSample ();
}

static QPainter* buff_painter;
static QPainter* buff_pick_painter;

struct LayerStuff : LayerCache
{
    // QImage *pm;
    QPixmap*   pm;
    double     tx, ty;
    double     s;
    QMatrix4x4 m;
    LayerStuff ()
        : s (1) {}
    ~LayerStuff ()
    {
        delete pm;
    }
};

struct PickLayerStuff : LayerCache
{
    QImage* pm;
    PickLayerStuff ()
        : pm (nullptr) {}
    ~PickLayerStuff ()
    {
        delete pm;
    }
};

// https://stackoverflow.com/a/1903975/2036022
static int
sign (double x)
{
    return (x > 0) - (x < 0);
}

bool
QtBackend::pre_draw_layer (Layer* l)
{
    rmt_BeginCPUSample (pre_draw_layer, RMTSF_Aggregate);

    // handle z value. FIXME: intention?
    if (z_processing_step == 1)
        z_processing_step = 3;

    // -------------
    // get layer geometry to compute pixmap geometry
    int x, y, w, h, pad;
    l->get_xywhp (x, y, w, h, pad);
    LayerStuff* ls = (LayerStuff*)(l->cache ());
    // auto * pick_pm = (QPixmap*) (l->pick_cache ());
    auto* pick_pm = (PickLayerStuff*)(l->pick_cache ());
    auto  fw      = l->get_frame ()->width ()->get_value ();
    auto  fh      = l->get_frame ()->height ()->get_value ();
    // if the user did not provide width and height, take those of the frame
    if (w < 0) {
        w = fw;
        h = fh;
    }

    // -------------
    // should we recompute the pixmap?
    // yes if the cache is invalid (e.g. because inner shapes have been damaged) or if geometry has changed
    bool recompute_pixmap =
        l->invalid_cache () || (ls && (ls->pm->width () != (w + pad * 2) || ls->pm->height () != (h + pad * 2)));

    if (recompute_pixmap) {
#ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_RECOMPUTE_PIXMAP_AND_PAINTEVENT || _DEBUG_SEE_RECOMPUTE_PIXMAP_ONLY)
            cerr << "\n RECOMPUTE PIXMAP " << l->get_debug_name () << " : " << (w + pad * 2) << " - " << (h + pad * 2) << endl;
#endif
        // -- drop former offscreen pixmaps
        delete ls;
        delete pick_pm;

        // -- prepare new offscreen pixmaps
        ls = new LayerStuff;
        w += pad * 2;
        h += pad * 2;

        double hidpi_scale = 1; // DisplayBackend::instance()->window()->hidpi_scale()->get_value(); // FIXME, what about multi-window?!
        ls->pm             = new QPixmap (w * hidpi_scale, h * hidpi_scale);
        ls->pm->setDevicePixelRatio (hidpi_scale);
        ls->pm->fill (Qt::transparent);
        pick_pm     = new PickLayerStuff;
        pick_pm->pm = new QImage (w, h, QImage::Format_ARGB32_Premultiplied);
        // pick_pm->pm->setDevicePixelRatio (1);
        // pick_pm = new QPixmap (w, h);
        pick_pm->pm->fill (Qt::transparent);
        l->set_cache (ls);
        l->set_pick_cache (pick_pm);
        l->set_invalid_cache (false);

        // -- save current painters (FIXME: in static global values :-/)
        buff_painter      = _painter;
        buff_pick_painter = _picking_view->painter ();

        // -- create new painters
        _painter = new QPainter (ls->pm);
        _picking_view->set_painter (new QPainter (pick_pm->pm));

        _in_cache = true;

        // -- find out the current scale, rotation and translation
        QtContext*  cur_context = _context_manager->get_current ();
        QMatrix4x4& origin      = cur_context->matrix;
        ls->m                   = origin;
        // https://math.stackexchange.com/a/13165
        // ---- find current scaling
        auto a = origin (0, 0);
        auto b = origin (0, 1);
        auto s = sign (a) * sqrt (a * a + b * b);
        // ---- find current translation
        auto tx = origin (0, 3);
        auto ty = origin (1, 3);
        // // ---- find current rotation
        // auto r             = atan2 (-b, a);
        // cerr << tx << " " << ty << " " << s <<  " "  << r << __FL__;

        // -- adjust the pixmap
        // if translation is positive, apply -translation to avoid pixmap emptiness at the top left // FIXME resize image!
        // if translation is negative, do not try to adjust translation so that the bottom-right part of the rendering is not clipped
        QMatrix4x4 newm;
        if (tx > 0) {
            newm.translate (-tx, 0); // apply -translation
        }
        if (ty > 0) {
            auto inv_tvy = (h - ls->pm->height ()) + ty; // take into account reverse y
            newm.translate (0, -inv_tvy);                // apply translation
        }

        // -- apply layer x and y
        newm.translate (-x, -y);

        // -- apply padding to get the surrounding
        newm.translate (pad, pad);

        // -- update the cur_context->matrix (origin is a &ref on it)
        origin = newm * origin;

        // -- store computed scale, rotation (?), and scale
        ls->tx = tx;
        ls->ty = ty;
        ls->s  = s;

    } else {
        // FIXME: this seems to be both a save of painters and a trick to inform post_draw_layer that the layer has not been recomputed
        buff_painter      = nullptr;
        buff_pick_painter = nullptr;
    }

    rmt_EndCPUSample ();

    return recompute_pixmap;
}

void
QtBackend::post_draw_layer (Layer* l)
{
    rmt_BeginCPUSample (post_draw_layer, RMTSF_Aggregate);

    LayerStuff* ls      = (LayerStuff*)(l->cache ());
    auto*       pick_pm = (PickLayerStuff*)(l->pick_cache ());

    QtContext*  cur_context = _context_manager->get_current ();
    QMatrix4x4& origin      = cur_context->matrix;
    QMatrix4x4  m           = origin;

    // if layer has been recomputed, restore the painters(?)
    if (buff_painter != nullptr) {
        delete _painter;
        _painter     = buff_painter;
        buff_painter = nullptr;

        delete _picking_view->painter ();
        _picking_view->set_painter (buff_pick_painter);
        buff_pick_painter = nullptr;

        _in_cache = false;
        // damaged = true; // never used
        m = ls->m;
    }

    // find out the current scale, rotation and translation
    // https://math.stackexchange.com/a/13165
    // note : old method use before introducing rotation
    // {
    //     auto a = m (0, 0);
    //     auto b = m (0, 1);
    //     auto curs = sign(a) * sqrt(a*a + b*b);
    //     //auto curs = m (0, 0);
    //     auto tx   = m (0, 3);
    //     auto ty   = m (1, 3);
    //     auto s    = curs / ls->s;
    //     // find current rotation
    //     auto r    = atan2 (-b, a)
    //     cerr << endl << "1 - " << tx << " " << ty << " " << s << "    " << r << "    prvious " << qRadiansToDegrees(r) <<endl; //__FL__;
    // }

    // https://math.stackexchange.com/a/43140
    auto tx = m (0, 3);
    auto ty = m (1, 3);
    auto a  = m (0, 0);
    // auto b = m (1, 0);
    auto c = m (0, 1);
    // auto d = m (1, 1);

    auto scaleX = sqrt ((a * a) + (c * c));
    // auto scaleY = sqrt((b * b) + (d * d));
    auto s = scaleX;

    auto sign = atan (-c / a);
    auto rad  = acos (a / scaleX);
    auto deg  = qRadiansToDegrees (rad);

    auto r = 0.;

    // Calculation of the correct angle to avoid sign inversions with cos and sin
    if (deg > 90 && sign > 0)
        r = (360 - deg);
    else if (deg < 90 && sign < 0)
        r = (360 - deg);
    else
        r = deg;

    // debug
    // cerr << "2 - " << tx << " " << ty << " " << s << "    " << rad << "    prvious " << r <<endl; //__FL__;

    // get layer geometry
    int x, y, w, h, pad;
    l->get_xywhp (x, y, w, h, pad);

    // compute the layer transform
    QMatrix4x4 newm;
    newm.translate (tx, ty);  // Reverse the translation to origin
    newm.rotate (r, 0, 0, 1); // r in degree

    // debug
    //  cerr << "newm: AFTER ROT" << endl;
    //  cerr << newm (0, 0) << "  " << newm (0, 1) << "  " << newm (0, 2) << "  " << newm (0, 3) << endl;
    //  cerr << newm (1, 0) << "  " << newm (1, 1) << "  " << newm (1, 2) << "  " << newm (1, 3) << endl;
    //  cerr << newm (2, 0) << "  " << newm (2, 1) << "  " << newm (2, 2) << "  " << newm (2, 3) << endl;
    //  cerr << newm (3, 0) << "  " << newm (3, 1) << "  " << newm (3, 2) << "  " << newm (3, 3) << endl;

    newm.translate (x, y);

    if (ls->tx < 0) {
        newm.translate ((-ls->tx) * s, 0); // apply no-emptiness translation
    }
    if (ls->ty < 0) {
        newm.translate (0, (-ls->ty) * s); // apply no-emptiness translation
    }
    // // newm = glm::translate(newm, gl2d::xxx_vertex_t{x, y});

    newm.translate (-pad * s, -pad * s);

    newm.scale (s, s);

    // update current transform in context
    origin = newm;
    // newm = glm::translate(newm, gl2d::xxx_vertex_t{-t.x, -t.y});
    // m = newm * m;

    // debug
    // cerr << "origin: " << endl;
    // cerr << origin (0, 0) << "  " << origin (0, 1) << "  " << origin (0, 2) << "  " << origin (0, 3) << endl;
    // cerr << origin (1, 0) << "  " << origin (1, 1) << "  " << origin (1, 2) << "  " << origin (1, 3) << endl;
    // cerr << origin (2, 0) << "  " << origin (2, 1) << "  " << origin (2, 2) << "  " << origin (2, 3) << endl;
    // cerr << origin (3, 0) << "  " << origin (3, 1) << "  " << origin (3, 2) << "  " << origin (3, 3) << endl;

    // align translation to the grid FIXME useless??? we do not use m anymore...
    m (2, 0) = round (m (2, 0));
    m (2, 1) = round (m (2, 1));

    // update painter transform
    const QTransform transform = origin.toTransform ();
    _painter->setTransform (transform);
    _picking_view->painter ()->setTransform (transform);

    // draw pixmaps
    QRect rect (0, 0, ls->pm->width (), ls->pm->height ());

    auto rh = _painter->renderHints ();
    _painter->setRenderHints ({});
    rmt_BeginCPUSample (post_draw_layer_pixmap, RMTSF_Aggregate);
    //_painter->drawImage (rect, *(ls->pm));
    _painter->drawPixmap (0, 0, *(ls->pm));
    rmt_EndCPUSample ();
    _painter->setRenderHints (rh);

    _picking_view->painter ()->setCompositionMode (QPainter::CompositionMode_SourceOver);
    rmt_BeginCPUSample (post_draw_layer_pixmap_pick, RMTSF_Aggregate);
    _picking_view->painter ()->drawImage (rect, *pick_pm->pm);
    //_picking_view->painter()->drawPixmap (0, 0, *pick_pm);
    rmt_EndCPUSample ();
    _picking_view->painter ()->setCompositionMode (QPainter::CompositionMode_Source);
    if (z_processing_step == 3)
        z_processing_step = 1;
    rmt_EndCPUSample ();
}
} /* namespace djnn */
