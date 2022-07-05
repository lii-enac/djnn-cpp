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

#include "core/property/template_property.h"

//#include "core/utils/error.h"

namespace djnn {

  // CoreAssignment does not follow the process/action model for memory minimization reason
  template <typename X>
  class TAbstractCoreAssignment : public CoreProcess
  {
  public:
    TAbstractCoreAssignment (X* src, X* dst, bool is_model=false)
    : CoreProcess (is_model), _src(src), _dst(dst)
    {
      graph_add_edge (src, dst);
      //_ttassignment = build_ttassignment (src, dst);
    }
    TAbstractCoreAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : CoreProcess (src, dst, is_model)
    {
      finalize_construction (parent, name);
    }

    virtual ~TAbstractCoreAssignment () {
      graph_remove_edge (get_src (), get_dst ());
      //delete _ttassignment;
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

    X* get_src () { return _src; /*_ttassignment->get_src ();*/ }
    X* get_dst () { return _dst; /*_ttassignment->get_dst ();*/ }
    virtual void perform_action () = 0;
    
  protected:
    void impl_activate   () override { perform_action (); }
    void impl_deactivate () override {}
    void post_activate   () override { post_activate_auto_deactivate (); }

    X *_src, *_dst;

    //TTAssignment * _ttassignment;
    
public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
  };

  template <typename X>
  class TCoreAssignment : public TAbstractCoreAssignment<X>
  {
  public:
    TCoreAssignment (X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (src, dst, is_model)
    {
    }
    TCoreAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (parent, name, src, dst, is_model)
    {
    }
    void perform_action () override {
      const auto & sv = TAbstractCoreAssignment<X>::get_src()->get_value ();
      const auto & dv = TAbstractCoreAssignment<X>::get_dst()->get_value ();
      //if (lazy && (sv == dv) ) return;
      TAbstractCoreAssignment<X>::get_dst()->set_value (sv, true);
    }
  };

  template <typename X>
  class TCorePausedAssignment : public TAbstractCoreAssignment<X>
  {
  public:
    TCorePausedAssignment (X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (src, dst, is_model)
    {
    }
    TCorePausedAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (parent, name, src, dst, is_model)
    {
    }
    void perform_action () override {
      const auto & sv = TAbstractCoreAssignment<X>::get_src()->get_value ();
      const auto & dv = TAbstractCoreAssignment<X>::get_dst()->get_value ();
      //if (lazy && (sv == dv) ) return;
      TAbstractCoreAssignment<X>::get_dst()->set_value (sv, false);
    }
  };

  template <typename X>
  class TCoreLazyAssignment : public TAbstractCoreAssignment<X>
  {
  public:
    TCoreLazyAssignment (X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (src, dst, is_model)
    {
    }
    TCoreLazyAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : TAbstractCoreAssignment<X> (parent, name, src, dst, is_model)
    {
    }
    void perform_action () override {
      const auto & sv = TAbstractCoreAssignment<X>::get_src()->get_value ();
      const auto & dv = TAbstractCoreAssignment<X>::get_dst()->get_value ();
      if (sv == dv) return;
      TAbstractCoreAssignment<X>::get_dst()->set_value (sv, true);
    }
  };

  // Assignement follows the process/action model
  template <typename X>
  class TAssignment : public FatProcess
  {
    friend class AssignmentAction;
  private:
    class AssignmentAction : public Action
    {
    public:
      AssignmentAction (ParentProcess* parent, const string& name)
      : Action (parent, name) { /*finalize_construction (parent, name);*/ }
      void impl_activate () override { (static_cast<TAssignment<X>*>(get_parent ())) -> perform_action (); }
    };
  public:
    TAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : FatProcess (name, is_model)
    , _src(src)
    , _dst(dst)
    , _action(this, "action")
    //, _ttassignment(nullptr) // FIXME
    , _propagate(true)
    , _lazy(false)
    {
      graph_add_edge (src, dst);
      finalize_construction (parent, name);
    }

    virtual ~TAssignment ()
    {
      graph_remove_edge (get_src(), get_dst());
      if (get_parent ()) {
        remove_state_dependency (get_parent (), get_dst ());
      }
      //delete _ttassignment;
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
    /*Assignment (ParentProcess* parent, const string& name,
                   CoreProcess* src, const string& sspec,
                   CoreProcess* dst, const string& dspec,
                   bool is_model=false)
    : Assignment (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}*/

  
    void impl_activate   () override { _action.activate(); }
    void impl_deactivate () override {} 
    void post_activate   () override { post_activate_auto_deactivate (); }

    void perform_action () {
      const auto & sv = _src->get_value ();
      const auto & dv = _dst->get_value ();
      if (_lazy) {
        if (sv == dv) return;
        _dst->set_value (sv, _propagate);
      } else {
        _dst->set_value (sv, _propagate);
      }
    }

    X* get_src() { return _src; }
    X* get_dst() { return _dst; }

  private:
    X *_src;
    X *_dst;
    AssignmentAction _action;
    //TTAssignment * _ttassignment;

  protected:
    bool _propagate;
    bool _lazy;

public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override {}
#endif
  };

  template <typename X>
  class TPausedAssignment : public TAssignment<X>
  {
  public:
    TPausedAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : TAssignment<X> (parent, name, src, dst, is_model)
    {
      TAssignment<X>::_propagate = false;
    }

    // for legacy reason, to get rid of?
    TPausedAssignment (ParentProcess* parent, const string& name,
                   X* src, const string& sspec,
                   X* dst, const string& dspec,
                   bool is_model=false)
    : TPausedAssignment<X> (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}
  };

  template <typename X>
  class TLazyAssignment : public TAssignment<X>
  {
  public:
    TLazyAssignment (ParentProcess* parent, const string& name, X* src, X* dst, bool is_model=false)
    : TAssignment<X> (parent, name, src, dst, is_model)
    {
      TAssignment<X>::_lazy = true;
    }

    // for legacy reason, to get rid of?
    TLazyAssignment (ParentProcess* parent, const string& name,
                   X* src, const string& sspec,
                   X* dst, const string& dspec,
                   bool is_model=false)
    : TLazyAssignment<X> (parent, name, src->find_child_impl (sspec), dst->find_child_impl (dspec), is_model)
    {}
  };

  template <typename X>
  void TMultiAssignment (ParentProcess* parent, X* src, vector <string> src_props, X* dst, vector <string> dst_props, bool copy_on_activation);
  
  template <typename X>
  void TMultiAssignment (ParentProcess* parent, X* src, X* dst, bool copy_on_activation);

}
