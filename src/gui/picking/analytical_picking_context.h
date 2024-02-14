#pragma once

namespace djnn {
class PickAnalyticalContext
{
  public:
    double x;
    double y;
    double half_outline_width;
    bool   filled;
    bool   outlined;
    bool   clipped;
};
} // namespace djnn
