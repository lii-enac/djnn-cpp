#pragma once

#include "core/tree/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_prop_line.h"

namespace djnn
{
  class Line : public AbstractPropLine
  {
  public:
    Line (ParentProcess* parent, const string& name, double x1, double y1, double x2, double y2);
    Line* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };
}
