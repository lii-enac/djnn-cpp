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

#include "core/ontology/process.h"
#include "core/control/action.h"

namespace djnn {
  using namespace std;

  typedef void (NativeCode) (Process*);
  class NativeAction : public Action
  {
  public:
    NativeAction (Process* parent, const string &name, NativeCode *action, void* data, bool isModel);
    virtual ~NativeAction ();
    void impl_activate () override;
    void impl_deactivate () override {}
    void* data ();

    virtual void set_activation_source (Process* src) override { _activation_source = src; }
    virtual Process* get_activation_source () override { return _activation_source; }

  private:
    void *_data;
    NativeCode *_action;
    Process *_activation_source;
  };
  void* get_native_user_data (Process* native_action);
}
