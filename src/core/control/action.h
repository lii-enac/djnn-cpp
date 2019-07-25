
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

#include <vector>

namespace djnn {

  class Action : public Process {
  public:
    Action (bool model = false) : Process (model), _src(nullptr) {}
    Action (Process *p, const std::string &n, bool model = false) : Process (p, n, model), _src(nullptr) {}
    virtual ~Action ();

    void add_native_edge (Process * src, Process * dst);

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

  private:
    Process *_src;
    std::vector<Process*> _dsts;
  };
}
