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

#include "counter.h"

#include "../core/ontology/coupling.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

#include <iostream>

namespace djnn
{
  Counter::Counter (Process *p, const std::string& n, double init, double delta) :
      Process (n)
  {
    _reset = new Spike (this, "reset");
    _step = new Spike (this, "step");
    _output = new DoubleProperty (this, "output", init);
    _init = new DoubleProperty (this, "init", init);
    _delta = new DoubleProperty (this, "delta", delta);

    /* reset action */
    _action_reset = new CounterResetAction (this, n + "_action_reset", &_reset_occurred);
    _c_reset = new Coupling (_reset, ACTIVATION, _action_reset, ACTIVATION);
    _c_reset->disable ();
    Graph::instance ().add_edge (_reset, _action_reset);
    Graph::instance ().add_edge (_action_reset, _output);

    /* step action */    
    _action_step = new CounterStepAction (this, n + "_action_step", _init, _delta, _output, &_reset_occurred);
    _c_step = new Coupling (_step, ACTIVATION, _action_step, ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step, _action_step);
    Graph::instance ().add_edge (_action_step, _output);

    add_state_dependency (get_parent (), _action_reset);
    add_state_dependency (get_parent (), _action_step);
    
    Process::finalize_construction (p);
  }

  Counter::Counter (double init, double delta) :
      Process ()
  {
    _reset = new Spike (this, "reset");
    _step = new Spike (this, "step");
    _output = new DoubleProperty (this, "output", init);
    _init = new DoubleProperty (this, "init", init);
    _delta = new DoubleProperty (this, "delta", delta);

    /* reset action */
    _action_reset = new CounterResetAction (this, "counter_action_reset", &_reset_occurred);
    _c_reset = new Coupling (_reset, ACTIVATION, _action_reset, ACTIVATION);
    _c_reset->disable ();
    Graph::instance ().add_edge (_reset, _action_reset);
    Graph::instance ().add_edge (_action_reset, _output);

    /* step action */    
    _action_step = new CounterStepAction (this, "counter_action_step", _init, _delta, _output, &_reset_occurred);
    _c_step = new Coupling (_step, ACTIVATION, _action_step, ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step, _action_step);
    Graph::instance ().add_edge (_action_step, _output);
  }

  Counter::~Counter () {
    remove_state_dependency (get_parent (), _action_reset);
    remove_state_dependency (get_parent (), _action_step);
    Graph::instance ().remove_edge (_reset, _action_reset);
    Graph::instance ().remove_edge (_action_reset, _output);
    Graph::instance ().remove_edge (_step, _action_step);
    Graph::instance ().remove_edge (_action_step, _output);

    delete _c_step;
    delete _action_step;
    delete _c_reset;
    delete _action_reset;
    delete _delta;
    delete _init;
    delete _output;
    delete _step;
    delete _reset;
  }

  void
  Counter::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), _action_reset);
       remove_state_dependency (get_parent (), _action_step);
    }

    add_state_dependency (p, _action_reset);
    add_state_dependency (p, _action_step);

    Process::set_parent (p); 
  }

  void
  Counter::impl_activate () {
    _c_reset->enable ();
    _c_step->enable ();
  }

  void
  Counter::impl_deactivate () {
    _c_reset->disable ();
    _c_step->disable ();
  }


  void
  Counter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:counter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("init", _init->get_value ());
    AbstractSerializer::serializer->float_attribute ("delta", _delta->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}









