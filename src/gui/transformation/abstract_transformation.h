#pragma once

#include "gui/abstract_gobj.h"

namespace djnn {
class AbstractTransformation : public AbstractGObj
{
  public:
    AbstractTransformation (CoreProcess* parent, const string& name);
    AbstractTransformation ();
    virtual ~AbstractTransformation ();
    virtual void    transform (double& x, double& y) {}
    virtual void    inverse_transform (double& x, double& y) {}
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
};
} // namespace djnn