#pragma once

#include "gen/abstract_opacity.h"

namespace djnn {
  class OutlineOpacity : public AbstractOpacity
  {
  public:
    OutlineOpacity (ParentProcess* parent, const string& name, double alpha) :
        AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~OutlineOpacity () {}
    void draw () override;
    OutlineOpacity* clone () override;
  };

  class FillOpacity : public AbstractOpacity
  {
  public:
    FillOpacity (ParentProcess* parent, const string& name, double alpha) :
        AbstractOpacity (parent, name, alpha) { finalize_construction (parent, name); }
    virtual ~FillOpacity () {}
    void draw () override;
    FillOpacity* clone () override;
  };

}