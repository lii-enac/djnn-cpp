#pragma once

#include "core/property/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_prop_ellipse.h"

namespace djnn
{
  class Ellipse : public AbstractPropEllipse
  {
  public:
    Ellipse (ParentProcess* parent, const string& name, double cx, double cy, double rx, double ry);
    Ellipse* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

} // namespace djnn
