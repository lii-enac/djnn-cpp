/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "gui/abstract_gobj.h"

namespace djnn {
class AbstractTransformation : public AbstractGObj {
  public:
    AbstractTransformation (CoreProcess* parent, const string& name);
    AbstractTransformation ();
    virtual ~AbstractTransformation ();
    virtual void    transform (double& x, double& y) {}
    virtual void    inverse_transform (double& x, double& y) {}
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
};
} // namespace djnn