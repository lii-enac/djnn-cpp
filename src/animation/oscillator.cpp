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

#include "../core/serializer/serializer.h"
#include "animation.h"

namespace djnn
{
  Oscillator::OscillatorAction::OscillatorAction (Process *p, const string &n, shared_ptr<DoubleProperty> m,
                                                  shared_ptr<DoubleProperty> k, shared_ptr<DoubleProperty> b,
                                                  shared_ptr<DoubleProperty> v, shared_ptr<DoubleProperty> output,
                                                  shared_ptr<DoubleProperty> dt) :
      Process (p, n), _m (m), _k (k), _b (b), _v (v), _output (output), _dt (dt)
  {
  }

  void
  Oscillator::OscillatorAction::activate ()
  {
    double f = (-_k->get_value () * _output->get_value ()) - (_b->get_value () * _v->get_value ());
    double a = f / _m->get_value ();
    double v = _v->get_value () + a * _dt->get_value ();
    double output = _output->get_value () + v * _dt->get_value ();
    _v->set_value (v, true);
    _output->set_value (output, true);
  }

  Oscillator::Oscillator (Process *p, const string &n) :
      Process (p, n)
  {
    _m = make_shared<DoubleProperty> (this, "m", 1);
    _k = make_shared<DoubleProperty> (this, "k", 1);
    _damping = make_shared<DoubleProperty> (this, "damping", 0);
    _v = make_shared<DoubleProperty> (this, "v", 0);
    _output = make_shared<DoubleProperty> (this, "output", 1);
    _dt = make_shared<DoubleProperty> (this, "dt", 0.001);
    _step = make_shared<Spike> (this, "step");
    _action = make_unique < OscillatorAction > (this, "action", _m, _k, _damping, _v, _output, _dt);
    _c_step = make_unique < Coupling > (_step.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  Oscillator::~Oscillator ()
  {
    Graph::instance ().remove_edge (_step.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  Oscillator::activate ()
  {
    _c_step->enable ();
  }

  void
  Oscillator::deactivate ()
  {
    _c_step->disable ();
  }

  void
  Oscillator::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("animation:oscillator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
}

