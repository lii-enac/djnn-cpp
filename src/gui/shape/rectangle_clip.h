#pragma once

#include "core/tree/double_property.h"
#include "gui/shape/abstract_gshape.h"
#include "gen/abstract_prop_rectangle_clip.h"

namespace djnn
{
  class RectangleClip : public AbstractPropRectangleClip
  {
  public:
    RectangleClip (ParentProcess* parent, const djnn::string& name, double x, double y, double width, double height);
    RectangleClip* clone () override;
    void draw () override;
    void get_bounding_box (double& x, double& y, double& w, double& h) const override;
    double sdf (double x, double y) const override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
  };
}