#pragma once

#include "core/control/action.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/double_property.h"

namespace djnn {
class Incr : public FatProcess {
  private:
    class StepAction : public Action {
      public:
        StepAction (CoreProcess* parent, const string& name)
            : Action (parent, name) { finalize_construction (parent, name); }
        virtual ~StepAction () {}
        void impl_activate () { ((Incr*)get_parent ())->step (); }
    };

  public:
    Incr (CoreProcess* parent, const string& name, bool is_model);
    bool pre_activate () override;
    void impl_activate () override;
    void impl_deactivate () override;
    void step ();
    virtual ~Incr ();

  private:
    int            init_incr (bool isModel);
    DoubleProperty _delta, _state;
    Spike          _step;
    StepAction     _action;
    Coupling       _c_step;
    bool           _is_model;

  protected:
    void set_parent (CoreProcess* parent) override;
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
};

class AdderAccumulator : public FatProcess {
  private:
    class AdderAccumulatorAction : public Action {
      public:
        AdderAccumulatorAction (CoreProcess* parent, const string& name, AdderAccumulator& aa);
        void impl_activate () override;

      private:
        AdderAccumulator& _aa;
    };

  public:
    AdderAccumulator (CoreProcess* parent, const string& name, double input, double clamp_min, double clamp_max);
    virtual ~AdderAccumulator ();
    void impl_activate () override;
    void impl_deactivate () override;

  protected:
    void set_parent (CoreProcess* parent) override;
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    friend class AdderAccumulatorAction;
    DoubleProperty         _input, _clamp_min, _clamp_max, _result;
    AdderAccumulatorAction _action;
    Coupling               _c_input;
};
} // namespace djnn