#pragma once

#include "abstract_transformation.h"

namespace djnn {
class Identity : public AbstractTransformation {
  public:
    Identity (CoreProcess* parent, const string& name);
    Identity ();
    void        draw () override;
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};
} // namespace djnn