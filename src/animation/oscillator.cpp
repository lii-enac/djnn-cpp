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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "animation.h"

#include "core/execution/graph.h"
#include "core/serializer/serializer.h"

namespace djnn
{
  Oscillator::OscillatorAction::OscillatorAction (Process *parent, const std::string& name, DoubleProperty* m,
                                                  DoubleProperty* k, DoubleProperty* b,
                                                  DoubleProperty* v, DoubleProperty* output,
                                                  DoubleProperty* dt) :
      Action (parent, name), _m (m), _k (k), _b (b), _v (v), _output (output), _dt (dt)
  {
  }

  void
  Oscillator::OscillatorAction::impl_activate ()
  {
    double f = (-_k->get_value () * _output->get_value ()) - (_b->get_value () * _v->get_value ());
    double a = f / _m->get_value ();
    double v = _v->get_value () + a * _dt->get_value ();
    double output = _output->get_value () + v * _dt->get_value ();
    _v->set_value (v, true);
    _output->set_value (output, true);
  }

  Oscillator::Oscillator (Process *parent, const std::string& name)
  : Process (name),
  _m (this, "m", 1),
  _k (this, "k", 1),
  _damping (this, "damping", 0),
  _v (this, "v", 0),
  _output (this, "output", 1),
  _dt (this, "dt", 0.001),
  _step (this, "step"),
  _c_step (&_step, ACTIVATION, &_action, ACTIVATION),
  _action (this, "action", &_m, &_k, &_damping, &_v, &_output, &_dt)
  {
    _c_step.disable ();
    Graph::instance ().add_edge (&_step, &_action);
    Graph::instance ().add_edge (&_action, &_output);
    Process::finalize_construction (parent, name);
  }

  Oscillator::~Oscillator ()
  { 
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (&_step, &_action);
    Graph::instance ().remove_edge (&_action, &_output);
  }

  void
  Oscillator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    Process::set_parent (p); 
  }

  void
  Oscillator::impl_activate ()
  {
    _c_step.enable ();
  }

  void
  Oscillator::impl_deactivate ()
  {
    _c_step.disable ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Oscillator::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("animation:oscillator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
#endif

}

