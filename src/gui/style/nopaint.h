#pragma once

#include "abstract_style.h"

namespace djnn {
  class NoOutline : public AbstractStyle
  {
  public:
    NoOutline (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoOutline () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoOutline* clone () override;
  };

  class NoFill : public AbstractStyle
  {
  public:
    NoFill (ParentProcess* parent, const std::string& name) :
        AbstractStyle (parent, name) { finalize_construction (parent, name); }
    virtual ~NoFill () {}
    void impl_activate () override { AbstractStyle::impl_activate ();}
    void impl_deactivate () override { AbstractStyle::impl_deactivate ();}
    void draw () override;
    NoFill* clone () override;
  };
}