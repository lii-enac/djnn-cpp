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

#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  Oscillator::OscillatorAction::OscillatorAction (Process *p, const string &n, DoubleProperty* m,
                                                  DoubleProperty* k, DoubleProperty* b,
                                                  DoubleProperty* v, DoubleProperty* output,
                                                  DoubleProperty* dt) :
      Action (p, n), _m (m), _k (k), _b (b), _v (v), _output (output), _dt (dt)
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

  Oscillator::Oscillator (Process *p, const string &n) :
      Process (n)
  {
    _m = new DoubleProperty (this, "m", 1);
    _k = new DoubleProperty (this, "k", 1);
    _damping = new DoubleProperty (this, "damping", 0);
    _v = new DoubleProperty (this, "v", 0);
    _output = new DoubleProperty (this, "output", 1);
    _dt = new DoubleProperty (this, "dt", 0.001);
    _step = new Spike (this, "step");
    _action = new OscillatorAction (this, "action", _m, _k, _damping, _v, _output, _dt);
    _c_step = new Coupling (_step, ACTIVATION, _action, ACTIVATION);
    _c_step->disable ();
    Graph::instance ().add_edge (_step, _action);
    Graph::instance ().add_edge (_action, _output);
    Process::finalize_construction (p);
  }

  Oscillator::~Oscillator ()
  { 
    remove_state_dependency (get_parent (), _action);
    Graph::instance ().remove_edge (_step, _action);
    Graph::instance ().remove_edge (_action, _output);
    
    delete _c_step;
    delete _action;
    delete _step;
    delete _dt;
    delete _v;
    delete _output;
    delete _damping;
    delete _k;
    delete _m;
  }

  void
  Oscillator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), _action);
    }

    add_state_dependency (p, _action);
    Process::set_parent (p); 
  }

  void
  Oscillator::impl_activate ()
  {
    _c_step->enable ();
  }

  void
  Oscillator::impl_deactivate ()
  {
    _c_step->disable ();
  }

  void
  Oscillator::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("animation:oscillator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
}

