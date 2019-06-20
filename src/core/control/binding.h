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
      BindingAction (Process* parent, const string &name, bool activate) : Action (parent, name) { set_binding_action (activate); }
      virtual ~BindingAction () {};
      void activate () override {
      	((Binding*)_parent)->_dst->set_activation_source (((Binding*)_parent)->_src);
      	if (get_binding_action()) {
      	  ((Binding*)_parent)->_dst->activation ();
      	}
      	else
      	  ((Binding*)_parent)->_dst->deactivation ();
      }
      void deactivate () override {}

    private:
      
      bool get_binding_action ()           { return get_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT); }
      void set_binding_action (bool VALUE) {        set_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT, VALUE); }

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
