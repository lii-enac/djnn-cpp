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

#include "core/ontology/coupling.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"

namespace djnn
{
  Counter::Counter (ParentProcess* parent, const std::string& name, double init, double delta)
  : FatProcess (name),
    _reset (this, "reset"),
    _step (this, "step"),
    _output (this, "output", init),
    _init (this, "init", init),
    _delta (this, "delta", delta),
    _action_reset (this, name + "_action_reset", &_reset_occurred),
    _c_reset (&_reset, ACTIVATION, &_action_reset, ACTIVATION),
    _action_step (this, name + "_action_step", &_init, &_delta, &_output, &_reset_occurred),
    _c_step (&_step, ACTIVATION, &_action_step, ACTIVATION)
  {
    _c_reset.disable (),
    _c_step.disable ();

    graph_add_edge (&_reset, &_action_reset);
    graph_add_edge (&_action_reset, &_output);
    
    graph_add_edge (&_step, &_action_step);
    graph_add_edge (&_action_step, &_output);

    add_state_dependency (get_parent (), &_action_reset);
    add_state_dependency (get_parent (), &_action_step);
    
    if(parent)
      finalize_construction (parent, name);
  }

  Counter::~Counter () {
    remove_state_dependency (get_parent (), &_action_reset);
    remove_state_dependency (get_parent (), &_action_step);
    graph_remove_edge (&_reset, &_action_reset);
    graph_remove_edge (&_action_reset, &_output);
    graph_remove_edge (&_step, &_action_step);
    graph_remove_edge (&_action_step, &_output);
  }

  void
  Counter::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), &_action_reset);
       remove_state_dependency (get_parent (), &_action_step);
    }

    add_state_dependency (parent, &_action_reset);
    add_state_dependency (parent, &_action_step);

    FatProcess::set_parent (parent); 
  }

  void
  Counter::impl_activate () {
    _c_reset.enable ();
    _c_step.enable ();
  }

  void
  Counter::impl_deactivate () {
    _c_reset.disable ();
    _c_step.disable ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Counter::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:counter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("init", _init.get_value ());
    AbstractSerializer::serializer->float_attribute ("delta", _delta.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

}









