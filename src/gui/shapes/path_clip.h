#pragma once

#include "gui/shapes/path.h"

namespace djnn {
  class PathClip : public Path
  {
  public:
    PathClip (ParentProcess* parent, const std::string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    PathClip* clone () override;
  };
}