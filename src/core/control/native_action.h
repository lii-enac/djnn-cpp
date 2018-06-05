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

  typedef void (NativeCode) (Process*);
  class NativeAction : public Process
  {
  public:
    NativeAction (Process* parent, const string &name, NativeCode *action, void* data, bool isModel);
    NativeAction (NativeCode *action, void* data, bool isModel);
    virtual ~NativeAction ();
    void activate () override;
    void deactivate () override {}
    void* data ();
  private:
    void *_data;
    NativeCode *_action;
  };
  void* get_native_user_data (Process* native_action);
}
