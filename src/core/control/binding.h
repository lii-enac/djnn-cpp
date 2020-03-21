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

  class Binding : public SrcToDstLink
  {
    friend class BindingAction;
  private:
    class BindingAction : public Action
    {
    public:
      BindingAction (Process* parent, const std::string& name, bool activate);
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
    Binding (Process* parent, const std::string& name, Process* src, const std::string&  ispec, Process* dst, const std::string&  dspec,
      std::string src_ref_spec = std::string(), std::string dst_ref_spec = std::string()); // hack to create temporary string objects for init of refs
    Binding (Process* parent, const std::string& name, Process* src, const std::string&  ispec, bool on_activation, Process* dst, const std::string&  dspec, bool activate,
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
    void serialize (const std::string& format) override;

    Process* get_src() { return _src; }
    Process* get_dst() { return _dst; }

  private:
    void set_parent (Process* p) override;

    struct Init { Init(Binding *, Process* src, const std::string&  ispec, Process* dst, const std::string&  dspec, std::string& src_ref_spec, std::string& dst_ref_spec); };
    friend struct Init;
    void check_init(const std::string& ispec, const std::string& dspec);

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    Process *_src, *_dst;
    ref_update _ref_update_src, _ref_update_dst;
    BindingAction _action;
    Coupling _c_src;
    
    bool _has_coupling;
    bool has_coupling () const { return _has_coupling; }
  };


  // this is what could a SimpleBinding ressemble, which a much smaller size: TODO use it in smala!
  class SimpleBinding : public Process
  {
    /*
    friend class BindingAction;
  private:
    class BindingAction : public Action
    {
    public:
      BindingAction (Process* parent, const std::string& name) //, bool activate);
      : Action(parent, name) { finalize_construction(parent, name); }
      virtual ~BindingAction () {};
      //void impl_activate () override { ((SimpleBinding*)get_parent ()) -> perform_action (get_binding_action()); }
      void impl_activate () override { dynamic_cast<SimpleBinding*>(get_parent> ()) -> perform_action (); }

    private: 
      //bool get_binding_action ()           { return get_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT); }
      //void set_binding_action (bool VALUE) {        set_bitset (BINDING_ACTION_MASK, BINDING_ACTION_SHIFT, VALUE); }
    };*/
  public:
    SimpleBinding (Process* parent, const std::string& name, Process* src, Process* dst)
    : Process (name), //_action(this, "action", true), /*_c_src(src, ACTIVATION, &_action, ACTIVATION)
    _c_src(src, ACTIVATION, dst, ACTIVATION) {}
    SimpleBinding (Process* parent, const std::string& name, Process* src, activation_flag_e src_flag, Process* dst, activation_flag_e dst_flag)
    : Process (name), //_action(this, "action", activate), _c_src(src, ACTIVATION, &_action, activate)
    _c_src(src, src_flag, dst, dst_flag) {}

    void impl_activate   () override { _c_src.enable  (); };
    void impl_deactivate () override { _c_src.disable (); }

    /*void perform_action (bool activate) {
      get_dst()->set_activation_source (get_src());
      if (activate)
        get_dst()->activate   ();
      else
        get_dst()->deactivate ();
    }*/
    
    void serialize (const std::string& format) override;

    //Process* get_src() { return _c_src.get_src(); } // delegate to coupling to save space
    //Process* get_dst() { return _dst; }

  private:
    void set_parent (Process* p) override;

    //Process *_src
    //Process *_dst;
    //BindingAction _action;
    Coupling _c_src;
  };
}
