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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/control/action.h"
#include "core/tree/blank.h"

#include "exec_env/external_source.h"

namespace djnn {

  //class NativeAsyncAction;
  //typedef void (NativeCode) (CoreProcess*);
  typedef void (NativeCode) (CoreProcess*);
  
  class NativeAsyncAction : public Action, public ExternalSource
  {
  public:
    NativeAsyncAction (FatProcess* parent, const std::string& name, NativeCode *action, void* data, bool isModel);
    virtual ~NativeAsyncAction ();
    virtual process_type_e get_process_type () const override { return NATIVE_ACTION_T; }
    void impl_activate () override;
    void impl_deactivate () override;
    void* data ();

    virtual void set_activation_source (CoreProcess* src) override { _activation_source = src; }
    virtual CoreProcess* get_activation_source () override { return _activation_source; }
  
  protected:
    // ExternalSource
    virtual void run () override;

  private:
    void *_data;
    NativeCode *_action;
    CoreProcess *_activation_source;
    Blank _end;
  };
  void* get_native_async_user_data (Process* native_action);
}
