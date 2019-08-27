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

#include "text.h"

#include "../core/execution/graph.h"
#include "../core/tree/text_property.h"
#include "../core/serializer/serializer.h"

#include <iostream>

namespace djnn
{

  void
  TextPrinter::init ()
  {
    _input = new TextProperty (this, "input", "");
    _action = new TextPrinterAction (this, get_name () + "_action", _input);
    c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION, true);
    c_input->disable ();
  }

  TextPrinter::~TextPrinter ()
  {
    delete c_input;
    delete _action;
    delete _input;
  }

  void
  TextPrinter::TextPrinterAction::impl_activate ()
  {
    std::cout << _input->get_value () << std::endl;
  }

  TextPrinter::TextPrinter (Process *p, const string &n) :
      Process (p, n)
  {
    init ();
    Process::finalize_construction (p);
  }

  TextPrinter::TextPrinter ()
  {
    init ();
  }

  void
  TextPrinter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:textprinter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  TextCatenator::TextCatenator (Process *p, const string &n) :
      BinaryOperator (p, n)
  {
    _left = new TextProperty (this, "head", "");
    _right = new TextProperty (this, "tail", "");
    _result = new TextProperty (this, "output", "");
    init_couplings (new TextCatenatorAction (this, n + "_action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  TextCatenator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:textcatenator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  TextComparator::TextComparator (Process *p, const string &n, const string &left, const string &right) :
      BinaryOperator (p, n)
  {
    _left = new TextProperty (this, "left", left);
    _right = new TextProperty (this, "right", right);
    _result = new BoolProperty (this, "output", left.compare (right) == 0);
    init_couplings (new TextComparatorAction (this, n + "_action", _left, _right, _result));
    Process::finalize_construction (p);
  }

  void
  TextComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:textcomparator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("left", dynamic_cast<TextProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->text_attribute ("right", dynamic_cast<TextProperty*> (_right)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  DoubleFormatter::init (double initial, int decimal)
  {
    _input = new DoubleProperty (this, "input", initial);
    _decimal = new IntProperty (this, "decimal", decimal);
    _output = new TextProperty (this, "output", "");
    _action = new DoubleFormatterAction (this, "action", _input, _decimal, _output);
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    _c_input->disable ();
    _c_decimal = new Coupling (_decimal, ACTIVATION, _action, ACTIVATION);
    _c_decimal->disable ();
    Graph::instance ().add_edge (_input, _action);
    Graph::instance ().add_edge (_decimal, _action);
    Graph::instance ().add_edge (_action, _output);
  }

  DoubleFormatter::~DoubleFormatter ()
  {
    remove_state_dependency (_parent, _action);
    Graph::instance ().remove_edge (_input, _action);
    Graph::instance ().remove_edge (_decimal, _action);
    Graph::instance ().remove_edge (_action, _output);

    delete _c_decimal;
    delete _c_input;
    delete _action;
    delete _output;
    delete _decimal;
    delete _input;
  }

  void
  DoubleFormatter::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent) {
       remove_state_dependency (_parent, _action);
    }

    add_state_dependency (p, _action);
    
    _parent = p; 
  }

  DoubleFormatter::DoubleFormatter (Process* parent, const string &name, double initial, int decimal) :
      Process (parent, name)
  {
    init (initial, decimal);
    Process::finalize_construction (parent);
  }

  DoubleFormatter::DoubleFormatter (double initial, int decimal) :
      Process ()
  {
    init (initial, decimal);
  }

  void
  DoubleFormatter::impl_activate ()
  {
    _c_input->enable ();
    _c_decimal->enable ();
    _action->activate ();
  }

  void
  DoubleFormatter::impl_deactivate ()
  {
    _c_input->disable ();
    _c_decimal->disable ();
    _action->deactivate ();
  }

  void
  DoubleFormatter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:doubleformatter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("initial", dynamic_cast<DoubleProperty*> (_input)->get_value ());
    AbstractSerializer::serializer->int_attribute ("decimal", dynamic_cast<IntProperty*> (_decimal)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  TextAccumulator::TextAccumulator (Process *p, const string &n, const string &init) : Process (p, n)
  {
    _input = new TextProperty (this, "input", "");
    _state = new TextProperty (this, "state", init);
    _del = new Spike (this, "delete");
    _acc_action = new AccumulateAction (this, "acc_action", _input, _state);
    _del_action = new DeleteAction (this, "del_action", _state);
    _c_acc = new Coupling (_input, ACTIVATION, _acc_action, ACTIVATION);
    _c_acc->disable ();
    _c_del = new Coupling (_del, ACTIVATION, _del_action, ACTIVATION);
    _c_del->disable ();
    Graph::instance ().add_edge (_input, _acc_action);
    Graph::instance ().add_edge (_del, _del_action);
    Graph::instance ().add_edge (_acc_action, _state);
    Graph::instance ().add_edge (_del_action, _state);
    Process::finalize_construction (p);
  }

  TextAccumulator::~TextAccumulator () {    
    remove_state_dependency (_parent, _acc_action);
    remove_state_dependency (_parent, _del_action);
    Graph::instance ().remove_edge (_input, _acc_action);
    Graph::instance ().remove_edge (_del, _del_action);
    Graph::instance ().remove_edge (_acc_action, _state);
    Graph::instance ().remove_edge (_del_action, _state);
  
    delete _c_acc;
    delete _c_del;
    delete _acc_action;
    delete _del_action;
    delete _del;
    delete _state;
    delete _input;
  }

  void
  TextAccumulator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent) {
       remove_state_dependency (_parent, _acc_action);
       remove_state_dependency (_parent, _del_action);
    }

    add_state_dependency (p, _acc_action);
    add_state_dependency (p, _del_action);
    
    _parent = p; 
  }

  void
  TextAccumulator::impl_activate ()
  {
    _c_acc->enable ();
    _c_del->enable ();
  }

  void
  TextAccumulator::impl_deactivate ()
  {
    _c_acc->disable ();
    _c_del->disable ();
  }

  void
  TextAccumulator::serialize (const string &type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:text-accumulator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("initial", _state->get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
}

