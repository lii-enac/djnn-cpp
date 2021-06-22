#pragma once

#include "core/tree/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_prop_rectangle.h"

namespace djnn
{
  class Rectangle : public AbstractPropRectangle
  {
  public:
    Rectangle (ParentProcess* parent, const string& name, double x, double y, double width, double height, double rx=0, double ry=0);
    Rectangle* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
  };

} // namespace djnn
