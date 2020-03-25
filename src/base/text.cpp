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

#include "core/execution/graph.h"
#include "core/tree/text_property.h"
#include "core/serializer/serializer.h"

#include <iostream>

namespace djnn
{

  void
  TextPrinter::init ()
  {
    c_input.disable ();
  }

  TextPrinter::~TextPrinter ()
  {
  }

  void
  TextPrinter::TextPrinterAction::impl_activate ()
  {
    std::cout << _input->get_value () << std::endl;
  }

  TextPrinter::TextPrinter (Process *parent, const std::string& name)
  : Process (name),
    _input (this, "input", ""),
    _action (this, get_name () + "_action", &_input),
    c_input (&_input, ACTIVATION, &_action, ACTIVATION, true)
  {
    init ();
    Process::finalize_construction (parent, name);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  TextPrinter::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:textprinter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  template <> const char name_info<TextCatenatorAction>::left [] = "head";
  template <> const char name_info<TextCatenatorAction>::right [] = "tail";
  template <> const char name_info<TextCatenatorAction>::serialize [] = "textcatenator";

  template <> const char name_info<TextComparatorAction>::left [] = "left";
  template <> const char name_info<TextComparatorAction>::right [] = "right";
  template <> const char name_info<TextComparatorAction>::serialize [] = "textcomparator";

  void
  DoubleFormatter::init (double initial, int decimal)
  {
    _c_input.disable ();
    _c_decimal.disable ();
    Graph::instance ().add_edge (&_input, &_action);
    Graph::instance ().add_edge (&_decimal, &_action);
    Graph::instance ().add_edge (&_action, &_output);
  }

  DoubleFormatter::~DoubleFormatter ()
  {
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (&_input, &_action);
    Graph::instance ().remove_edge (&_decimal, &_action);
    Graph::instance ().remove_edge (&_action, &_output);
  }

  void
  DoubleFormatter::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    
    Process::set_parent (p); 
  }

  DoubleFormatter::DoubleFormatter (Process* parent, const std::string& name, double initial, int decimal) :
    Process (name),
    _input (this, "input", initial),
    _decimal (this, "decimal", decimal),
    _output (this, "output", ""),
    _action (this, "action", *this),
    _c_input (&_input, ACTIVATION, &_action, ACTIVATION),
    _c_decimal (&_decimal, ACTIVATION, &_action, ACTIVATION)
  {
    init (initial, decimal);
    Process::finalize_construction (parent, name);
  }

  void
  DoubleFormatter::impl_activate ()
  {
    _c_input.enable ();
    _c_decimal.enable ();
    _action.activate ();
  }

  void
  DoubleFormatter::impl_deactivate ()
  {
    _c_input.disable ();
    _c_decimal.disable ();
    _action.deactivate ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  DoubleFormatter::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:doubleformatter");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("initial", _input.get_value ());
    AbstractSerializer::serializer->int_attribute ("decimal", _decimal.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  TextAccumulator::TextAccumulator (Process *parent, const std::string& name, const std::string& init)
  : Process (name),
    _input (this, "input", ""),
    _state (this, "state", init),
    _del (this, "delete"),
    _acc_action (this, "acc_action", *this),
    _del_action (this, "del_action", *this),
    _c_acc (&_input, ACTIVATION, &_acc_action, ACTIVATION),
    _c_del (&_del, ACTIVATION, &_del_action, ACTIVATION)
  {
    _c_acc.disable ();
    _c_del.disable ();
    Graph::instance ().add_edge (&_input, &_acc_action);
    Graph::instance ().add_edge (&_del, &_del_action);
    Graph::instance ().add_edge (&_acc_action, &_state);
    Graph::instance ().add_edge (&_del_action, &_state);
    Process::finalize_construction (parent, name);
  }

  TextAccumulator::~TextAccumulator ()
  {
    remove_state_dependency (get_parent (), &_acc_action);
    remove_state_dependency (get_parent (), &_del_action);
    Graph::instance ().remove_edge (&_input, &_acc_action);
    Graph::instance ().remove_edge (&_del, &_del_action);
    Graph::instance ().remove_edge (&_acc_action, &_state);
    Graph::instance ().remove_edge (&_del_action, &_state);
  }

  void
  TextAccumulator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_acc_action);
       remove_state_dependency (get_parent (), &_del_action);
    }

    add_state_dependency (p, &_acc_action);
    add_state_dependency (p, &_del_action);
    
    Process::set_parent (p); 
  }

  void
  TextAccumulator::impl_activate ()
  {
    _c_acc.enable ();
    _c_del.enable ();
  }

  void
  TextAccumulator::impl_deactivate ()
  {
    _c_acc.disable ();
    _c_del.disable ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  TextAccumulator::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:text-accumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("initial", _state.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

}

