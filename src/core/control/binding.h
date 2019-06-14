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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "../ontology/process.h"
#include "../ontology/coupling.h"

#include <string>

namespace djnn {
  using namespace std;

  class Binding : public Process
  {
    friend class BindingAction;
  private:
    class BindingAction : public Action
    {
    public:
      BindingAction (Process* parent, const string &name, bool activate) : Action (parent, name) { set_binding_activate (activate); }
      virtual ~BindingAction () {};
      void activate () override {
      	((Binding*)_parent)->_dst->set_activation_source (((Binding*)_parent)->_src);
      	if (is_binding_activate()) {
      	  ((Binding*)_parent)->_dst->activation ();
      	}
      	else
      	  ((Binding*)_parent)->_dst->deactivation ();
      }
      void deactivate () override {}
      void exec (int flag) override { activate (); }
    private:
      void set_binding_activate (bool activate) {
        if (activate)
          set_flag (BINDING_ACTION_MASK, BINDING_ACTION_ACTIVATE);
        else
          set_flag (BINDING_ACTION_MASK, BINDING_ACTION_DEACTIVATE);
      }
      bool is_binding_activate() { return is_flag_set (BINDING_ACTION_MASK, BINDING_ACTION_ACTIVATE); }
    };
  public:
    Binding (Process* parent, const string &name, Process* src, const string & ispec, Process* dst, const string & dspec);
    Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool activate);
    Binding (Process* src, const string & ispec, Process* dst, const string & dspec);
    virtual ~Binding ();
    void activate () override { _c_src->enable(); };
    void deactivate () override { _c_src->disable (); }
    void serialize (const string& format) override;
  private:
    void init_binding (Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool to_activate);
    Process *_src, *_dst;
    Coupling *_c_src;
    BindingAction *_action;
  };
}
