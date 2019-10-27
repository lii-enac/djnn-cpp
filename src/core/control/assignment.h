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
#include "core/control/src_to_dst_link.h"

namespace djnn {

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

    ref_info _ref_info_src, _ref_info_dst;
    Init _init; // will be "created" third
    Process* _src;
    AbstractProperty* _dst;
    ref_update _ref_update_src, _ref_update_dst;
    

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
