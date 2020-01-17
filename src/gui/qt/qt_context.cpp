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

#include "gui/backend.h"

#include "qt_backend.h"
#include "display/qt/qt_window.h"

#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtCore/QFileInfo>
#include <iostream>
#include <cmath>

namespace djnn
{
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
                                                                                            Qt::SolidPattern), matrix (), gradientTransform (), font ()
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
    gradientTransform = QTransform (gradientTransform);
    font = QFont (p->font);
    alpha = p->alpha;
    fillRule = p->fillRule;
    textAnchor = p->textAnchor;
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

} /* namespace djnn */
