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
#include "core/core-dev.h" // graph add/remove edge

//#include "core/utils/error.h"

namespace djnn { 

  // CoreAssignment does not follow the process/action model for memory minimization reason
  class CoreAssignment : public CoreProcess
  {
  public:
    CoreAssignment (CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreProcess (is_model)
    , _src(src), _dst(dst), _propagate(true)
    {
      graph_add_edge (src, dst);
    }
    CoreAssignment (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreAssignment (src, dst, is_model)
    {
      finalize_construction (parent, name);
    }

    ~CoreAssignment () {
      graph_remove_edge (get_src (), get_dst ());
    }

    CoreProcess* get_src () { return _src; }
    CoreProcess* get_dst () { return _dst; }
    void perform_action ();
    
  protected:
    void impl_activate   () override { perform_action (); }
    void impl_deactivate () override {}
    void post_activate   () override { post_activate_auto_deactivate (); }

  private:    
    CoreProcess *_src, *_dst;
  
  protected:
    bool _propagate;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  void
    MultiAssignment (ParentProcess* parent, CoreProcess* src, std::vector <std::string> src_props, CoreProcess* dst, std::vector <std::string> dst_props, bool copy_on_activation);
  void
    MultiAssignment (ParentProcess* parent, CoreProcess* src, CoreProcess* dst, bool copy_on_activation);

  class CorePausedAssignment : public CoreAssignment
  {
  public:
    CorePausedAssignment (CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreAssignment (src, dst, is_model)
    {
      _propagate = false;
    }
    CorePausedAssignment (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreAssignment (parent, name, src, dst, is_model)
    {
      _propagate = false;
    }
  };


  // SimpleAssignement follows the process/action model
  class Assignment : public FatProcess
  {
    friend class AssignmentAction;
  private:
    class AssignmentAction : public Action
    {
    public:
      AssignmentAction (ParentProcess* parent, const std::string& name)
      : Action (parent, name) { /*finalize_construction (parent, name);*/ }
      void impl_activate () override { (static_cast<Assignment*>(get_parent ())) -> perform_action (); }
    };
  public:
    Assignment (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : FatProcess (name, is_model)
    , _src(src)
    , _dst(dst)
    , _action(this, "action")
    , _propagate(true)
    {
      graph_add_edge (src, dst);
      finalize_construction (parent, name);
    }

    // for legacy reason, to get rid of?
    Assignment (ParentProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec,
                   bool is_model=false)
    : Assignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}

    virtual ~Assignment () {
      graph_remove_edge (get_src(), get_dst());
    }

    void impl_activate   () override { _action.activate(); }
    void impl_deactivate () override {} 
    void post_activate   () override { post_activate_auto_deactivate (); }

    void perform_action ();

    CoreProcess* get_src() { return _src; }
    CoreProcess* get_dst() { return _dst; }

  private:
    CoreProcess *_src;
    CoreProcess *_dst;
    AssignmentAction _action;
  protected:
    bool _propagate;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const std::string& format) override;
#endif
  };

  class PausedAssignment : public Assignment
  {
  public:
    PausedAssignment (ParentProcess* parent, const std::string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : Assignment (parent, name, src, dst, is_model)
    {
      _propagate = false;
    }

    // for legacy reason, to get rid of?
    PausedAssignment (ParentProcess* parent, const std::string& name,
                   CoreProcess* src, const std::string& sspec,
                   CoreProcess* dst, const std::string& dspec,
                   bool is_model=false)
    : PausedAssignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}
  };

}