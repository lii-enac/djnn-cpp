/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

//#include "core/control/action.h"
#include "core/control/blank.h"
#include "core/control/native_action.h"
#include "core/control/spike.h"
#include "core/ontology/process.h"
#include "exec_env/external_source.h"
#include "base/fat_action.h"

namespace djnn {

class FatNativeAction : public FatAction {
public:
  FatNativeAction (CoreProcess* parent, const string& name, NativeCode* action, void* data, bool isModel);
  virtual ~FatNativeAction ();
  virtual process_type_e get_process_type () const override { return NATIVE_ACTION_T; }
  void                   impl_activate () override;
  void*                  data ();

  virtual void         set_activation_source (CoreProcess* src) override { _activation_source = src; }
  virtual CoreProcess* get_activation_source () override { return _activation_source; }

protected:
  void*        _data;
  NativeCode*  _action;
  CoreProcess* _activation_source;
};
void* get_fat_native_user_data (CoreProcess* native_action);


class NativeAsyncAction : public FatNativeAction, public ExternalSource {
  public:
    NativeAsyncAction (CoreProcess* parent, const CoreProcess::string& name, NativeCode action, void* data, bool isModel);
    virtual ~NativeAsyncAction ();
    virtual process_type_e get_process_type () const override { return NATIVE_ACTION_T; }
    void                   impl_activate () override;
    void                   impl_deactivate () override;

    virtual void         set_activation_source (CoreProcess* src) override { _activation_source = src; }
    virtual CoreProcess* get_activation_source () override { return _activation_source; }

  protected:
    // ExternalSource
    virtual void run () override;

    void post_activate () override {
        // contrarily to Action and NativeAction, do not auto deactivate
        // post_activate_auto_deactivate ();
        CoreProcess::post_activate ();
    }

  private:
    class RestartAction : public Action {
      public:
        RestartAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {};
        virtual ~RestartAction () {}
        void impl_activate () override { ((NativeAsyncAction*)get_parent ())->restart (); }
    };

    void restart ();
    friend class RestartAction;

  private:
    CoreProcess*  _activation_source;
    Blank         _end;
    Spike         _restart;
    RestartAction _action;
    Coupling      _c_restart;
};
} // namespace djnn
