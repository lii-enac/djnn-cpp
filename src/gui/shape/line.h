#pragma once

#include "core/property/double_property.h"
#include "gen/abstract_prop_line.h"
#include "gui/shape/abstract_gshape.h"

namespace djnn {
class Line : public AbstractPropLine
{
  public:
    Line (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2);
    Line*  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void   draw () override;
    void   get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
};
} // namespace djnn
