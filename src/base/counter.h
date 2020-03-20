/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/spike.h"
#include "core/tree/double_property.h"

namespace djnn
{

  class Counter : public Process
  {
  private:

    /* RESET ACTION */
    class CounterResetAction : public Action
    {
    public:
      CounterResetAction (Process* parent, const std::string &name, bool* reset_occurred) :
        Action (parent, name), _reset_occurred(reset_occurred) {};
    
      virtual ~CounterResetAction () {}
      void impl_activate () override { *_reset_occurred = true; }
      void impl_deactivate () override {}
    private:
      bool *_reset_occurred;
    };

    /* STEP ACTION */
    class CounterStepAction : public Action
    {
    public:
      CounterStepAction (Process* parent, const std::string &name, DoubleProperty *init, DoubleProperty *delta, DoubleProperty *output, bool* reset_occurred) :
        Action (parent, name), _init(init), _delta (delta), _output (output), _reset_occurred(reset_occurred) {};
    
      virtual ~CounterStepAction () {}
      void impl_activate () override
      {
        /* if reset occured wew send _init else (_output + _delta ) */
        if (*_reset_occurred) {
          _output->set_value (_init->get_value (), true);
          *_reset_occurred = false;
        }
        else
          _output->set_value (_output->get_value () +  _delta->get_value (), true);
      }
      void impl_deactivate () override {}
    private:
      DoubleProperty *_init;
    	DoubleProperty *_delta;
    	DoubleProperty *_output;
      bool *_reset_occurred;
    };

  /* COUNTER ACTION */
  public:
    Counter (Process* p, const std::string& n, double init, double delta);
    ~Counter();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const std::string& type) override;

  private:
    void set_parent (Process* p) override;
    bool _reset_occurred;
    Spike _reset, _step;
    DoubleProperty _output, _init, _delta;
    CounterResetAction _action_reset;
    Coupling _c_reset;
    CounterStepAction _action_step;
    Coupling _c_step;
  };

}
