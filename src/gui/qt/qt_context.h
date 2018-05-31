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

#if QT

//#include "qt_backend.h"
#include "../style/style_types.h"
#include "../../core/execution/component_observer.h"
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QMatrix4x4>
#include <QtGui/QTransform>
#include <QtGui/QFont>

namespace djnn
{
  class QtContext
  {
    friend class QtBackend;
  public:
    QtContext ();
    QtContext (shared_ptr<QtContext> parent);
    virtual ~QtContext ();
  private:
    int DEFAULT_DPI_RES;
    double alpha;
    Qt::FillRule fillRule;
    QPen pen;
    QBrush brush;
    QMatrix4x4 matrix;
    QTransform gradientTransform;
    QFont font;
    double factor[10];
    int textAnchor;
    void
    update_relative_units ();
    double
    get_unit_factor (djnLengthUnit unit);
  };

  class QtContextManager : public ContextManager
  {
  public:
    QtContextManager () :
      ContextManager ()
    {
      ComponentObserver::instance ().add_draw_context_manager (this);
    }
    ;
    virtual
    ~QtContextManager ()
    {
    }
    ;
    void pop () override;
    void push () override;
    shared_ptr<QtContext> get_current ();

  private:
    vector<shared_ptr<QtContext>> _context_list;
  };

} /* namespace djnn */

#endif
