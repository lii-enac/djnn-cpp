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
#include "core/control/blank.h"
#include "core/control/spike.h"
#include "core/control/native_action.h"

#include "exec_env/external_source.h"

namespace djnn {
  
  class NativeAsyncAction : public NativeAction, public ExternalSource
  {
  public:
    NativeAsyncAction (ParentProcess* parent, const string& name, NativeCode action, void* data, bool isModel);
    virtual ~NativeAsyncAction ();
    virtual process_type_e get_process_type () const override { return NATIVE_ACTION_T; }
    void impl_activate () override;
    void impl_deactivate () override;

    virtual void set_activation_source (CoreProcess* src) override { _activation_source = src; }
    virtual CoreProcess* get_activation_source () override { return _activation_source; }
  
  protected:
    // ExternalSource
    virtual void run () override;

    void post_activate () override {
      // contrarily to Action and NativeAction, do not auto deactivate
      //post_activate_auto_deactivate ();
      CoreProcess::post_activate ();
    }


  private:
    class RestartAction : public Action
    {
    public:
      RestartAction (ParentProcess* parent, const string& name) :
        Action (parent, name) {};
      virtual ~RestartAction () {}
      void impl_activate () override { ((NativeAsyncAction*)get_parent())->restart (); }
    };
  
  void restart ();
  friend class RestartAction;

  private:
    CoreProcess *_activation_source;
    Blank _end;
    Spike _restart;
    RestartAction _action;
    Coupling _c_restart;
  };
}
