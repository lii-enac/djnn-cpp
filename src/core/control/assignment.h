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

#include "../tree/process.h"
#include "coupling.h"

namespace djnn {

  class SrcDstLink {
  public:
    SrcDstLink() {}
    virtual ~SrcDstLink () {}
    virtual void update_graph () = 0;
  };
  class UpdateSrcOrDst : public Process
  {
    public:
      UpdateSrcOrDst (Process* p, const string &n, RefProperty* prop, const string &spec, AbstractProperty **to_update) : Process (p, n), _to_update (to_update), _prop (prop), _spec (spec) {}
      virtual ~UpdateSrcOrDst () {}
      void activate () override;
      void deactivate () override {}
    private:
      AbstractProperty** _to_update;
      RefProperty* _prop;
      string _spec;
  };
  class AbstractAssignment : public Process, public SrcDstLink {

   friend class Assignment;
   friend class PausedAssignment;
   /* PRIVATE CLASS */
  private:
   class AssignmentAction : public Process
   {
   public:
    AssignmentAction (Process* p, const string &n, AbstractProperty** src, AbstractProperty** dst, bool propagate) : Process (p, n), _src (src), _dst (dst), _propagate (propagate) {}
    virtual ~AssignmentAction () {}
    void activate () override { if (_src && _dst) AbstractAssignment::do_assignment(*_src, *_dst, _propagate); };
    void deactivate () override {}
    void exec (int flag) override { activate (); }
    void set_src (AbstractProperty** src) { _src = src; }
    void set_dst (AbstractProperty** dst) { _dst = dst; }
  private:
    AbstractProperty** _src;
    AbstractProperty** _dst;
    bool _propagate;
  };

  private:
    void init_AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec);
  public:
    AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    AbstractAssignment (Process* p, const string &n, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void update_graph () override;
    virtual ~AbstractAssignment ();
    static void do_assignment (AbstractProperty* src, AbstractProperty* dst, bool propagate);
  protected:
    AbstractProperty* _src;
    AbstractProperty* _dst;
    RefProperty *_ref_src, *_ref_dst;
    AssignmentAction* _action;
    UpdateSrcOrDst* _update_src, *_update_dst;
    Coupling *_c_src, *_c_dst;
    bool _has_coupling;
  };

  class Assignment : public AbstractAssignment {
  private:
    void init_Assignment ();
  public:
    Assignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    Assignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void activate () override;
    void deactivate () override { _action->deactivation (); }
    void serialize (const string& format) override;
    virtual ~Assignment ();
  };


  class PausedAssignment : public AbstractAssignment {
  private:
    void init_PausedAssignment ();
  public:
    PausedAssignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    PausedAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void activate () override;
    void deactivate () override { _action->deactivation (); }
    void serialize (const string& format) override;
    virtual ~PausedAssignment ();
  };

}
