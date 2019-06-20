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

#pragma once

#include "../ontology/process.h"
#include "../control/action.h"
#include "../tree/abstract_property.h"

namespace djnn {
  using namespace std;

  typedef void (NativeExpression) (std::map<std::string, AbstractProperty*>&, bool string_setter);
  class NativeExpressionAction : public Action
  {
  public:
    NativeExpressionAction (Process* parent, const string &name, NativeExpression *action, std::map<std::string, AbstractProperty*>& data, bool string_setter, bool isModel);
    NativeExpressionAction (NativeExpression *action, std::map<std::string, AbstractProperty*>& data, bool string_setter, bool isModel);
    virtual ~NativeExpressionAction ();
    void impl_activate () override;
    void impl_deactivate () override {}
    std::map<std::string, AbstractProperty*>& data ();
  private:
    std::map<std::string, AbstractProperty*> _data;
    NativeExpression *_action;
    bool _string_setter;
  };
}
