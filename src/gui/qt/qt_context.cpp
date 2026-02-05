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

#include <QtCore/QFileInfo>
#include <QtCore/QtMath>
#include <QtWidgets/QWidget>

#include "qt_context.h"

#include "qt_backend.h"
#if DJNN_STL_DJNN
#include "core/utils/containers_impl.h"
#endif

#include <cmath>

#include "core/utils/iostream.h"

using namespace djnnstl;

namespace djnn {
vector<QtVectorShapeToDraw*> shapes_vectors;
int                          z_processing_step = 0;
QtContext*                   cur_context       = nullptr;

QtContext*
QtContextManager::get_current ()
{
    return _context_list.back ();
}

void
QtContextManager::push ()
{
    QtBackend::instance ()->save_context ();
    if (_context_list.empty ())
        _context_list.push_back (new QtContext ());
    else {
        _context_list.push_back (new QtContext (_context_list.back ()));
    }
}

void
QtContextManager::pop ()
{
    delete _context_list.back ();
    _context_list.pop_back ();
    QtBackend::instance ()->restore_context ();
}

QtContext::QtContext ()
    : DEFAULT_DPI_RES (96), alpha (1), fillRule (Qt::OddEvenFill),
      pen (QBrush(QColor("darkslategray")), 1, Qt::NoPen), 
      brush (QColor ("black"), Qt::SolidPattern),
      fontMetrics (nullptr), _z_order (0), _pick_bitset (0b0101)
{
    textAnchor = DJN_START_ANCHOR;
    for (int i = 0; i < 10; i++)
        factor[i] = 1.;
    factor[DJN_IN] = DEFAULT_DPI_RES;
    factor[DJN_CM] = DEFAULT_DPI_RES * 2.54;
    factor[DJN_MM] = DEFAULT_DPI_RES * 25.4;
    factor[DJN_PT] = DEFAULT_DPI_RES / 72;
    factor[DJN_PC] = (DEFAULT_DPI_RES / 72) * 12;
}

QtContext::QtContext (const QtContext& p) : fontMetrics (nullptr)
{
    *this       = p;
}

QtContext::QtContext (QtContext* p) : fontMetrics(nullptr)
{
    if (p) {
        pen               = QPen (p->pen);
        brush             = QBrush (p->brush);
        matrix            = QMatrix4x4 (p->matrix);
        gradientTransform = QTransform (p->gradientTransform);
        clipTransform     = QTransform (p->clipTransform);
        font              = QFont (p->font);
        clip              = QPainterPath (p->clip);
        alpha             = p->alpha;
        fillRule          = p->fillRule;
        textAnchor        = p->textAnchor;
        _z_order          = p->_z_order;
        _pick_bitset      = p->_pick_bitset;
        DEFAULT_DPI_RES   = p->DEFAULT_DPI_RES;
        for (int i = 0; i < 10; i++)
            factor[i] = p->factor[i];
        
        if (p->fontMetrics) {
            fontMetrics = new QFontMetrics (*p->fontMetrics);
        }
    }
}

QtContext& QtContext::operator= (const QtContext& p) {
    if (this == &p) 
        return *this;

    delete fontMetrics;

    DEFAULT_DPI_RES   = p.DEFAULT_DPI_RES;
    alpha             = p.alpha;
    fillRule          = p.fillRule;
    pen               = p.pen;
    brush             = p.brush;
    matrix            = p.matrix;
    gradientTransform = p.gradientTransform;
    clipTransform     = p.clipTransform;
    font              = p.font;
    clip              = p.clip;
    textAnchor        = p.textAnchor;
    _z_order          = p._z_order;
    _pick_bitset      = p._pick_bitset;
    for (int i = 0; i < 10; i++) factor[i] = p.factor[i];

    if (p.fontMetrics) {
        fontMetrics = new QFontMetrics (*p.fontMetrics);
    }

    return *this;
}

QtContext::~QtContext () {
    delete fontMetrics;
}

void
QtContext::update_relative_units ()
{
    QFontInfo fi (font);
    factor[DJN_EM] = fi.pixelSize ();
    factor[DJN_EX] = fi.pixelSize () / 2.; /* rough approximation */
}

double
QtContext::get_unit_factor (djnLengthUnit unit)
{
    return factor[unit];
}

QtVectorShapeToDraw::~QtVectorShapeToDraw ()
{
    // for (auto p : _shapes) {
    // delete p.second;
    //}
}

void
QtVectorShapeToDraw::add_item (AbstractGShape* shape, QtContext* context)
{
    _shapes.push_back (pair (shape, QtContext (context)));
}

void
QtVectorShapeToDraw::draw ()
{
    for (auto& p : _shapes) {
        p.first->draw ();
    }
}
void
add_shape (AbstractGShape* shape, QtContext* context)
{
    int z = context->z_order ();
    if (shapes_vectors.empty () || shapes_vectors.back ()->z_order () < z) {
        QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
        v->add_item (shape, context);
        shapes_vectors.push_back (v);
        return;
    }
    if (shapes_vectors.at (0)->z_order () > z) {
        QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
        v->add_item (shape, context);
        vector<QtVectorShapeToDraw*>::iterator it = shapes_vectors.begin ();
        shapes_vectors.insert (it, v);
        return;
    }
    vector<QtVectorShapeToDraw*>::iterator it = shapes_vectors.begin ();
    while (it != shapes_vectors.end ()) {
        if ((*it)->z_order () == z) {
            (*it)->add_item (shape, context);
            return;
        }
        if ((*it)->z_order () < z && (*(it + 1))->z_order () > z) {
            QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
            v->add_item (shape, context);
            shapes_vectors.insert (it + 1, v);
            return;
        }
        it++;
    }
}

void
clear_shapes ()
{
    for (auto v : shapes_vectors) {
        delete v;
    }
}

} /* namespace djnn */

#if DJNN_STL_DJNN
namespace djnnstl {
template class vector<pair<djnn::AbstractGShape*, djnn::QtContext>>;
}
#endif
