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

    if (_parent && _parent->state_dependency () != nullptr) {
      Graph::instance ().add_edge (_parent->state_dependency (), _action_reset);
      Graph::instance ().add_edge (_parent->state_dependency (), _action_step);
    }
    
    Process::finalize ();
  }

  Counter::~Counter () {
    if (_parent && _parent->state_dependency () != nullptr) {
      Graph::instance ().remove_edge (_parent->state_dependency (), _action_reset);
      Graph::instance ().remove_edge (_parent->state_dependency (), _action_step);
    }
    Graph::instance ().remove_edge (_reset, _action_reset);
    Graph::instance ().remove_edge (_action_reset, _output);
    Graph::instance ().remove_edge (_step, _action_step);
    Graph::instance ().remove_edge (_action_step, _output);

    if (_c_step) { delete _c_step; _c_step = nullptr;}
    if (_action_step) { delete _action_step; _action_step = nullptr;}
    if (_c_reset) { delete _c_reset; _c_reset = nullptr;}
    if (_action_reset) { delete _action_reset; _action_reset = nullptr;}
    if (_delta) { delete _delta; _delta = nullptr;}
    if (_init) { delete _init; _init = nullptr;}
    if (_output) { delete _output; _output = nullptr;}
    if (_step) { delete _step; _step = nullptr;}
    if (_reset) { delete _reset; _reset = nullptr;}
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









