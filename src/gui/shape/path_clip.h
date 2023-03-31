#pragma once

#include "gui/shape/path.h"

namespace djnn {
  class PathClip : public Path
  {
  public:
    PathClip (CoreProcess* parent, const string& name) :
        Path (parent, name) {}
    virtual ~PathClip () {}
    void draw () override;
    //PathClip* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    PathClip* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  };
}