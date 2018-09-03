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


#include "../core/serializer/serializer.h"
#include "../core/control/coupling.h"
#include "counter.h"

#include <iostream>

namespace djnn
{
  Counter::Counter (Process *p, const std::string& n, double init, double delta) :
      Process (p, n)
  {
    _reset = shared_ptr<Spike> (new Spike (this, "reset"));
    _step = shared_ptr<Spike> (new Spike (this, "step"));
    _output = shared_ptr<DoubleProperty> (new DoubleProperty(this, "output", 0));
    _init = shared_ptr<DoubleProperty> (new DoubleProperty(this, "init", init));
    _delta = shared_ptr<DoubleProperty> (new DoubleProperty(this, "delta", delta));


    /* reset action */
    _action_reset = shared_ptr<CounterResetAction> (new CounterResetAction (this, n + "_action_reset", _init, _output));
    _c_reset = make_unique<Coupling> (_reset.get (), ACTIVATION, _action_reset.get (), ACTIVATION);
    _c_reset->disable ();
    Graph::instance ().add_edge (_reset.get (), _action_reset.get ());
    Graph::instance ().add_edge (_action_reset.get (), _output.get ());

    /* step action */    
    _action_step = shared_ptr<CounterStepAction> (new CounterStepAction (this, n + "_action_step", _delta, _output));
    _c_step = make_unique<Coupling> (_step.get (), ACTIVATION, _action_step.get (), ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step.get (), _action_step.get ());
    Graph::instance ().add_edge (_action_step.get (), _output.get ());

    if (p && p->state_dependency () != nullptr) {
      Graph::instance ().add_edge (p->state_dependency (), _action_reset.get ());
      Graph::instance ().add_edge (p->state_dependency (), _action_step.get ());
    }
    
    Process::finalize ();
  }

  Counter::~Counter () {

    //
    //
    //

  }

  void
  Counter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:counter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("init", _init->get_value ());
    AbstractSerializer::serializer->float_attribute ("delta", _delta->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}









