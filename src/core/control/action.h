
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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"

namespace djnn {

  class Action : public Process {
  public:
    Action (Process *parent, const std::string& n, bool model = false) : Process (n, model) 
    {
      /* note:
       * finalize_constructor for action 
       * they add a symbol process but do not appear in _children Component
       * so they has to be deleted manualy from destructor
       */ 
       if (parent) {
        set_state_dependency (parent->state_dependency ());
        Process::set_parent (parent);
      }
    }
    virtual ~Action ();
    virtual process_type_e get_process_type () const override { return ACTION_T; }
    void impl_deactivate () override {} // save from providing it in inherited actions

  protected:
    virtual bool pre_activate () override {
      if (get_parent () != 0 && !get_parent ()->somehow_activating () )
        return false;
      set_activation_state(ACTIVATING);
      return true;
    }
    void post_activate () override {
      Process::post_activate_auto_deactivate ();
    }
  };
}
