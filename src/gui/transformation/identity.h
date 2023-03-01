#pragma once

#include "abstract_transformation.h"


namespace djnn
{
  class Identity : public AbstractTransformation
  {
  public:
    Identity (ParentProcess* parent, const string& name);
    Identity ();
    void draw () override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}