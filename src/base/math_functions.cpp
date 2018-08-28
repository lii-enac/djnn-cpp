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
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", exp (i_val)));
    init_couplings (shared_ptr<Process> (new ExpAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  void
  Exp::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:exp");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Log::Log (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", log (i_val)));
    init_couplings (shared_ptr<Process> (new LogAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  void
  Log::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:log");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Log10::Log10 (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", log10 (i_val)));
    init_couplings (shared_ptr<Process> (new Log10Action (this, "action", _input, _output)));
    Process::finalize ();
  }

  void
  Log10::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:log10");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Pow::Pow (Process *p, const string &n, double base, double exponent) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new DoubleProperty (this, "base", base));
    _right = shared_ptr<AbstractProperty> (new DoubleProperty (this, "exponent", exponent));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", pow (base, exponent)));
    init_couplings (shared_ptr<Process> (new PowAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  void
  Pow::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:pow");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("base", std::dynamic_pointer_cast<DoubleProperty> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("exponent", std::dynamic_pointer_cast<DoubleProperty> (_right)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Sqrt::Sqrt (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", sqrt (i_val)));
    init_couplings (shared_ptr<Process> (new SqrtAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  void
  Sqrt::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:sqrt");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Abs::Abs (Process *p, const string &n, double i_val) :
      UnaryOperator (p, n)
  {
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", i_val));
    _output = shared_ptr<AbstractProperty> (new DoubleProperty (this, "output", sqrt (i_val)));
    init_couplings (shared_ptr<Process> (new AbsAction (this, "action", _input, _output)));
    Process::finalize ();
  }

  void
  Abs::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:abs");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Min::Min (Process *p, const string &n, double min, double init_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new DoubleProperty (this, "min", min));
    _right = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", init_val));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", min < init_val ? init_val : min));
    init_couplings (shared_ptr<Process> (new MinAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  void
  Min::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:min");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("min", std::dynamic_pointer_cast<DoubleProperty> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_right)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Max::Max (Process *p, const string &n, double max, double init_val) :
      BinaryOperator (p, n)
  {
    _left = shared_ptr<AbstractProperty> (new DoubleProperty (this, "max", max));
    _right = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", init_val));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", max >= init_val ? init_val : max));
    init_couplings (shared_ptr<Process> (new MaxAction (this, "action", _left, _right, _result)));
    Process::finalize ();
  }

  void
  Max::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:max");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("max", std::dynamic_pointer_cast<DoubleProperty> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_right)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  BoundedValue::BoundedValue (Process *p, const string &n, double min, double max, double init_val) :
      Process (p, n)
  {
    _min = shared_ptr<AbstractProperty> (new DoubleProperty (this, "min", min));
    _max = shared_ptr<AbstractProperty> (new DoubleProperty (this, "max", max));
    _input = shared_ptr<AbstractProperty> (new DoubleProperty (this, "input", init_val));
    _result = shared_ptr<AbstractProperty> (new DoubleProperty (this, "result", init_val < min ? min : (init_val > max ? max : init_val)));
    _action = shared_ptr<BoundedValueAction> (new BoundedValueAction (this, "action", _min, _max,_input, _result));
    _c_min = make_unique<Coupling> (_min.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_max = make_unique<Coupling> (_max.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_input = make_unique<Coupling> (_input.get (), ACTIVATION, _action.get (), ACTIVATION);
    Graph::instance ().add_edge (_min.get (), _action.get ());
    Graph::instance ().add_edge (_max.get (), _action.get ());
    Graph::instance ().add_edge (_input.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _result.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  BoundedValue::~BoundedValue ()
  {
    Graph::instance ().remove_edge (_min.get (), _action.get ());
    Graph::instance ().remove_edge (_max.get (), _action.get ());
    Graph::instance ().remove_edge (_input.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _result.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  BoundedValue::serialize (const string& type)
  {
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:boundedvalue");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("min", std::dynamic_pointer_cast<DoubleProperty> (_min)->get_value ());
    AbstractSerializer::serializer->int_attribute ("max", std::dynamic_pointer_cast<DoubleProperty> (_max)->get_value ());
    AbstractSerializer::serializer->int_attribute ("input", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}

