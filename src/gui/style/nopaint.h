#pragma once

#include "abstract_style.h"

namespace djnn {
class NoOutline : public AbstractStyle
{
  public:
    NoOutline (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoOutline () {}
    void       impl_activate () override { AbstractStyle::impl_activate (); }
    void       impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void       draw () override;
    NoOutline* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class NoFill : public AbstractStyle
{
  public:
    NoFill (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoFill () {}
    void    impl_activate () override { AbstractStyle::impl_activate (); }
    void    impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void    draw () override;
    NoFill* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};
} // namespace djnn