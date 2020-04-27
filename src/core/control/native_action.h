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

  typedef Process ProcessForNative;
  typedef void (NativeCode) (ProcessForNative*);

  class NativeAction : public Action
  {
  public:
    NativeAction (Process* parent, const std::string& name, NativeCode *action, void* data, bool isModel);
    virtual ~NativeAction ();
    virtual process_type_e get_process_type () const override { return NATIVE_ACTION_T; }
    void impl_activate () override;
    void impl_deactivate () override {}
    void* data ();

    virtual void set_activation_source (CoreProcess* src) override { _activation_source = src; }
    virtual CoreProcess* get_activation_source () override { return _activation_source; }

  private:
    void *_data;
    NativeCode *_action;
    CoreProcess *_activation_source;
  };
  void* get_native_user_data (Process* native_action);
}
