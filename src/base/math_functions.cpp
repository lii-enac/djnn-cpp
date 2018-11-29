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

#include "math_functions.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  Exp::Exp (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", exp (i_val));
    init_couplings (new ExpAction (this, "action", _input, _output));
    Process::finalize ();
  }

  void
  Exp::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:exp");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Log::Log (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", log (i_val));
    init_couplings (new LogAction (this, "action", _input, _output));
    Process::finalize ();
  }

  void
  Log::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:log");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Log10::Log10 (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", log10 (i_val));
    init_couplings (new Log10Action (this, "action", _input, _output));
    Process::finalize ();
  }

  void
  Log10::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:log10");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Pow::Pow (Process *p, const string &n, double base, double exponent) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "base", base);
    _right = new DoubleProperty (this, "exponent", exponent);
    _result = new DoubleProperty (this, "result", pow (base, exponent));
    init_couplings (new PowAction (this, "action", _left, _right, _result));
    Process::finalize ();
  }

  void
  Pow::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:pow");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("base", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("exponent", dynamic_cast<DoubleProperty*> (_right)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Sqrt::Sqrt (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", sqrt (i_val));
    init_couplings (new SqrtAction (this, "action", _input, _output));
    Process::finalize ();
  }

  void
  Sqrt::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sqrt");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Abs::Abs (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = new DoubleProperty (this, "input", i_val);
    _output = new DoubleProperty (this, "output", sqrt (i_val));
    init_couplings (new AbsAction (this, "action", _input, _output));
    Process::finalize ();
  }

  void
  Abs::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:abs");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Min::Min (Process *p, const string &n, double min, double init_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "min", min);
    _right = new DoubleProperty (this, "input", init_val);
    _result = new DoubleProperty (this, "result", min < init_val ? init_val : min);
    init_couplings (new MinAction (this, "action", _left, _right, _result));
    Process::finalize ();
  }

  void
  Min::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:min");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("min", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_right)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Max::Max (Process *p, const string &n, double max, double init_val) :
      BinaryOperator (p, n)
  {
    _left = new DoubleProperty (this, "max", max);
    _right = new DoubleProperty (this, "input", init_val);
    _result = new DoubleProperty (this, "result", max >= init_val ? init_val : max);
    init_couplings (new MaxAction (this, "action", _left, _right, _result));
    Process::finalize ();
  }

  void
  Max::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:max");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("max", dynamic_cast<DoubleProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_right)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  BoundedValue::BoundedValue (Process *p, const string &n, double min, double max, double init_val) :
      Process (p, n)
  {
    _min = new DoubleProperty (this, "min", min);
    _max = new DoubleProperty (this, "max", max);
    _input = new DoubleProperty (this, "input", init_val);
    _result = new DoubleProperty (this, "result", init_val < min ? min : (init_val > max ? max : init_val));
    _action = new BoundedValueAction (this, "action", _min, _max,_input, _result);
    _c_min = new Coupling (_min, ACTIVATION, _action, ACTIVATION);
    _c_max = new Coupling (_max, ACTIVATION, _action, ACTIVATION);
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    Graph::instance ().add_edge (_min, _action);
    Graph::instance ().add_edge (_max, _action);
    Graph::instance ().add_edge (_input, _action);
    Graph::instance ().add_edge (_action, _result);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);
    Process::finalize ();
  }

  BoundedValue::~BoundedValue ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance ().remove_edge (_action, _result);
    Graph::instance ().remove_edge (_input, _action);
    Graph::instance ().remove_edge (_min, _action);
    Graph::instance ().remove_edge (_max, _action);
    
    if (_c_input) { delete _c_input; _c_input=nullptr;}
    if (_c_max) { delete _c_max; _c_max=nullptr;}
    if (_c_min) { delete _c_min; _c_min=nullptr;}
    if (_action) { delete _action; _action=nullptr;}
    if (_result) { delete _result; _result=nullptr;}
    if (_input) { delete _input; _input=nullptr;}
    if (_max) { delete _max; _max=nullptr;}
    if (_min) { delete _min; _min=nullptr;}
  }

  void
  BoundedValue::serialize (const string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:boundedvalue");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("min", dynamic_cast<DoubleProperty*> (_min)->get_value ());
    AbstractSerializer::serializer->int_attribute ("max", dynamic_cast<DoubleProperty*> (_max)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<DoubleProperty*> (_input)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}

