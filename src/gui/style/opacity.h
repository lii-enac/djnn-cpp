#pragma once

#include "gen/abstract_opacity.h"

namespace djnn {
class OutlineOpacity : public AbstractOpacity {
  public:
    OutlineOpacity (CoreProcess* parent, const string& name, double alpha)
        : AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~OutlineOpacity () {}
    void            draw () override;
    OutlineOpacity* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

class FillOpacity : public AbstractOpacity {
  public:
    FillOpacity (CoreProcess* parent, const string& name, double alpha)
        : AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~FillOpacity () {}
    void         draw () override;
    FillOpacity* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

} // namespace djnn