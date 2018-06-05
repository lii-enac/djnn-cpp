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

#include "geometry.h"

namespace djnn
{
  HermiteCurve::HermiteCurveAction::HermiteCurveAction (Process* parent, const string &name,
                                                        shared_ptr<AbstractProperty> input,
                                                        shared_ptr<AbstractProperty> t1,
                                                        shared_ptr<AbstractProperty> t2,
                                                        shared_ptr<AbstractProperty> p1,
                                                        shared_ptr<AbstractProperty> p2,
                                                        shared_ptr<AbstractProperty> output) :
      _input (input), _t1 (t1), _t2 (t2), _p1 (p1), _p2 (p2), _output (output)
  {
    Process::finalize ();
  }

  void
  HermiteCurve::HermiteCurveAction::activate ()
  {
    if (_parent->get_state () > activated)
      return;
    double t1 = ((DoubleProperty*) _t1.get ())->get_value ();
    double t2 = ((DoubleProperty*) _t2.get ())->get_value ();
    double p1 = ((DoubleProperty*) _p1.get ())->get_value ();
    double p2 = ((DoubleProperty*) _p2.get ())->get_value ();
    double in = ((DoubleProperty*) _t1.get ())->get_value ();
    double pow2 = in * in;
    double pow3 = pow2 * in;
    double h1 = 2 * pow3 - 3 * pow2 + 1;
    double h2 = -2 * pow3 + 3 * pow2;
    double h3 = pow3 - 2 * pow2 + in;
    double h4 = pow3 - pow2;
    double out = h1 * p1 + h2 * p2 + h3 * t1 + h4 * t2;
    _output->set_value (out, true);
  }

  HermiteCurve::HermiteCurve (Process *p, const string &n, double t1, double t2, double p1, double p2) :
      Process (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", 0));
    _t1 = shared_ptr<AbstractProperty> (new DoubleProperty (this, "t1", t1));
    _t2 = shared_ptr<AbstractProperty> (new DoubleProperty (this, "t2", t2));
    _p1 = shared_ptr<AbstractProperty> (new DoubleProperty (this, "p1", p1));
    _p2 = shared_ptr<AbstractProperty> (new DoubleProperty (this, "p2", p2));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", 0));
    _action = make_unique<HermiteCurveAction> (this, "action", _input, _t1, _t2, _p1, _p2, _output);
    _c_input = make_unique<Coupling> (_input.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_input->disable ();
    _c_t1 = make_unique<Coupling> (_t1.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_t1->disable ();
    _c_t2 = make_unique<Coupling> (_t2.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_t2->disable ();
    _c_p1 = make_unique<Coupling> (_p1.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_p1->disable ();
    _c_p2 = make_unique<Coupling> (_p2.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_p2->disable ();
    Graph::instance ().add_edge (_input.get (), _action.get ());
    Graph::instance ().add_edge (_t1.get (), _action.get ());
    Graph::instance ().add_edge (_t2.get (), _action.get ());
    Graph::instance ().add_edge (_p1.get (), _action.get ());
    Graph::instance ().add_edge (_p2.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  HermiteCurve::~HermiteCurve ()
  {
    Graph::instance ().remove_edge (_input.get (), _action.get ());
    Graph::instance ().remove_edge (_t1.get (), _action.get ());
    Graph::instance ().remove_edge (_t2.get (), _action.get ());
    Graph::instance ().remove_edge (_p1.get (), _action.get ());
    Graph::instance ().remove_edge (_p2.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  HermiteCurve::activate ()
  {
    _c_input->enable ();
    _c_t1->enable ();
    _c_t2->enable ();
    _c_p1->enable ();
    _c_p2->enable ();
  }

  void
  HermiteCurve::deactivate ()
  {
    _c_input->disable ();
    _c_t1->disable ();
    _c_t2->disable ();
    _c_p1->disable ();
    _c_p2->disable ();
  }
}

