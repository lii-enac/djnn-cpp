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
#include "core/control/action.h"
#include "core/execution/graph.h"

namespace djnn { 

  class SimpleAssignment : public Process
  {
    friend class AssignmentAction;
  private:
    class AssignmentAction : public Action
    {
    public:
      AssignmentAction (Process* parent, const std::string& name)
      : Action(parent, name) { finalize_construction(parent, name); }
      //void impl_activate () override { dynamic_cast<SimpleAssignment*>(get_parent ()) -> perform_action (); }
      void impl_activate () override { (static_cast<SimpleAssignment*>(get_parent ())) -> perform_action (); }
    };
  public:
    SimpleAssignment (Process* parent, const std::string& name, Process* src, Process* dst, bool propagate)
    : Process (name), _src(src), _dst(dst), _action(this, "action"), _c_src(src, ACTIVATION, &_action, ACTIVATION), _propagate(propagate)
    {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }
    virtual ~SimpleAssignment () {
      Graph::instance ().remove_edge (get_src(), get_dst());
    }

    void impl_activate   () override { _c_src.enable  (); _action.activate(); };
    void impl_deactivate () override { _c_src.disable (); _action.deactivate(); }
    void post_activate   () override { Process::post_activate_auto_deactivate (); }

    void perform_action ();

    Process* get_src() { return _src; } //_c_src.get_src(); } // delegate to coupling to save space
    Process* get_dst() { return _dst; }

  private:
    Process *_src;
    Process *_dst;
    AssignmentAction _action;
    Coupling _c_src;
    bool _propagate;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

}