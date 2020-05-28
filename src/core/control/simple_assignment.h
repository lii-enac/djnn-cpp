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

#define DJNN_SIMPLE_ASSIGNMENT_INCLUDED 1


namespace djnn { 

  // CoreAssignment does not follow the process/action model for memory minimization reason
  class CoreAssignment : public CoreProcess
  {
  public:
    CoreAssignment (CoreProcess* src, CoreProcess* dst, bool propagate)
    : _src(src), _dst(dst), _propagate(propagate)
    {
      Graph::instance ().add_edge (src, dst);
    }
    CoreAssignment (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool propagate)
    : CoreAssignment (src, dst, propagate)
    {
      finalize_construction (parent, name);
    }

    ~CoreAssignment () {
      Graph::instance ().remove_edge (get_src (), get_dst ());
    }

    CoreProcess* get_src () { return _src; }
    CoreProcess* get_dst () { return _dst; }

  protected:
    void impl_activate   () override { perform_action (); }
    void impl_deactivate () override {}
    void post_activate   () override { set_activation_state (DEACTIVATED); }

    void perform_action ();

  private:    
    CoreProcess *_src, *_dst;
    bool _propagate;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class CorePausedAssignment : public CoreAssignment
  {
  public:
    CorePausedAssignment (CoreProcess* src, CoreProcess* dst)
    : CoreAssignment (src, dst, false)
    {
    }
    CorePausedAssignment (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : CoreAssignment (parent, name, src, dst, false)
    {
    }
  };


  // SimpleAssignement follows the process/action model
  class SimpleAssignment : public FatProcess
  {
    friend class AssignmentAction;
  private:
    class AssignmentAction : public Action
    {
    public:
      AssignmentAction (FatProcess* parent, const std::string& name)
      : Action (parent, name) { finalize_construction (parent, name); }
      //void impl_activate () override { dynamic_cast<SimpleAssignment*>(get_parent ()) -> perform_action (); }
      void impl_activate () override { (static_cast<SimpleAssignment*>(get_parent ())) -> perform_action (); }
    };
  public:
    SimpleAssignment (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool propagate)
    :
    FatProcess (name),
     _dst(dst),
    _action(this, "action"), _c_src(src, ACTIVATION, &_action, ACTIVATION), _propagate(propagate)
    {
      Graph::instance ().add_edge (src, dst);
      finalize_construction (parent, name);
    }
    virtual ~SimpleAssignment () {
      Graph::instance ().remove_edge (get_src(), get_dst());
    }

    void impl_activate   () override { _c_src.enable  (); _action.activate(); }
    void impl_deactivate () override { _c_src.disable (); _action.deactivate(); }
    void post_activate   () override { post_activate_auto_deactivate (); }

    void perform_action ();

    CoreProcess* get_src() { return _c_src.get_src(); } // delegate to coupling to save space
    CoreProcess* get_dst() { return _dst; }

  private:
    CoreProcess *_dst;
    AssignmentAction _action;
    Coupling _c_src;
    bool _propagate;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class SimplePausedAssignment : public SimpleAssignment
  {
  public:
    SimplePausedAssignment (FatProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst)
    : SimpleAssignment (parent, name, src, dst, false)
    {}
  };

}