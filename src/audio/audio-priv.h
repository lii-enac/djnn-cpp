#pragma once

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/double_property.h"

namespace djnn {

class AudioListener : public FatProcess {
  public:
    class GainAction : public Action {
      public:
        GainAction (CoreProcess* parent, const string& name)
            : Action (parent, name){};
        virtual ~GainAction () {}
        void impl_activate () override { ((AudioListener*)get_parent ())->update_gain (); }
    };

  public:
    AudioListener (CoreProcess* parent, const string& name);
    DoubleProperty* gain () { return &_gain; }

    void impl_activate () override {}
    void impl_deactivate () override {}

  protected:
    void           update_gain ();
    DoubleProperty _gain;

    GainAction _gain_action;
    Coupling   _c_gain_action;
};
} // namespace djnn