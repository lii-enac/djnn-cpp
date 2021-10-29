#pragma once

#include "abstract_style.h"

namespace djnn {
  class PickOutline : public AbstractStyle
  {
  public:
    PickOutline (ParentProcess* parent, const string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~PickOutline () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    PickOutline* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class PickFill : public AbstractStyle
  {
  public:
    PickFill (ParentProcess* parent, const string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~PickFill () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    PickFill* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class NoPickOutline : public AbstractStyle
  {
  public:
    NoPickOutline (ParentProcess* parent, const string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoPickOutline () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoPickOutline* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };

  class NoPickFill : public AbstractStyle
  {
  public:
    NoPickFill (ParentProcess* parent, const string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoPickFill () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoPickFill* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}