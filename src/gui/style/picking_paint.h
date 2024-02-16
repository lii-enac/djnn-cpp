#pragma once

#include "abstract_style.h"

namespace djnn {
class PickOutline : public AbstractStyle
{
  public:
    PickOutline (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~PickOutline () {}
    void         impl_activate () override { AbstractStyle::impl_activate (); }
    void         impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void         draw () override;
    PickOutline* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class PickFill : public AbstractStyle
{
  public:
    PickFill (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~PickFill () {}
    void      impl_activate () override { AbstractStyle::impl_activate (); }
    void      impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void      draw () override;
    PickFill* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class NoPickOutline : public AbstractStyle
{
  public:
    NoPickOutline (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoPickOutline () {}
    void           impl_activate () override { AbstractStyle::impl_activate (); }
    void           impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void           draw () override;
    NoPickOutline* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};

class NoPickFill : public AbstractStyle
{
  public:
    NoPickFill (CoreProcess* parent, const string& name)
        : AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoPickFill () {}
    void        impl_activate () override { AbstractStyle::impl_activate (); }
    void        impl_deactivate () override { AbstractStyle::impl_deactivate (); }
    void        draw () override;
    NoPickFill* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
};
} // namespace djnn