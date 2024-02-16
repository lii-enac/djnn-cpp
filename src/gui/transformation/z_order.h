#pragma once

#include "gui/abstract_gobj.h"

namespace djnn {
class ZOrder : public AbstractGObj
{
  public:
    ZOrder (CoreProcess* parent, const string& name, int z);
    ~ZOrder () {}
    void        draw () override;
    FatProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
    void        impl_activate () override;
    void        impl_deactivate () override;

  private:
    IntProperty _z;
    Coupling    _c_z;
};
} // namespace djnn