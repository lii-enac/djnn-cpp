#pragma once

#include "gui/shape/path.h"

namespace djnn {
  class PathClip : public Path
  {
  public:
    PathClip (ParentProcess* parent, const string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    //PathClip* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
    PathClip* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}