#pragma once

#include "core/property/double_property.h"
#include "gen/abstract_prop_rectangle.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class Rectangle : public AbstractPropRectangle
{
  public:
    Rectangle (CoreProcess* parent, const string& name, double x, double y, double width, double height, double rx = 0, double ry = 0);
    Rectangle* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void       draw () override;
    void       get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double     sdf (double x, double y) const override;
};

} // namespace djnn
