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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "geometry.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"

namespace djnn
{
  HermiteCurve::HermiteCurveAction::HermiteCurveAction (CoreProcess* parent, const string& name,
                                                        AbstractSimpleProperty *input, AbstractSimpleProperty *p1, AbstractSimpleProperty *p2,
                                                        AbstractSimpleProperty *t1, AbstractSimpleProperty *t2, AbstractSimpleProperty *output) :
      Action (parent, name), _input (input), _p1 (p1), _p2 (p2), _t1 (t1), _t2 (t2), _output (output)
  {
  }

  void
  HermiteCurve::HermiteCurveAction::impl_activate ()
  {
    double p1 = ((DoubleProperty*) _p1)->get_value ();
    double p2 = ((DoubleProperty*) _p2)->get_value ();
    double t1 = ((DoubleProperty*) _t1)->get_value ();
    double t2 = ((DoubleProperty*) _t2)->get_value ();
    double in = ((DoubleProperty*) _input)->get_value ();
    double pow2 = in * in;
    double pow3 = pow2 * in;
    double h1 = 2 * pow3 - 3 * pow2 + 1;
    double h2 = -2 * pow3 + 3 * pow2;
    double h3 = pow3 - 2 * pow2 + in;
    double h4 = pow3 - pow2;
    double out = h1 * p1 + h2 * p2 + h3 * t1 + h4 * t2;
    _output->set_value (out, true);
  }

  HermiteCurve::HermiteCurve (CoreProcess* parent, const string& name, double p1, double p2, double t1, double t2) 
  : FatProcess (name),
  _input (this, "input", 0),
  _p1 (this, "p1", p1),
  _p2 (this, "p2", p2),
  _t1 (this, "t1", t1),
  _t2 (this, "t2", t2),
  _output (this, "output", 0),
  _action (this, "action", &_input, &_p1, &_p2, &_t1, &_t2, &_output),
  _c_input (&_input, ACTIVATION, &_action, ACTIVATION),
  _c_p1 (&_p1, ACTIVATION, &_action, ACTIVATION),
  _c_p2 (&_p2, ACTIVATION, &_action, ACTIVATION),
  _c_t1 (&_t1, ACTIVATION, &_action, ACTIVATION),
  _c_t2 (&_t2, ACTIVATION, &_action, ACTIVATION)
  {
    _c_input.disable ();
    _c_p1.disable ();
    _c_p2.disable ();
    _c_t1.disable ();
    _c_t2.disable ();
    graph_add_edge (&_action, &_output);
    finalize_construction (parent, name);
  }

  HermiteCurve::~HermiteCurve ()
  {
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_output);
  }

  void
  HermiteCurve::set_parent (CoreProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);

    FatProcess::set_parent (parent); 
  }

  void
  HermiteCurve::impl_activate ()
  {
    _c_input.enable ();
    _c_p1.enable ();
    _c_p2.enable ();
    _c_t1.enable ();
    _c_t2.enable ();
  }

  void
  HermiteCurve::impl_deactivate ()
  {
    _c_input.disable ();
    _c_p1.disable ();
    _c_p2.disable ();
    _c_t1.disable ();
    _c_t2.disable ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  HermiteCurve::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:hermitecurve");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("p1", _p1.get_value ());
    AbstractSerializer::serializer->float_attribute ("p2", _p2.get_value ());
    AbstractSerializer::serializer->float_attribute ("t1", _t1.get_value ());
    AbstractSerializer::serializer->float_attribute ("t2", _t2.get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif
}

