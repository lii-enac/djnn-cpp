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
    _reset = make_shared<Spike> (this, "reset");
    _step = make_shared<Spike> (this, "step");
    _output = make_shared<DoubleProperty> (this, "output", init);
    _init = make_shared<DoubleProperty> (this, "init", init);
    _delta = make_shared<DoubleProperty> (this, "delta", delta);

    /* reset action */
    _action_reset = make_shared<CounterResetAction> (this, n + "_action_reset", &_reset_occurred);
    _c_reset = make_unique<Coupling> (_reset.get (), ACTIVATION, _action_reset.get (), ACTIVATION);
    _c_reset->disable ();
    Graph::instance ().add_edge (_reset.get (), _action_reset.get ());
    Graph::instance ().add_edge (_action_reset.get (), _output.get ());

    /* step action */    
    _action_step = make_shared<CounterStepAction> (this, n + "_action_step", _init, _delta, _output, &_reset_occurred);
    _c_step = make_unique<Coupling> (_step.get (), ACTIVATION, _action_step.get (), ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step.get (), _action_step.get ());
    Graph::instance ().add_edge (_action_step.get (), _output.get ());

    if (_parent && _parent->state_dependency () != nullptr) {
      Graph::instance ().add_edge (_parent->state_dependency (), _action_reset.get ());
      Graph::instance ().add_edge (_parent->state_dependency (), _action_step.get ());
    }
    
    Process::finalize ();
  }

  Counter::~Counter () {
    if (_parent && _parent->state_dependency () != nullptr) {
      Graph::instance ().remove_edge (_parent->state_dependency (), _action_reset.get ());
      Graph::instance ().remove_edge (_parent->state_dependency (), _action_step.get ());
    }
    Graph::instance ().remove_edge (_reset.get (), _action_reset.get ());
    Graph::instance ().remove_edge (_action_reset.get (), _output.get ());
    Graph::instance ().remove_edge (_step.get (), _action_step.get ());
    Graph::instance ().remove_edge (_action_step.get (), _output.get ());
  }

  void
  Counter::activate () {
    _c_reset->enable ();
    _c_step->enable ();
  }

  void
  Counter::deactivate () {
    _c_reset->disable ();
    _c_step->disable ();
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









