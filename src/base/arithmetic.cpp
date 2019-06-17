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

#include "arithmetic.h"

#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  Adder::Adder (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val + r_val);
    init_couplings (new AdderAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  Adder::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adder");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Subtractor::Subtractor (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val - r_val);
    init_couplings (new SubtractorAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  Subtractor::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:subtractor");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Multiplier::Multiplier (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val * r_val);
    init_couplings (new MultiplierAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  Multiplier::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:multiplier");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Divider::Divider (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", 0);
    if (r_val != 0)
      _result->set_value (l_val / r_val, true);
    init_couplings (new DividerAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  Divider::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:divider");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Modulo::Modulo (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = new IntProperty (this, "left", l_val);
    _right = new IntProperty (this, "right", r_val);
    _result = new IntProperty (this, "result", 0);
    if ((int) r_val != 0)
      _result->set_value ((int) l_val % (int) r_val, true);
    init_couplings (new ModuloAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  Modulo::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:modulo");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("left", dynamic_cast<IntProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("right",dynamic_cast<IntProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  AscendingComparator::AscendingComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val <= r_val);
    init_couplings (new AscendingComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  AscendingComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:ascendingcomparator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  StrictAscendingComparator::StrictAscendingComparator (Process *p, const string &n, double l_val,
                                                        double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val < r_val);
    init_couplings (new StrictAscendingComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  StrictAscendingComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:strictascendingcomparator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  EqualityComparator::EqualityComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val == r_val);
    init_couplings (new EqualityComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction ();
  }

  void
  EqualityComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:equalitycomparator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  SignInverter::SignInverter (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", -(i_val));
    init_couplings (new SignInverterAction (this, "action", _input, _output));
    Process::finalize_construction ();
  }

  void
  SignInverter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:signinverter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Previous::Previous (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", 0);
    init_couplings (new PreviousAction (this, "action", _input, _output, i_val));
    Process::finalize_construction ();
  }

  void
  Previous::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  int
  Incr::init_incr (bool isModel)
  {
    _delta = new DoubleProperty (this, "delta", 1);
    _state = new DoubleProperty (this, "state", 0);
    Graph::instance ().add_edge (this, _state);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _state);
    set_is_model (isModel);
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
    Process::finalize_construction ();
  }

  Incr::~Incr ()
  { 
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _state);
    Graph::instance ().remove_edge (this, _state);

    delete _state;
    delete _delta;
  }

  void
  Incr::activate ()
  {
    _state->set_value (
        ((DoubleProperty*) _state)->get_value () + ((DoubleProperty*) _delta)->get_value (), true);
  }

  void
  Incr::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:incr");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  AdderAccumulator::AdderAccumulatorAction::AdderAccumulatorAction (Process* parent, const string &name,
                                                                    AbstractProperty* input,
                                                                    AbstractProperty* clamp_min,
                                                                    AbstractProperty* clamp_max,
                                                                    AbstractProperty* result) :
      Process (parent, name), _input (input), _clamp_min (clamp_min), _clamp_max (clamp_max), _result (result)
  {
    Process::finalize_construction ();
  }

  void
  AdderAccumulator::AdderAccumulatorAction::activate ()
  {
    //if (_parent->get_state () > activated)
    if (!_parent->somehow_activating ())
      return;
    double input = ((DoubleProperty*) _input)->get_value ();
    double clamp_min = ((DoubleProperty*) _clamp_min)->get_value ();
    double clamp_max = ((DoubleProperty*) _clamp_max)->get_value ();
    double value = ((DoubleProperty*) _result)->get_value ();
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
    _input = new DoubleProperty (this, "input", input);
    _clamp_min = new DoubleProperty (this, "clamp_min", clamp_min);
    _clamp_max = new DoubleProperty (this, "clamp_max", clamp_max);

    if (clamp_min < clamp_max) {
      input = input < clamp_min ? clamp_min : input;
      input = input > clamp_max ? clamp_max : input;
    }
    _result = new DoubleProperty (this, "result", input);
    _action = new AdderAccumulatorAction (this, name + "_action", _input, _clamp_min, _clamp_max, _result);
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    _c_input->disable ();
    Graph::instance ().add_edge (_input, _action);
    Graph::instance ().add_edge (_action, _result);
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action);
    Process::finalize_construction ();
  }

  AdderAccumulator::~AdderAccumulator ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance ().remove_edge (_input, _action);
    Graph::instance ().remove_edge (_action, _result);

    delete _c_input;
    delete _action;
    delete _result;
    delete _clamp_max;
    delete _clamp_min;
    delete _input;
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

  void
  AdderAccumulator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adderaccumulator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_min", dynamic_cast<DoubleProperty*> (_clamp_min)->get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_max", dynamic_cast<DoubleProperty*> (_clamp_max)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}

