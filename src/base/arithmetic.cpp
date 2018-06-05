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

#include "arithmetic.h"

namespace djnn
{
  Adder::Adder (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", l_val + r_val));
    init_couplings (
        shared_ptr<Process> (new AdderAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Subtractor::Subtractor (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", l_val - r_val));
    init_couplings (
        shared_ptr<Process> (new SubtractorAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Multiplier::Multiplier (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", l_val * r_val));
    ;
    init_couplings (
        shared_ptr<Process> (new MultiplierAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Divider::Divider (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", 0));
    if (r_val != 0)
      _result.get ()->set_value (l_val / r_val, true);
    init_couplings (
        shared_ptr<Process> (new DividerAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  Modulo::Modulo (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new IntProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new IntProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new IntProperty (this, "result", 0));
    if ((int) r_val != 0)
      _result.get ()->set_value ((int) l_val % (int) r_val, true);
    init_couplings (
        shared_ptr<Process> (new ModuloAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  AscendingComparator::AscendingComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val <= r_val));
    init_couplings (
        shared_ptr<Process> (
            new AscendingComparatorAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  StrictAscendingComparator::StrictAscendingComparator (Process *p, const string &n, double l_val,
                                                        double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val < r_val));
    init_couplings (
        shared_ptr<Process> (
            new StrictAscendingComparatorAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  EqualityComparator::EqualityComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "left", l_val));
    _right = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "right", r_val));
    _result = std::shared_ptr<AbstractProperty> (new BoolProperty (this, "result", l_val == r_val));
    init_couplings (
        shared_ptr<Process> (
            new EqualityComparatorAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  SignInverter::SignInverter (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", -(i_val)));
    init_couplings (
        shared_ptr<Process> (new SignInverterAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  Predecessor::Predecessor (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", -(i_val)));
    init_couplings (
        shared_ptr<Process> (new PredecessorAction (this, "action", _input, _output, i_val)));
    Process::finalize ();
  }

  int
  Incr::init_incr (bool isModel)
  {
    _delta = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "delta", 1));
    _state = std::shared_ptr<AbstractProperty> (new DoubleProperty (this, "state", 0));
    Graph::instance ().add_edge (this, _state.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _state.get ());
    _model = isModel;
    return 1;
  }

  Incr::Incr (bool isModel)
  {
    init_incr (isModel);
  }

  Incr::Incr (Process *parent, string name, bool isModel) :
      Process (parent, name)
  {
    init_incr (isModel);
    Process::finalize ();
  }

  Incr::~Incr ()
  {
    Graph::instance ().remove_edge (this, _state.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _state.get ());
  }

  void
  Incr::activate ()
  {
    _state.get ()->set_value (
        ((DoubleProperty*) _state.get ())->get_value () + ((DoubleProperty*) _delta.get ())->get_value (), true);
  }

  AdderAccumulator::AdderAccumulatorAction::AdderAccumulatorAction (Process* parent, const string &name,
                                                                    shared_ptr<AbstractProperty> input,
                                                                    shared_ptr<AbstractProperty> clamp_min,
                                                                    shared_ptr<AbstractProperty> clamp_max,
                                                                    shared_ptr<AbstractProperty> result) :
      Process (parent, name), _input (input), _clamp_min (clamp_min), _clamp_max (clamp_max), _result (result)
  {
    Process::finalize ();
  }

  void
  AdderAccumulator::AdderAccumulatorAction::activate ()
  {
    if (_parent->get_state () > activated)
      return;
    double input = ((DoubleProperty*) _input.get ())->get_value ();
    double clamp_min = ((DoubleProperty*) _clamp_min.get ())->get_value ();
    double clamp_max = ((DoubleProperty*) _clamp_max.get ())->get_value ();
    double value = ((DoubleProperty*) _result.get ())->get_value ();
    value += input;
    if (clamp_min < clamp_max) {
      value = value < clamp_min ? clamp_min : value;
      value = value > clamp_max ? clamp_max : value;
    }
    _result->set_value (value, true);
  }

  AdderAccumulator::AdderAccumulator (Process* parent, const string &name, double input, double clamp_min,
                                      double clamp_max) :
      Process (parent, name)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", input));
    _clamp_min = shared_ptr<AbstractProperty> (new DoubleProperty (this, "clamp_min", clamp_min));
    _clamp_max = shared_ptr<AbstractProperty> (new DoubleProperty (this, "clamp_max", clamp_max));

    if (clamp_min < clamp_max) {
      input = input < clamp_min ? clamp_min : input;
      input = input > clamp_max ? clamp_max : input;
    }
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", input));
    _action = make_unique<AdderAccumulatorAction> (this, name + "_action", _input, _clamp_min, _clamp_max, _result);
    _c_input = make_unique<Coupling> (_input.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_input->disable ();
    Graph::instance ().add_edge (_input.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _result.get ());
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  AdderAccumulator::~AdderAccumulator ()
  {
    Graph::instance ().remove_edge (_input.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _result.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  AdderAccumulator::activate ()
  {
    _c_input->enable ();
  }

  void
  AdderAccumulator::deactivate ()
  {
    _c_input->disable ();
  }
}

