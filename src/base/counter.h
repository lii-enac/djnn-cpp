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

#include "../core/tree/process.h"
#include "../core/tree/spike.h"
#include "../core/tree/double_property.h"

namespace djnn
{

  class Counter : public Process
  {
  private:

    /* RESET ACTION */
    class CounterResetAction : public Process
    {
    public:
      CounterResetAction (Process* p, const string &n, shared_ptr<DoubleProperty> init, shared_ptr<DoubleProperty> output) :
      Process (p, n), _init (init), _output (output) { Process::finalize ();};
    
      virtual ~CounterResetAction () {}
      void activate () override
      {
        _output->set_value (_init->get_value (), true);
      }
      void deactivate () override {}
    private:
    	shared_ptr<DoubleProperty> _init;
    	shared_ptr<DoubleProperty> _output;

    };

    /* STEP ACTION */
    class CounterStepAction : public Process
    {
    public:
      CounterStepAction (Process* p, const string &n, shared_ptr<DoubleProperty> delta, shared_ptr<DoubleProperty> output) :
      Process (p, n), _delta (delta), _output (output) { Process::finalize ();};
    
      virtual ~CounterStepAction () {}
      void activate () override
      {
        _output->set_value (_output->get_value () +  _delta->get_value (), true);
      }
      void deactivate () override {}
    private:
    	shared_ptr<DoubleProperty> _delta;
    	shared_ptr<DoubleProperty> _output;

    };

  /* COUNTER ACTION */
  public:
    Counter (Process* p, const std::string& n, double init, double delta);
    ~Counter();
    void activate () override {};
    void deactivate () override {};
    void serialize (const string& type) override;

  private:
    shared_ptr <Spike> _reset, _step;
    shared_ptr <DoubleProperty> _output, _init, _delta;
    unique_ptr<Coupling> _c_reset, _c_step;
    shared_ptr<Process> _action_reset, _action_step;
  };

}
