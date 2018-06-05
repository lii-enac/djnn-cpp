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
  BinaryOperator::init_couplings (shared_ptr<Process> action)
  {
    _action = action;
    _c_left = make_unique<Coupling> (_left.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_left->disable ();
    _c_right = make_unique<Coupling> (_right.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_right->disable ();
    Graph::instance ().add_edge (_left.get (), _action.get ());
    Graph::instance ().add_edge (_right.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _result.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
  }

  BinaryOperator::~BinaryOperator ()
  {
    Graph::instance ().remove_edge (_left.get (), _action.get ());
    Graph::instance ().remove_edge (_right.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _result.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  UnaryOperator::UnaryOperator (Process *p, const string &n) :
      Process (p, n), _input (nullptr), _output (), _c_input (nullptr), _action (nullptr)
  {
  }

  void
  UnaryOperator::init_couplings (shared_ptr<Process> action)
  {
    _action = action;
    _c_input = make_unique<Coupling> (_input.get (), ACTIVATION, _action.get (), ACTIVATION);
    _c_input->disable ();
    Graph::instance ().add_edge (_input.get (), _action.get ());
    Graph::instance ().add_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
  }

  UnaryOperator::~UnaryOperator ()
  {
    Graph::instance ().remove_edge (_input.get (), _action.get ());
    Graph::instance ().remove_edge (_action.get (), _output.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }
} /* namespace djnn */
