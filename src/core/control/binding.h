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

#include "core/ontology/coupling.h"
#include "core/control/src_to_dst_link.h"

namespace djnn {

  class Binding : public SrcToDstLink
  {
    friend class BindingAction;
  private:
    class BindingAction : public Action
    {
    public:
      BindingAction (ParentProcess* parent, const std::string& name, bool activate);
      virtual ~BindingAction () {};
      void impl_activate () override { ((Binding*)get_parent ()) -> perform_action (get_binding_action()); }
      /*void impl_activate () override {
      	((SimpleBinding*)get_parent ())->get_dst()->set_activation_source (((SimpleBinding*)get_parent ())->get_src());
        if (get_binding_action()) {
          ((SimpleBinding*)get_parent ())->get_dst()->activate ();
        }
        else
          ((SimpleBinding*)get_parent ())->get_dst()->deactivate ();
      }
      void impl_deactivate () override {}*/

    private:
      
      bool get_binding_action ()           { return get_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT); }
      void set_binding_action (bool VALUE) {        set_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT, VALUE); }

    };
  public:
    Binding (ParentProcess* parent, const std::string& name, FatProcess* src, const std::string&  ispec, FatProcess* dst, const std::string&  dspec,
      std::string src_ref_spec = std::string(), std::string dst_ref_spec = std::string()); // hack to create temporary string objects for init of refs
    Binding (ParentProcess* parent, const std::string& name, FatProcess* src, const std::string&  ispec, bool on_activation, FatProcess* dst, const std::string&  dspec, bool activate,
      std::string src_ref_spec = std::string(), std::string dst_ref_spec = std::string()); // hack to create temporary string objects for init of refs
    virtual ~Binding ();
    void impl_activate () override { 
      if(_ref_info_src.is_ref())
        _ref_update_src._update.impl_activate ();
      _c_src.enable();
    };
    void impl_deactivate () override {
      _c_src.disable ();
    }

    void perform_action (bool activate) {
      get_dst()->set_activation_source (get_src());
      if (activate)
        get_dst()->activate   ();
      else
        get_dst()->deactivate ();
    }

    void update_graph () override;
    void about_to_update_graph () override;

    FatProcess* get_src() { return _src; }
    FatProcess* get_dst() { return _dst; }

  private:
    void set_parent (ParentProcess* parent) override;

    struct Init { Init(Binding *, FatProcess* src, const std::string&  ispec, FatProcess* dst, const std::string&  dspec, std::string& src_ref_spec, std::string& dst_ref_spec); };
    friend struct Init;
    void check_init(const std::string& ispec, const std::string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    FatProcess *_src;
    FatProcess *_dst;
    ref_update _ref_update_src, _ref_update_dst;
    BindingAction _action;
    Coupling _c_src;
    
    bool _has_coupling;
    bool has_coupling () const { return _has_coupling; }

#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };


  
}
