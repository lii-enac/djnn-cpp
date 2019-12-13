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

#include "core/control/src_to_dst_link.h"
#include "core/ontology/coupling.h"

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
      	((Binding*)get_parent ())->_dst->set_activation_source (((Binding*)get_parent ())->_src);
      	if (get_binding_action()) {
      	  ((Binding*)get_parent ())->_dst->activate ();
      	}
      	else
      	  ((Binding*)get_parent ())->_dst->deactivate ();
      }
      void impl_deactivate () override {}

    private:
      
      bool get_binding_action ()           { return get_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT); }
      void set_binding_action (bool VALUE) {        set_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT, VALUE); }

    };
  public:
    Binding (Process* parent, const string &name, Process* src, const string & ispec, Process* dst,
      const string & dspec, string src_ref_spec = string(), string dst_ref_spec = string()); // hack to create temporary string objects for init of refs
    Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool activate,
      string src_ref_spec = string(), string dst_ref_spec = string()); // hack to create temporary string objects for init of refs
    virtual ~Binding ();
    void impl_activate () override { 
      if(_ref_info_src.is_ref())
        _ref_update_src._update.impl_activate ();
      _c_src.enable();
    };
    void impl_deactivate () override {
      _c_src.disable ();
    }
    void update_graph () override;
    void about_to_update_graph () override;
    void serialize (const string& format) override;
  private:
    void set_parent (Process* p) override;

    struct Init { Init(Binding *, Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec, bool to_activate,
      string& src_ref_spec, string& dst_ref_spec); };
    friend struct Init;
    void check_init(const string& ispec, const string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    Process *_src, *_dst;
    ref_update _ref_update_src, _ref_update_dst;
    BindingAction _action;
    Coupling _c_src;
    
    bool _has_coupling;    
  };
}
