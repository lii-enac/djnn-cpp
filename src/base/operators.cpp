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

#include "core/control/action.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"


namespace djnn
{
  using namespace std;

  void
  init_binary_couplings (FatProcess& _left, FatProcess& _right, FatProcess& _result, Action& _action, Coupling& _c_left, Coupling& _c_right)
  {
    _c_left.disable ();
    _c_right.disable ();
    graph_add_edge (&_left, &_action);
    graph_add_edge (&_right, &_action);
    graph_add_edge (&_action, &_result);
  }

  void
  uninit_binary_couplings (FatProcess* this_, FatProcess& _left, FatProcess& _right, FatProcess& _result, Action& _action, Coupling& _c_left, Coupling& _c_right)
  {
    remove_state_dependency (this_->get_parent (), &_action);
    graph_remove_edge (&_left, &_action);
    graph_remove_edge (&_right, &_action);
    graph_remove_edge (&_action, &_result);
  }

  void
  init_unary_couplings (FatProcess& _input, FatProcess& _output, Action& _action, Coupling& _coupling)
  {
    _coupling.disable ();
    graph_add_edge (&_input, &_action);
    graph_add_edge (&_action, &_output);
  }

  void
  uninit_unary_couplings (FatProcess* this_, FatProcess& _input, FatProcess& _output, Action& _action, Coupling& _coupling)
  {
    remove_state_dependency (this_->get_parent (), &_action);
    graph_remove_edge (&_input, &_action);
    graph_remove_edge (&_action, &_output);
  }

} /* namespace djnn */
