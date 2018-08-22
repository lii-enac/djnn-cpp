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

#include "../core/tree/text_property.h"
#include "../core/serializer/serializer.h"
#include "text.h"

namespace djnn
{

  void
  TextPrinter::init ()
  {
    input = make_shared<TextProperty> (this, "input", "");
    _action = make_shared<TextPrinterAction> (this, get_name () + "_action", input);
    c_input = make_unique<Coupling> (input.get (), ACTIVATION, _action.get (), ACTIVATION);
    c_input->disable ();
    Graph::instance ().add_edge (input.get (), _action.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
  }

  TextPrinter::~TextPrinter ()
  {
    Graph::instance ().remove_edge (input.get (), _action.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  TextPrinter::TextPrinterAction::activate ()
  {
    std::cout << _input->get_value () << std::endl;
  }

  TextPrinter::TextPrinter (Process *p, const string &n) :
      Process (p, n)
  {
    init ();
    Process::finalize ();
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
    _left = make_shared<TextProperty> (this, "head", "");
    _right = make_shared<TextProperty> (this, "tail", "");
    _result = make_shared<TextProperty> (this, "output", "");
    init_couplings (make_shared<TextCatenatorAction> (this, n + "_action", _left, _right, _result));
    Process::finalize ();
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
    _left = make_shared<TextProperty> (this, "left", left);
    _right = make_shared<TextProperty> (this, "right", right);
    _result = make_shared<BoolProperty> (this, "output", left.compare (right) == 0);
    init_couplings (make_shared<TextComparatorAction> (this, n + "_action", _left, _right, _result));
    Process::finalize ();
  }

  void
  TextComparator::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:textcomparator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("left", std::dynamic_pointer_cast<TextProperty> (_left)->get_value ());
    AbstractSerializer::serializer->text_attribute ("right", std::dynamic_pointer_cast<TextProperty> (_right)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  DoubleFormatter::init (double initial, int decimal)
  {
    _input = make_shared<DoubleProperty> (this, "input", initial);
    _decimal = make_shared<IntProperty> (this, "decimal", decimal);
    _output = make_shared<TextProperty> (this, "output", "");
    _action = make_shared<DoubleFormatterAction> (this, "action", _input, _decimal, _output);
    _c_input = make_unique<Coupling> (_input.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_input.get ()->disable ();
    _c_decimal = make_unique<Coupling> (_decimal.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_decimal.get ()->disable ();
    Graph::instance ().add_edge (_input.get (), _action.get ());
    Graph::instance ().add_edge (_decimal.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
  }

  DoubleFormatter::~DoubleFormatter ()
  {
    Graph::instance ().remove_edge (_input.get (), _action.get ());
    Graph::instance ().remove_edge (_decimal.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }
  DoubleFormatter::DoubleFormatter (Process* parent, const string &name, double initial, int decimal) :
      Process (parent, name)
  {
    init (initial, decimal);
    Process::finalize ();
  }

  DoubleFormatter::DoubleFormatter (double initial, int decimal) :
      Process ()
  {
    init (initial, decimal);
  }

  void
  DoubleFormatter::activate ()
  {
    _c_input->enable ();
    _c_decimal->enable ();
    _action->activation ();
  }

  void
  DoubleFormatter::deactivate ()
  {
    _c_input->disable ();
    _c_decimal->disable ();
    _action->deactivation ();
  }

  void
  DoubleFormatter::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:doubleformatter");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("initial", std::dynamic_pointer_cast<DoubleProperty> (_input)->get_value ());
    AbstractSerializer::serializer->int_attribute ("decimal", std::dynamic_pointer_cast<IntProperty> (_decimal)->get_value ());
    
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}

