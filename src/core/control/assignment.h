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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/ref_property.h"

namespace djnn {

  class UpdateSrcOrDst : public Action
  {
    public:
      UpdateSrcOrDst (Process* p, const string &n, RefProperty* prop, const string &spec, Process **to_update);
      UpdateSrcOrDst () {} // needed for pointer-less zombie initialization
      
      virtual ~UpdateSrcOrDst () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      Process** _to_update;
      RefProperty* _prop;
      string _spec;
  };

  class SrcToDstLink : public Process {
  public:
    SrcToDstLink (bool is_model = false) : Process (is_model) {}
    SrcToDstLink (Process* p, const string &n, bool is_model = false) : Process (n, is_model) {}
    virtual ~SrcToDstLink () {}
    virtual void update_graph () = 0;
  };

  class AbstractAssignment : public SrcToDstLink {
   friend class Assignment;
   friend class PausedAssignment;
   /* PRIVATE CLASS */
  private:
   
   class AssignmentAction : public Action
   {
   public:
    AssignmentAction (Process* p, const string &n, Process** src, AbstractProperty** dst, bool propagate) : Action (p, n), _src (src), _dst (dst), _propagate (propagate) {}
    virtual ~AssignmentAction () {}
    void impl_activate () override { if (_src && _dst) AbstractAssignment::do_assignment(*_src, *_dst, _propagate); };
    void impl_deactivate () override {}
    void set_src (Process** src) { _src = src; }
    void set_dst (AbstractProperty** dst) { _dst = dst; }
  private:
    Process** _src;
    AbstractProperty** _dst;
    bool _propagate;
  };
    
  public:
    AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    AbstractAssignment (Process* p, const string &n, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void update_graph () override;
    virtual ~AbstractAssignment ();
    static void do_assignment (Process* src, AbstractProperty* dst, bool propagate);
  
  protected:
    void set_parent (Process* p) override;

    struct Init { Init(AbstractAssignment *, const string& name, Process* src, const string &ispec, Process* dst, const string &dspec); };
    friend struct Init;
    void check_init(const string& ispec, const string& dspec);

    struct ref_info {
      ref_info() : _ref(nullptr) {}
      bool is_ref() const { return _ref != nullptr; }
      RefProperty * _ref;
      string _name;
    };
    struct ref_update {
      ref_update() {}
      ref_update(Process *p, const ref_info& ri, Process* to_update) :
        _update(p, "update_src_action", ri._ref, ri._name, &to_update),
        _c(ri._ref, ACTIVATION, &_update, ACTIVATION, true)
        { _update.impl_activate(); }
      UpdateSrcOrDst _update;
      Coupling _c;
    };

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    ref_update _ref_update_src, _ref_update_dst;
    Process* _src;
    AbstractProperty* _dst;

    bool _has_coupling;
  };

  class Assignment : public AbstractAssignment {
  private:
    void init_Assignment ();
  public:
    Assignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    Assignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void impl_activate () override;
    void post_activate () override { set_activation_state (DEACTIVATED); }
    void impl_deactivate () override {}
    void serialize (const string& format) override;
    virtual ~Assignment ();

  private:
    AssignmentAction _action;
  };

  class PausedAssignment : public AbstractAssignment {
  private:
    void init_PausedAssignment ();
  public:
    PausedAssignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    PausedAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void impl_activate () override;
    void post_activate () override { set_activation_state (DEACTIVATED); }
    void impl_deactivate () override {}
    void serialize (const string& format) override;
    virtual ~PausedAssignment ();

  private:
    AssignmentAction _action;
  };

}
