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

#include "operators.h"

#include "../core/tree/bool_property.h"
#include "../core/tree/int_property.h"
#include "../core/tree/double_property.h"
#include "../core/tree/text_property.h"
#include "../core/tree/ref_property.h"
#include "../core/execution/graph.h"


namespace djnn
{
  using namespace std;

  BinaryOperatorAction::BinaryOperatorAction (Process* parent, const string &name, 
      AbstractProperty* left, AbstractProperty* right, AbstractProperty* result) :
    Action (parent, name), _left (left), _right (right), _result (result)
  {
  }


  BinaryOperator::BinaryOperator (Process *p, const string &n) :
      Process (n), _left (nullptr), _right (nullptr), _result (nullptr), _c_left (nullptr), _c_right (
          nullptr), _action (nullptr)
  {
  }

  void
  BinaryOperator::init_couplings (Process* action)
  {
    _action = action;
    _c_left = new Coupling (_left, ACTIVATION, _action, ACTIVATION);
    _c_left->disable ();
    _c_right = new Coupling (_right, ACTIVATION, _action, ACTIVATION);
    _c_right->disable ();
    Graph::instance ().add_edge (_left, _action);
    Graph::instance ().add_edge (_right, _action);
    Graph::instance ().add_edge (_action, _result);
  }

  BinaryOperator::~BinaryOperator ()
  {
    remove_state_dependency (get_parent (), _action);
    Graph::instance ().remove_edge (_left, _action);
    Graph::instance ().remove_edge (_right, _action);
    Graph::instance ().remove_edge (_action, _result);

    delete _c_right;
    delete _c_left;
    delete _action;
    delete _left;
    delete _right;
    delete _result;
  }

  void
  BinaryOperator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), _action);
    }

    add_state_dependency (p, _action);
    
    Process::set_parent (p); 
  }

  UnaryOperatorAction::UnaryOperatorAction (Process* parent, const string &name, AbstractProperty* input, AbstractProperty* output) :
    Action (parent, name), _input (input), _output (output)
  {
  }

  UnaryOperator::UnaryOperator (Process *p, const string &n) :
      Process (n), _input (nullptr), _output (), _c_input (nullptr), _action (nullptr)
  {
  }

  void
  UnaryOperator::init_couplings (Process* action)
  {
    _action = action;
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION, true);
    _c_input->disable ();
    Graph::instance ().add_edge (_input, _output);
  }

  UnaryOperator::~UnaryOperator ()
  {
    Graph::instance ().remove_edge (_input, _output);

    delete _c_input;
    delete _action;
    delete _input;
    delete _output;
  }
} /* namespace djnn */
