#pragma once

#include "core/property/double_property.h"
#include "gen/abstract_prop_circle.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class Circle : public AbstractPropCircle
{
  public:
    Circle (CoreProcess* parent, const string& name, double cx, double cy, double r);
    Circle* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void    draw () override;
    void    get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double  sdf (double x, double y) const override;
};

} // namespace djnn
