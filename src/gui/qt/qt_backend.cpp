/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2021)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include <QtCore/QtMath>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
#include <cmath>

#include "qt_backend.h"

#include "core/utils/iostream.h"
#include "core/utils/remotery.h"
#include "display/qt/qt_window.h"
#include "display/ui.h"
#include "gui/backend.h"
#include "gui/shape/image.h"
#include "gui/shape/text.h"
#include "gui/transformation/homography.h"
#include "qt_context.h"

namespace djnn {
QtBackend*     QtBackend::_instance;
std::once_flag QtBackend::onceFlag;

QtBackend*
QtBackend::instance ()
{
    std::call_once (QtBackend::onceFlag, [] () {
        _instance = new QtBackend ();
    });

    return _instance;
}

QtBackend::QtBackend ()
    : _painter (nullptr), _picking_view (nullptr), _in_cache (false)
{
    _context_manager = new QtContextManager ();
}

QtBackend::~QtBackend ()
{
    delete _context_manager;
}

void
QtBackend::save_context ()
{
    rmt_BeginCPUSample (save_context, RMTSF_Aggregate);
    if (_painter != nullptr)
        _painter->save ();
    if (_picking_view && _picking_view->painter ()) {
        _picking_view->painter ()->save ();
    }
    rmt_EndCPUSample ();
}

void
QtBackend::restore_context ()
{
    rmt_BeginCPUSample (restore_context, RMTSF_Aggregate);
    if (_painter != nullptr)
        _painter->restore ();
    if (_picking_view && _picking_view->painter ()) {
        _picking_view->painter ()->restore ();
    }
    rmt_EndCPUSample ();
}
void
QtBackend::set_painter (QPainter* p)
{
    _painter = p;
}

void
QtBackend::set_picking_view (QtPickingView* p)
{
    _picking_view = p;
}

// void
// QtBackend::set_picking_view (Picking* p)
// {
//   _picking_view = p;
// }

/* Qt context management is imported from djnn v1 */
void
QtBackend::load_drawing_context (AbstractGShape* s, QtContext* cur_context, double tx, double ty, double width, double height)
{
    rmt_BeginCPUSample (load_drawing_context, RMTSF_Aggregate);

    const QMatrix4x4& matrix    = cur_context->matrix;
    const QTransform  transform = matrix.toTransform ();
    if (s->matrix () != nullptr) {
        Homography*       h          = dynamic_cast<Homography*> (s->matrix ());
        const QMatrix4x4& loc_matrix = QMatrix4x4 (matrix);
        // we don't want any propagation so we can write directly in raw_props
        h->raw_props.m11 = loc_matrix (0, 0);
        h->raw_props.m12 = loc_matrix (0, 1);
        h->raw_props.m13 = loc_matrix (0, 2);
        h->raw_props.m14 = loc_matrix (0, 3);

        h->raw_props.m21 = loc_matrix (1, 0);
        h->raw_props.m22 = loc_matrix (1, 1);
        h->raw_props.m23 = loc_matrix (1, 2);
        h->raw_props.m24 = loc_matrix (1, 3);

        h->raw_props.m31 = loc_matrix (2, 0);
        h->raw_props.m32 = loc_matrix (2, 1);
        h->raw_props.m33 = loc_matrix (2, 2);
        h->raw_props.m34 = loc_matrix (2, 3);

        h->raw_props.m41 = loc_matrix (3, 0);
        h->raw_props.m42 = loc_matrix (3, 1);
        h->raw_props.m43 = loc_matrix (3, 2);
        h->raw_props.m44 = loc_matrix (3, 3);
    }
    if (s->inverted_matrix () != nullptr) {
        Homography*      hi             = dynamic_cast<Homography*> (s->inverted_matrix ());
        const QMatrix4x4 loc_matrix_inv = QMatrix4x4 (matrix).inverted ();
        // we don't want any propagation so we can write directly in raw_props
        hi->raw_props.m11 = loc_matrix_inv (0, 0);
        hi->raw_props.m12 = loc_matrix_inv (0, 1);
        hi->raw_props.m13 = loc_matrix_inv (0, 2);
        hi->raw_props.m14 = loc_matrix_inv (0, 3);

        hi->raw_props.m21 = loc_matrix_inv (1, 0);
        hi->raw_props.m22 = loc_matrix_inv (1, 1);
        hi->raw_props.m23 = loc_matrix_inv (1, 2);
        hi->raw_props.m24 = loc_matrix_inv (1, 3);

        hi->raw_props.m31 = loc_matrix_inv (2, 0);
        hi->raw_props.m32 = loc_matrix_inv (2, 1);
        hi->raw_props.m33 = loc_matrix_inv (2, 2);
        hi->raw_props.m34 = loc_matrix_inv (2, 3);

        hi->raw_props.m41 = loc_matrix_inv (3, 0);
        hi->raw_props.m42 = loc_matrix_inv (3, 1);
        hi->raw_props.m43 = loc_matrix_inv (3, 2);
        hi->raw_props.m44 = loc_matrix_inv (3, 3);
    }

    /* setup the painting environment of the drawing view */
    QPen tmpPen (cur_context->pen);
    /*
     * with Qt, dash pattern and dash offset are specified
     * in stroke-width unit so we need to update the values accordingly
     */
    if (cur_context->pen.style () == Qt::CustomDashLine) {
        qreal          w           = cur_context->pen.widthF () || 1;
        QVector<qreal> dashPattern = cur_context->pen.dashPattern ();
        QVector<qreal> tmp (dashPattern.size ());
        for (int i = 0; i < dashPattern.size (); i++)
            tmp[i] = dashPattern.value (i) / w;
        tmpPen.setDashPattern (tmp);
        tmpPen.setDashOffset (cur_context->pen.dashOffset () / w);
    }
    if (z_processing_step == 2) {
        if (!cur_context->clip.isEmpty ()) {
            _painter->setTransform (cur_context->clipTransform);
            _painter->setClipPath (cur_context->clip);
        } else {
            _painter->setClipping (false);
        }
    }

    _painter->setRenderHint (QPainter::Antialiasing, true);
    _painter->setPen (tmpPen);
    _painter->setTransform (transform);

    /* If the brush style is gradient and the coordinate mode is ObjectBoundingMode
     * then translate the rotation axis of the gradient transform and transpose
     * the translation values in the global coordinate system*/
    if (cur_context->brush.style () == Qt::LinearGradientPattern || cur_context->brush.style () == Qt::RadialGradientPattern) {
        if (cur_context->brush.gradient ()->coordinateMode () == QGradient::ObjectBoundingMode) {
            QTransform origin = cur_context->gradientTransform;
            QTransform newT   = QTransform ();
            newT.translate (tx, ty);
            QTransform result = origin * newT;
            result.translate (-tx, -ty);
            double m11 = result.m11 ();
            double m12 = result.m12 ();
            double m13 = result.m13 ();
            double m21 = result.m21 ();
            double m22 = result.m22 ();
            double m23 = result.m23 ();
            double m31 = result.m31 () * width;
            double m32 = result.m32 () * height;
            double m33 = result.m33 ();
            result.setMatrix (m11, m12, m13, m21, m22, m23, m31, m32, m33);
            cur_context->gradientTransform = result;
        }
    }
    cur_context->brush.setTransform (cur_context->gradientTransform);
    _painter->setBrush (cur_context->brush);

    rmt_EndCPUSample ();
}

void
QtBackend::load_pick_context (AbstractGShape* s, QtContext* cur_context)
{
    rmt_BeginCPUSample (load_pick_context, RMTSF_Aggregate);

    QBrush pickBrush (_picking_view->pick_color ());
    QPen   pickPen;
    if ((cur_context->pen.style () == Qt::NoPen && cur_context->pick_outline_undefined () == true) || (cur_context->pick_outline_undefined () == false && cur_context->pick_outline () == false)) {
        pickPen.setStyle (Qt::NoPen);
    } else {
        pickPen.setStyle (Qt::SolidLine);
    }
    pickPen.setColor (_picking_view->pick_color ());
    pickPen.setWidth (cur_context->pen.width ());
    if ((cur_context->brush.style () == Qt::NoBrush && cur_context->pick_fill_undefined () == true) || (cur_context->pick_fill_undefined () == false && cur_context->pick_fill () == false)) {
        pickBrush.setStyle (Qt::NoBrush);
    }
    /* no antialiasing in Color Picking otherwise it will modified a given color */
    //_picking_view->painter ()->setCompositionMode(QPainter::CompositionMode_Source);
    _picking_view->painter ()->setPen (pickPen);
    _picking_view->painter ()->setBrush (pickBrush);
    _picking_view->painter ()->setTransform (cur_context->matrix.toTransform ());
    PickUI* p = dynamic_cast<PickUI*> (s);
    _picking_view->add_pick_shape (p, _in_cache);
    rmt_EndCPUSample ();
}

/*
WinImpl*
QtBackend::create_window (Window *win, const string& title, double x, double y, double w, double h)
{
  return new QtWindow (win, title, x, y, w, h);
}
*/

bool
QtBackend::is_in_picking_view (AbstractGShape* s)
{
    return is_pickable (s);
    /*return s->press ()->has_coupling () || s->x ()->has_coupling () || s->y ()->has_coupling ()
        || s->move ()->has_coupling () || s->release ()->has_coupling () || s->enter ()->has_coupling ()
        || s->leave ()->has_coupling ();*/
}

static QFont::Style fontStyleArray[3] =
    {QFont::StyleNormal, QFont::StyleItalic, QFont::StyleOblique};

void
QtBackend::update_text_geometry (Text* text, FontFamily* ff, FontSize* fsz, FontStyle* fs, FontWeight* fw)
{
    rmt_BeginCPUSample (update_text_geometry, RMTSF_Aggregate);

    int           width, height, ascent, descent;
    QFontMetrics* fm = (QFontMetrics*)(text->get_font_metrics ());
    if (fm) {
        QString str (text->get_raw_text ().c_str ());
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
        width = fm->width (str);
#else
        width = fm->horizontalAdvance (str);
#endif
        height  = fm->height ();
        ascent  = fm->ascent ();
        descent = fm->descent ();
    } else {
        QFont qfont;
        if (ff) {
            QString val (ff->family ()->get_value ().c_str ());
            qfont.setFamily (val);
        }
        if (fsz) {
            qfont.setPixelSize (fsz->size ()->get_value ());
        }
        if (fs) {
            int i = fs->style ()->get_value ();
            if (i >= 0 && i <= 3)
                qfont.setStyle (fontStyleArray[i]);
        }
        if (fw) {
            qfont.setWeight (fw->weight ()->get_value ());
        }
        QString      str (text->text ()->get_value ().c_str ());
        QFontMetrics fm (qfont);
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
        width = fm.width (str);
#else
        width = fm.horizontalAdvance (str);
#endif
        height  = fm.height ();
        ascent  = fm.ascent ();
        descent = fm.descent ();
    }
    text->set_width (width);
    text->set_height (height);
    text->set_ascent (ascent);
    text->set_descent (descent);

    rmt_EndCPUSample ();
}

void
QtBackend::delete_text_impl (Text* text)
{
    rmt_BeginCPUSample (delete_text_impl, RMTSF_Aggregate);

    delete (QFontMetrics*)text->get_font_metrics ();

    rmt_EndCPUSample ();
}

void
QtBackend::delete_image_impl (Image* image)
{
    rmt_BeginCPUSample (delete_image_impl, RMTSF_Aggregate);

    delete (QPixmap*)image->cache ();

    rmt_EndCPUSample ();
}
} /* namespace djnn */
