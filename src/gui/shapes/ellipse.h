#pragma once

#include "core/tree/double_property.h"
#include "gen/abstract_prop_ellipse.h"

namespace djnn
{
  class Ellipse : public AbstractPropEllipse
  {
  public:
    Ellipse (ParentProcess* parent, const std::string& name, double cx, double cy, double rx, double ry);
    Ellipse* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

} // namespace djnn
