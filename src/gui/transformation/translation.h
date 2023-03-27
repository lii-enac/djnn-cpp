#pragma once

#include "abstract_transformation.h"
#include "gen/abstract_translation.h"

namespace djnn
{
  class Translation : public AbstractTranslation
  {
  public:
    Translation (CoreProcess* parent, const string& name, double tx, double ty);
    Translation (double tx, double ty);
    virtual ~Translation ();
    void draw () override;
    virtual void transform( double& x, double& y) override;
    virtual void inverse_transform( double& x, double& y) override;
    FatProcess* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  };
}