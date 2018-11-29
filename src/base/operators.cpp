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

#include "../core/tree/bool_property.h"
#include "../core/tree/int_property.h"
#include "../core/tree/double_property.h"
#include "../core/tree/text_property.h"
#include "../core/tree/ref_property.h"
#include "operators.h"

namespace djnn
{
  using namespace std;

  BinaryOperator::BinaryOperator (Process *p, const string &n) :
      Process (p, n), _left (nullptr), _right (nullptr), _result (nullptr), _c_left (nullptr), _c_right (
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
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);
  }

  BinaryOperator::~BinaryOperator ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance ().remove_edge (_left, _action);
    Graph::instance ().remove_edge (_right, _action);
    Graph::instance ().remove_edge (_action, _result);

    if (_c_right) { delete _c_right; _c_right=nullptr;}
    if (_c_left) { delete _c_left; _c_left=nullptr;}
    if (_action) { delete _action; _action=nullptr;}
    if (_left) { delete _left; _left=nullptr;}
    if (_right) { delete _right; _right=nullptr;}
    if (_result) { delete _result; _result=nullptr;} 
  }

  UnaryOperator::UnaryOperator (Process *p, const string &n) :
      Process (p, n), _input (nullptr), _output (), _c_input (nullptr), _action (nullptr)
  {
  }

  void
  UnaryOperator::init_couplings (Process* action)
  {
    _action = action;
    _c_input = new Coupling (_input, ACTIVATION, _action, ACTIVATION);
    _c_input->disable ();
    Graph::instance ().add_edge (_input, _action);
    Graph::instance ().add_edge (_action, _output);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);
  }

  UnaryOperator::~UnaryOperator ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance ().remove_edge (_input, _action);
    Graph::instance ().remove_edge (_action, _output);

    if (_c_input) { delete _c_input; _c_input=nullptr;}
    if (_action) { delete _action; _action=nullptr;}
    if (_input) { delete _input; _input=nullptr;}
    if (_output) { delete _output; _output=nullptr;}
  }
} /* namespace djnn */
