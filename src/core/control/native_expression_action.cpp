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

  NativeExpressionAction::NativeExpressionAction (NativeExpression *action, std::map<std::string, AbstractProperty*> data, bool isModel) :
      Process (), _data (data), _action (action)
  {
    _model = isModel;
  }

  NativeExpressionAction::NativeExpressionAction (Process* parent, const string &name, NativeExpression *action, std::map<std::string, AbstractProperty*> data,
                              bool isModel) :
      Process (parent, name), _data (data), _action (action)
  {
    _model = isModel;
    Process::finalize ();
  }

  NativeExpressionAction::~NativeExpressionAction ()
  {
  }

  void
  NativeExpressionAction::activate ()
  {
    (_action) (_data);
  }

  std::map<std::string, AbstractProperty*>
  NativeExpressionAction::data ()
  {
    return _data;
  }
}
