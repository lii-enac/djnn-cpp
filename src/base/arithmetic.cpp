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

#include "core/execution/graph.h"
#include "core/serializer/serializer.h"

namespace djnn
{

#if NEW_OP

  template <> const char name_info<std::plus<double>>::left[] = "left";
  template <> const char name_info<std::plus<double>>::right[] = "right";
  template <> const char name_info<std::plus<double>>::serialize[] = "adder";

  template <> const char name_info<std::minus<double>>::left[] = "left";
  template <> const char name_info<std::minus<double>>::right[] = "right";
  template <> const char name_info<std::minus<double>>::serialize[] = "subtractor";

  template <> const char name_info<std::multiplies<double>>::left[] = "left";
  template <> const char name_info<std::multiplies<double>>::right[] = "right";
  template <> const char name_info<std::multiplies<double>>::serialize[] = "multiplier";

  template <> const char name_info<std::divides<double>>::left[] = "left";
  template <> const char name_info<std::divides<double>>::right[] = "right";
  template <> const char name_info<std::divides<double>>::serialize[] = "divider";

  template <> const char name_info<std::modulus<int>>::left[] = "left";
  template <> const char name_info<std::modulus<int>>::right[] = "right";
  template <> const char name_info<std::modulus<int>>::serialize[] = "modulo";

  template <> const char name_info<std::greater<double>>::left[] = "left";
  template <> const char name_info<std::greater<double>>::right[] = "right";
  template <> const char name_info<std::greater<double>>::serialize[] = "ascendingcomparator";

  template <> const char name_info<std::greater_equal<double>>::left[] = "left";
  template <> const char name_info<std::greater_equal<double>>::right[] = "right";
  template <> const char name_info<std::greater_equal<double>>::serialize[] = "strictascendingcomparator";

  template <> const char name_info<std::equal_to<double>>::left[] = "left";
  template <> const char name_info<std::equal_to<double>>::right[] = "right";
  template <> const char name_info<std::equal_to<double>>::serialize[] = "equalitycomparator";

  template <> const char name_info<std::negate<double>>::left[] = "left";
  template <> const char name_info<std::negate<double>>::right[] = "right";
  template <> const char name_info<std::negate<double>>::serialize[] = "signinverter";

  
  NewPrevious::NewPrevious (Process *p, const string &n, double i_val)
  : Process (n),
    _input(this, "input", i_val),
    _output(this, "output", i_val),
    _action(this, "action", *this, i_val),
    _coupling(&_input, ACTIVATION, &_action, ACTIVATION)
  {
    init_unary_couplings(_input, _output, _action, _coupling);
    Process::finalize_construction (p);
  }

  void
  NewPrevious::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", _input.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  int
  NewIncr::init_incr (bool isModel)
  {
    Graph::instance ().add_edge (this, &_state);
    set_is_model (isModel);
    return 1;
  }

  NewIncr::NewIncr (bool isModel)
  : _delta (this, "delta", 1),
    _state (this, "state", 0)
  {
    init_incr (isModel);
  }

  NewIncr::NewIncr (Process *parent, const string& name, bool isModel) :
      Process (name),
      _delta (this, "delta", 1),
      _state (this, "state", 0)
  {
    init_incr (isModel);
    Process::finalize_construction (parent);
  }

  NewIncr::~NewIncr ()
  { 
    remove_state_dependency (get_parent (), &_state);
    Graph::instance ().remove_edge (this, &_state);
  }

  void
  NewIncr::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), &_state);
    }

    add_state_dependency (p, &_state);
    Process::set_parent (p); 
  }

  void
  NewIncr::impl_activate ()
  {
    _state.set_value (_state.get_value () + _delta.get_value (), true);
  }

  void
  NewIncr::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:incr");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  NewAdderAccumulator::NewAdderAccumulatorAction::NewAdderAccumulatorAction (Process* parent, const string &name,
                                                                    NewAdderAccumulator& aa) :
      Action (parent, name), _aa(aa)
  {
    Process::finalize_construction (parent);
  }

  void
  NewAdderAccumulator::NewAdderAccumulatorAction::impl_activate ()
  {
    double input = _aa._input.get_value ();
    double clamp_min = _aa._clamp_min.get_value ();
    double clamp_max = _aa._clamp_max.get_value ();
    double value = _aa._result.get_value ();
    value += input;
    if (clamp_min < clamp_max) {
      value = value < clamp_min ? clamp_min : value;
      value = value > clamp_max ? clamp_max : value;
    }
    _aa._result.set_value (value, true);
  }

  NewAdderAccumulator::NewAdderAccumulator (Process* parent, const string &name, double input, double clamp_min,
                                      double clamp_max)
  : Process (name),
    _input (this, "input", input),
    _clamp_min (this, "clamp_min", clamp_min),
    _clamp_max (this, "clamp_max", clamp_max),

    _result (this, "result", clamp_min >= clamp_max ? input : input < clamp_min ? clamp_min : input > clamp_max ? clamp_max : input),
    _action (this, name + "_action", *this),
    _c_input (&_input, ACTIVATION, &_action, ACTIVATION)
  {
    _c_input.disable ();
    Graph::instance ().add_edge (&_input, &_action);
    Graph::instance ().add_edge (&_action, &_result);
    Process::finalize_construction (parent);
  }

  NewAdderAccumulator::~NewAdderAccumulator ()
  {
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (&_input, &_action);
    Graph::instance ().remove_edge (&_action, &_result);
  }

  void
  NewAdderAccumulator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    Process::set_parent (p); 
  }

  void
  NewAdderAccumulator::impl_activate ()
  {
    _c_input.enable ();
  }

  void
  NewAdderAccumulator::impl_deactivate ()
  {
    _c_input.disable ();
  }

  void
  NewAdderAccumulator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adderaccumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", _input.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_min", _clamp_min.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_max", _clamp_max.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

