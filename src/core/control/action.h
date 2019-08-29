
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

#include "../ontology/process.h"
#include "../execution/graph.h"

namespace djnn {

  class Action : public Process {
  public:
    Action (bool model = false) : Process (model) {}
    Action (Process *parent, const std::string &n, bool model = false) : Process (n, model) 
    {
      /* note:
       * finaliaze_constructor for action 
       * they add a symbol process but do not appear in _children Component
       * so they has to be delete manualy from destructor
       */ 
       if (parent) {
        _state_dependency = parent->state_dependency ();
        Process::set_parent (parent);
      }
    }
    virtual ~Action ();

  protected:
    virtual bool pre_activate () override {
      if (_parent != 0 && !_parent->somehow_activating () )
        return false;
      set_activation_state(ACTIVATING);
      return true;
    }
    void post_activate () override {
      notify_activation ();
      set_activation_state (DEACTIVATED);
    }
  };
}
