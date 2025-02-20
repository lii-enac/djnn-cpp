#pragma once

#include "core/property/double_property.h"
#include "gen/abstract_prop_ellipse.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class Ellipse : public AbstractPropEllipse {
  public:
    Ellipse (CoreProcess* parent, const string& name, double cx, double cy, double rx, double ry);
    Ellipse* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
    void     draw () override;
    void     get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double   sdf (double x, double y) const override;
};

} // namespace djnn
