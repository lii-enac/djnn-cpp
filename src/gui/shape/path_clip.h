#pragma once

#include "gui/shape/path.h"

namespace djnn {
  class PathClip : public Path
  {
  public:
    PathClip (ParentProcess* parent, const djnn::string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    PathClip* clone () override;
  };
}