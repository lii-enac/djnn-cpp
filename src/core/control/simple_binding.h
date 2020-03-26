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
 *      Stephane Conversy <mathieu.stephane.conversy@enac.fr>
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
//#include "core/control/action.h"

namespace djnn {
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
    _c(src, ACTIVATION, dst, ACTIVATION) {}
    SimpleBinding (Process* parent, const std::string& name, Process* src, activation_flag_e src_flag, Process* dst, activation_flag_e dst_flag)
    : Process (name), //_action(this, "action", activate), _c_src(src, ACTIVATION, &_action, activate)
    _c(src, src_flag, dst, dst_flag) {}

    void impl_activate   () override { _c.enable  (); };
    void impl_deactivate () override { _c.disable (); }

    /*void perform_action (bool activate) {
      get_dst()->set_activation_source (get_src());
      if (activate)
        get_dst()->activate   ();
      else
        get_dst()->deactivate ();
    }*/
    
    //Process* get_src() { return _c_src.get_src(); } // delegate to coupling to save space
    //Process* get_dst() { return _dst; }

  private:
    //void set_parent (Process* p) override;

    //Process *_src
    //Process *_dst;
    //BindingAction _action;
    Coupling _c;

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };
}