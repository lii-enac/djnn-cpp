#pragma once

#include "core/tree/component.h"

namespace djnn {

class Defs : public Container {
  public:
    Defs (CoreProcess* parent, const string& name);
    virtual ~Defs () override;
    void                   impl_activate () override;
    void                   impl_deactivate () override;
    virtual process_type_e get_process_type () const override { return DEFS_T; }
    Defs*                  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};

} // namespace djnn