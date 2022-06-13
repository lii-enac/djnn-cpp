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

#include "qt_context.h"
#include "qt_backend.h"

#include <QtWidgets/QWidget>
#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include "core/utils/iostream.h"
#include <cmath>

namespace djnn
{
  vector<QtVectorShapeToDraw*> shapes_vectors;
  int z_processing_step = 0;
  QtContext *cur_context = nullptr;

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
    QtBackend::instance ()->restore_context();
  }

  QtContext::QtContext () :
      pen (QColor ("darkslategray"), 1, Qt::NoPen, Qt::FlatCap, Qt::MiterJoin), brush (QColor ("black"),
                                                                                            Qt::SolidPattern), matrix (), gradientTransform (), clipTransform(), font (), _pick_bitset (0b0101)
  {
    alpha = 1;
    fillRule = Qt::OddEvenFill;
    textAnchor = DJN_START_ANCHOR;
    DEFAULT_DPI_RES = 96;
    for (int i = 0; i < 10; i++)
      factor[i] = 1.;
    factor[DJN_IN] = DEFAULT_DPI_RES; /* pixels by inch */
    factor[DJN_CM] = DEFAULT_DPI_RES * 2.54; /* pixels by cm */
    factor[DJN_MM] = DEFAULT_DPI_RES * 25.4; /* pixels by mm */
    factor[DJN_PT] = DEFAULT_DPI_RES / 72; /* pixels by point (given that 1pt = 1/72 inch) */
    factor[DJN_PC] = (DEFAULT_DPI_RES / 72) * 12; /* pixels by pica (given that 1pc = 12pt) */
    //update_relative_units (); Fail at startup
  }

  QtContext::QtContext (QtContext *p)
  {
    pen = QPen (p->pen);
    brush = QBrush (p->brush);
    matrix = QMatrix4x4 (p->matrix);
    gradientTransform = QTransform (p->gradientTransform);
    clipTransform = QTransform (p->clipTransform);
    font = QFont (p->font);
    clip = QPainterPath (p->clip);
    alpha = p->alpha;
    fillRule = p->fillRule;
    textAnchor = p->textAnchor;
    _pick_bitset = p->_pick_bitset;
    DEFAULT_DPI_RES = 96;
    for (int i = 0; i < 10; i++)
      factor[i] = p->factor[i];
  }

  QtContext::~QtContext ()
  {
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

  QtVectorShapeToDraw::~QtVectorShapeToDraw () {
    //for (auto p : _shapes) {
      //delete p.second;
    //}
  }

  void
  QtVectorShapeToDraw::add_item (AbstractGShape* shape, QtContext *context) {
    _shapes.push_back (make_pair(shape, QtContext (context)));
  }

  void
  QtVectorShapeToDraw::draw () {
    for (auto p : _shapes) {
      p.first->draw();
    }
  }
  void add_shape (AbstractGShape* shape, QtContext *context)
  {
    int z = shape->z_order();
    if (shapes_vectors.empty () || shapes_vectors.back()->z_order() < z) {
      QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
      v->add_item(shape, context);
      shapes_vectors.push_back(v);
      return;
    }
    if (shapes_vectors.at(0)->z_order() > z) {
      QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
      v->add_item(shape, context);
      vector<QtVectorShapeToDraw*>::iterator it = shapes_vectors.begin ();
      shapes_vectors.insert(it, v);
      return;
    }
    vector<QtVectorShapeToDraw*>::iterator it = shapes_vectors.begin ();
    while (it != shapes_vectors.end ()) {
      if ((*it)->z_order () == z) {
        (*it)->add_item(shape, context);
        return;
      }
      if ((*it)->z_order () < z && (*(it+1))->z_order () > z) {
        QtVectorShapeToDraw* v = new QtVectorShapeToDraw (z);
        v->add_item(shape, context);
        shapes_vectors.insert(it+1, v);
        return;
      }
      it++;
    }
  }

  void clear_shapes ()
  {
    for (auto v : shapes_vectors) {
      delete v;
    }
  }
} /* namespace djnn */
