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

  // abstract class for various combinations of assignment operand types 
  struct TTAssignment {
    virtual ~TTAssignment() {}
    virtual void perform_action (double propagate = true, double lazy = false) = 0;
    virtual CoreProcess* get_src () = 0;
    virtual CoreProcess* get_dst () = 0;
  };

  TTAssignment * build_ttassignment (CoreProcess* src, CoreProcess* dst);

  // CoreAssignment does not follow the process/action model for memory minimization reason
  class CoreAssignment : public CoreProcess
  {
  public:
    CoreAssignment (CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreProcess (is_model)
    //, _src(src), _dst(dst),
    , _propagate(true)
    {
      graph_add_edge (src, dst);
      _ttassignment = build_ttassignment (src, dst);
    }
    CoreAssignment (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreAssignment (src, dst, is_model)
    {
      finalize_construction (parent, name);
    }

    ~CoreAssignment () {
      graph_remove_edge (get_src (), get_dst ());
      delete _ttassignment;
      // if (get_parent ()) {
      //   remove_state_dependency (get_parent (), get_dst ());
      // }
    }

    void set_parent (ParentProcess* parent) override
    {
      /* in case of re-parenting remove edge dependency in graph */
      if (get_parent ()){
        remove_state_dependency (get_parent (), get_dst ());
      }
      add_state_dependency (parent, get_dst ());
      CoreProcess::set_parent (parent);
    }

    CoreProcess* get_src () { return _ttassignment->get_src (); }
    CoreProcess* get_dst () { return _ttassignment->get_dst (); }
    void perform_action ();
    
  protected:
    void impl_activate   () override { perform_action (); }
    void impl_deactivate () override {}
    void post_activate   () override { post_activate_auto_deactivate (); }

  private:    
    //CoreProcess *_src, *_dst;
    TTAssignment * _ttassignment;
  
  protected:
    bool _propagate;
    
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  void
    MultiAssignment (ParentProcess* parent, CoreProcess* src, vector <string> src_props, CoreProcess* dst, vector <string> dst_props, bool copy_on_activation);
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
    CorePausedAssignment (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : CoreAssignment (parent, name, src, dst, is_model)
    {
      _propagate = false;
    }
  };

  // Assignement follows the process/action model
  class Assignment : public FatProcess
  {
    friend class AssignmentAction;
  private:
    class AssignmentAction : public Action
    {
    public:
      AssignmentAction (ParentProcess* parent, const string& name)
      : Action (parent, name) { /*finalize_construction (parent, name);*/ }
      void impl_activate () override { (static_cast<Assignment*>(get_parent ())) -> perform_action (); }
    };
  public:
    Assignment (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : FatProcess (name, is_model)
    , _src(src)
    , _dst(dst)
    , _action(this, "action")
    , _ttassignment(nullptr) // FIXME
    , _propagate(true)
    , _lazy(false)
    {
      graph_add_edge (src, dst);
      finalize_construction (parent, name);
    }

    virtual ~Assignment ()
    {
      graph_remove_edge (get_src(), get_dst());
      if (get_parent ()) {
        remove_state_dependency (get_parent (), get_dst ());
      }
      delete _ttassignment;
    }

    void set_parent (ParentProcess* parent) override
    {
      /* in case of re-parenting remove edge dependency in graph */
      if (get_parent ()) {
        remove_state_dependency (get_parent (), get_dst ());
      }
      add_state_dependency (parent, get_dst ());
      FatProcess::set_parent (parent);
    }
    
    // for legacy reason, to get rid of?
    Assignment (ParentProcess* parent, const string& name,
                   CoreProcess* src, const string& sspec,
                   CoreProcess* dst, const string& dspec,
                   bool is_model=false)
    : Assignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}

  
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
    TTAssignment * _ttassignment;

  protected:
    bool _propagate;
    bool _lazy;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class PausedAssignment : public Assignment
  {
  public:
    PausedAssignment (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : Assignment (parent, name, src, dst, is_model)
    {
      _propagate = false;
    }

    // for legacy reason, to get rid of?
    PausedAssignment (ParentProcess* parent, const string& name,
                   CoreProcess* src, const string& sspec,
                   CoreProcess* dst, const string& dspec,
                   bool is_model=false)
    : PausedAssignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}
  };

  class LazyAssignment : public Assignment
  {
  public:
    LazyAssignment (ParentProcess* parent, const string& name, CoreProcess* src, CoreProcess* dst, bool is_model=false)
    : Assignment (parent, name, src, dst, is_model)
    {
      _lazy = true;
    }

    // for legacy reason, to get rid of?
    LazyAssignment (ParentProcess* parent, const string& name,
                   CoreProcess* src, const string& sspec,
                   CoreProcess* dst, const string& dspec,
                   bool is_model=false)
    : LazyAssignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}
  };

}
