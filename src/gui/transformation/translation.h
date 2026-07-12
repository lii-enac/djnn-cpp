/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2025)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "abstract_transformation.h"
#include "gen/abstract_translation.h"

namespace djnn {
class Translation : public AbstractTranslation {
  public:
    Translation (CoreProcess* parent, const string& name, double tx, double ty);
    Translation (double tx, double ty);
    virtual ~Translation ();
    void         draw () override;
    virtual void transform (double& x, double& y) override;
    virtual void inverse_transform (double& x, double& y) override;
    FatProcess*  impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;
};
} // namespace djnn