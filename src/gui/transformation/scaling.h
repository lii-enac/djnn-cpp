#pragma once

#include "abstract_transformation.h"
#include "gen/abstract_scaling.h"

namespace djnn {
  class Scaling : public AbstractScaling
  {
  public:
    Scaling (ParentProcess* parent, const std::string& name, double sx, double sy, double cx, double cy);
    Scaling (double sx, double sy, double cx, double cy);
    virtual ~Scaling ();
    void draw () override;
    virtual void transform( double& x, double& y) override;
    virtual void inverse_transform( double& x, double& y) override;
    FatProcess* clone () override;
  };
}