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

#include "native_expression_action.h"

namespace djnn
{
  using namespace std;

  NativeExpressionAction::NativeExpressionAction (NativeExpression *action, std::map<std::string, AbstractProperty*>& data, bool string_setter, bool isModel) :
      Action (), _data (data), _action (action), _string_setter (string_setter)
  {
    set_is_model (isModel);
  }

  NativeExpressionAction::NativeExpressionAction (Process* parent, const string &name, NativeExpression *action, std::map<std::string, AbstractProperty*>& data, bool string_setter,
                              bool isModel) :
      Action (parent, name), _data (data), _action (action), _string_setter (string_setter)
  {
    set_is_model (isModel);
    Process::finalize ();
  }

  NativeExpressionAction::~NativeExpressionAction ()
  {
  }

  void
  NativeExpressionAction::activate ()
  {
    (_action) (_data, _string_setter);
  }

  std::map<std::string, AbstractProperty*>&
  NativeExpressionAction::data ()
  {
    return _data;
  }
}
