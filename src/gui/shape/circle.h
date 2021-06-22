#pragma once

#include "core/tree/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_prop_circle.h"

namespace djnn
{
  class Circle : public AbstractPropCircle
  {
  public:
    Circle (ParentProcess* parent, const string& name, double cx, double cy, double r);
    Circle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

} // namespace djnn
