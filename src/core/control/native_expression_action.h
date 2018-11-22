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

#include "../tree/process.h"

#include <string>

namespace djnn {
  using namespace std;

  typedef void (NativeExpression) (std::map<std::string, AbstractProperty*>);
  class NativeExpressionAction : public Process
  {
  public:
    NativeExpressionAction (Process* parent, const string &name, NativeExpression *action, std::map<std::string, AbstractProperty*> data, bool isModel);
    NativeExpressionAction (NativeExpression *action, std::map<std::string, AbstractProperty*> data, bool isModel);
    virtual ~NativeExpressionAction ();
    void activate () override;
    void deactivate () override {}
    std::map<std::string, AbstractProperty*> data ();
  private:
    std::map<std::string, AbstractProperty*> _data;
    NativeExpression *_action;
  };
}
