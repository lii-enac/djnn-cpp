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

#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/execution/graph.h"
#include "core/serializer/serializer.h"


namespace djnn
{
  using namespace std;

  void
  init_binary_couplings (Process& _left, Process& _right, Process& _result, Process& _action, Coupling& _c_left, Coupling& _c_right)
  {
    _c_left.disable ();
    _c_right.disable ();
    Graph::instance ().add_edge (&_left, &_action);
    Graph::instance ().add_edge (&_right, &_action);
    Graph::instance ().add_edge (&_action, &_result);
  }

  void
  uninit_binary_couplings (Process* this_, Process& _left, Process& _right, Process& _result, Process& _action, Coupling& _c_left, Coupling& _c_right)
  {
    remove_state_dependency (this_->get_parent (), &_action);
    Graph::instance ().remove_edge (&_left, &_action);
    Graph::instance ().remove_edge (&_right, &_action);
    Graph::instance ().remove_edge (&_action, &_result);
  }

  void
  init_unary_couplings (Process& _input, Process& _output, Process& _action, Coupling& _coupling)
  {
    _coupling.disable ();
    Graph::instance ().add_edge (&_input, &_action);
    Graph::instance ().add_edge (&_action, &_output);
  }

  void
  uninit_unary_couplings (Process* this_, Process& _input, Process& _output, Process& _action, Coupling& _coupling)
  {
    remove_state_dependency (this_->get_parent (), &_action);
    Graph::instance ().remove_edge (&_input, &_action);
    Graph::instance ().remove_edge (&_action, &_output);
  }

} /* namespace djnn */