#else

  OldAdder::OldAdder (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val + r_val);
    init_couplings (new AdderAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldAdder::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adder");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldSubtractor::OldSubtractor (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val - r_val);
    init_couplings (new SubtractorAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldSubtractor::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:subtractor");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldMultiplier::OldMultiplier (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", l_val * r_val);
    init_couplings (new MultiplierAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldMultiplier::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:multiplier");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldDivider::OldDivider (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new DoubleProperty (this, "result", 0);
    if (r_val != 0)
      _result->set_value (l_val / r_val, true);
    init_couplings (new DividerAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldDivider::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:divider");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldModulo::OldModulo (Process *p, const string &n, int l_val, int r_val) :
      BinaryOperator (p, n)
  {
    _left = new IntProperty (this, "left", l_val);
    _right = new IntProperty (this, "right", r_val);
    _result = new IntProperty (this, "result", 0);
    if ((int) r_val != 0)
      _result->set_value ((int) l_val % (int) r_val, true);
    init_couplings (new ModuloAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldModulo::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:modulo");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("left", dynamic_cast<IntProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("right",dynamic_cast<IntProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldAscendingComparator::OldAscendingComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val <= r_val);
    init_couplings (new AscendingComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldAscendingComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:ascendingcomparator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldStrictAscendingComparator::OldStrictAscendingComparator (Process *p, const string &n, double l_val,
                                                        double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val < r_val);
    init_couplings (new StrictAscendingComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldStrictAscendingComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:strictascendingcomparator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldEqualityComparator::OldEqualityComparator (Process *p, const string &n, double l_val, double r_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "left", l_val);
    _right = new DoubleProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val == r_val);
    init_couplings (new EqualityComparatorAction (this, "action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  OldEqualityComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:equalitycomparator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("left", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->float_attribute ("right", dynamic_cast<DoubleProperty*> (_right)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldSignInverter::OldSignInverter (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", -(i_val));
    init_couplings (new OldSignInverterAction (this, "action", _input, _output));
    Process::finalize_construction (p);
  }

  void
  OldSignInverter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:signinverter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldPrevious::OldPrevious (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", 0);
    init_couplings (new OldPreviousAction (this, "action", _input, _output, i_val));
    Process::finalize_construction (p);
  }

  void
  OldPrevious::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  int
  OldIncr::init_incr (bool isModel)
  {
    _delta = new DoubleProperty (this, "delta", 1);
    _state = new DoubleProperty (this, "state", 0);
    Graph::instance ().add_edge (this, _state);
    set_is_model (isModel);
    return 1;
  }

  OldIncr::OldIncr (bool isModel)
  {
    init_incr (isModel);
  }

  OldIncr::OldIncr (Process *parent, string name, bool isModel) :
      Process (name)
  {
    init_incr (isModel);
    Process::finalize_construction (parent);
  }

  OldIncr::~OldIncr ()
  { 
    remove_state_dependency (get_parent (), _state);
    Graph::instance ().remove_edge (this, _state);

    delete _state;
    delete _delta;
  }

  void
  OldIncr::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), _state);
    }

    add_state_dependency (p, _state);
    Process::set_parent (p); 
  }

  void
  OldIncr::impl_activate ()
  {
    _state->set_value (
        ((DoubleProperty*) _state)->get_value () + ((DoubleProperty*) _delta)->get_value (), true);
  }

  void
  OldIncr::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:incr");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  OldAdderAccumulator::OldAdderAccumulatorAction::OldAdderAccumulatorAction (Process* parent, const string &name,
                                                                    AbstractProperty* input,
                                                                    AbstractProperty* clamp_min,
                                                                    AbstractProperty* clamp_max,
                                                                    AbstractProperty* result) :
      Action (parent, name), _input (input), _clamp_min (clamp_min), _clamp_max (clamp_max), _result (result)
  {
    Process::finalize_construction (parent);
  }

  void
  OldAdderAccumulator::OldAdderAccumulatorAction::impl_activate ()
  {
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

  OldAdderAccumulator::OldAdderAccumulator (Process* parent, const string &name, double input, double clamp_min,
                                      double clamp_max) :
      Process (name)
  {
    _input = new DoubleProperty (this, "input", input);
    _clamp_min = new DoubleProperty (this, "clamp_min", clamp_min);
    _clamp_max = new DoubleProperty (this, "clamp_max", clamp_max);

    if (clamp_min < clamp_max) {
      input = input < clamp_min ? clamp_min : input;
      input = input > clamp_max ? clamp_max : input;
    }
    _result = new DoubleProperty (this, "result", input);
    _action = new OldAdderAccumulatorAction (this, name + "_action", _input, _clamp_min, _clamp_max, _result);
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    _c_input->disable ();
    Graph::instance ().add_edge (_input, _action);
    Graph::instance ().add_edge (_action, _result);
    Process::finalize_construction (parent);
  }

  OldAdderAccumulator::~OldAdderAccumulator ()
  {
    remove_state_dependency (get_parent (), _action);
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
  OldAdderAccumulator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), _action);
    }

    add_state_dependency (p, _action);
    Process::set_parent (p); 
  }

  void
  OldAdderAccumulator::impl_activate ()
  {
    _c_input->enable ();
  }

  void
  OldAdderAccumulator::impl_deactivate ()
  {
    _c_input->disable ();
  }

  void
  OldAdderAccumulator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adderaccumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_min", dynamic_cast<DoubleProperty*> (_clamp_min)->get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_max", dynamic_cast<DoubleProperty*> (_clamp_max)->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

#endif

}

