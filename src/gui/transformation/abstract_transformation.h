#pragma once

#include "gui/abstract_gobj.h"

namespace djnn
{
  class AbstractTransformation : public AbstractGObj
  {
  public:
    AbstractTransformation (ParentProcess* parent, const djnn::string& name);
    AbstractTransformation ();
    virtual ~AbstractTransformation ();
    virtual void transform( double& x, double& y) {}
    virtual void inverse_transform( double& x, double& y) {}
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
  };
}