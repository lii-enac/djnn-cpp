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
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"

namespace djnn {
Oscillator::OscillatorAction::OscillatorAction (CoreProcess* parent, const string& name, DoubleProperty* m,
                                                DoubleProperty* k, DoubleProperty* b,
                                                DoubleProperty* v, DoubleProperty* output,
                                                DoubleProperty* dt)
    : Action (parent, name), _m (m), _k (k), _b (b), _v (v), _output (output), _dt (dt)
{
}

void
Oscillator::OscillatorAction::impl_activate ()
{
    double f      = (-_k->get_value () * _output->get_value ()) - (_b->get_value () * _v->get_value ());
    double a      = f / _m->get_value ();
    double v      = _v->get_value () + a * _dt->get_value ();
    double output = _output->get_value () + v * _dt->get_value ();
    _v->set_value (v, true);
    _output->set_value (output, true);
}

Oscillator::Oscillator (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _m (this, "m", 1),
      _k (this, "k", 1),
      _damping (this, "damping", 0),
      _v (this, "v", 0),
      _output (this, "output", 1),
      _dt (this, "dt", 0.001),
      _step (this, "step"),
      _action (this, "action", &_m, &_k, &_damping, &_v, &_output, &_dt),
      _c_step (&_step, ACTIVATION, &_action, ACTIVATION)
{
    _c_step.disable ();
    graph_add_edge (&_action, &_output);
    finalize_construction (parent, name);
}

Oscillator::~Oscillator ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_output);
}

void
Oscillator::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);
    FatProcess::set_parent (parent);
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
Oscillator::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("animation:oscillator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
