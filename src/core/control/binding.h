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
#include "../control/action.h"
#include "../control/assignment.h"

namespace djnn {
  using namespace std;

  class Binding : public SrcToDstLink
  {
    friend class BindingAction;
  private:
    class BindingAction : public Action
    {
    public:
      BindingAction (Process* parent, const string &name, bool activate);
      virtual ~BindingAction () {};
      void impl_activate () override {
      	((Binding*)_parent)->_dst->set_activation_source (((Binding*)_parent)->_src);
      	if (get_binding_action()) {
      	  ((Binding*)_parent)->_dst->activate ();
      	}
      	else
      	  ((Binding*)_parent)->_dst->deactivate ();
      }
      void impl_deactivate () override {}

    private:
      
      bool get_binding_action ()           { return get_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT); }
      void set_binding_action (bool VALUE) {        set_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT, VALUE); }

    };
  public:
    Binding (Process* parent, const string &name, Process* src, const string & ispec, Process* dst, const string & dspec);
    Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool activate);
    Binding (Process* src, const string & ispec, Process* dst, const string & dspec);
    virtual ~Binding ();
    void impl_activate () override { if (_c_src) _c_src->enable(); };
    void impl_deactivate () override { if (_c_src) _c_src->disable (); }
    void update_graph () override;
    void serialize (const string& format) override;
  private:
    void set_parent (Process* p) override;
    void init_binding (Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool to_activate);
    Process *_src, *_dst;
    RefProperty *_ref_src, *_ref_dst;
    Coupling *_c_src;
    BindingAction *_action;
    UpdateSrcOrDst* _update_src, *_update_dst;
    Coupling *_c_update_src, *_c_update_dst;
    bool _has_coupling;
  };
}
