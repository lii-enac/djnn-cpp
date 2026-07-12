/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Magnaudet Mathieu <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "picking.h"

namespace djnn {

class AnalyticalPicking : public Picking {
  public:
    AnalyticalPicking (Window* win);
    virtual ~AnalyticalPicking ();
    virtual void            init ();
    virtual AbstractGShape* pick (double x, double y);
    virtual void            add_pick_shape (PickUI* pshape, bool cache = false);
};

} // namespace djnn
