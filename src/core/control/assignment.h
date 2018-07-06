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


  class AbstractAssignment {

   friend class Assignment;
   friend class PausedAssignment;
   /* PRIVATE CLASS */
  private:
   class AssignmentAction : public Process
   {
   public:
    AssignmentAction (Process* p, const string &n, AbstractProperty* src, AbstractProperty* dst, bool propagate) : Process (p, n), _src (src), _dst (dst), _propagate (propagate) {}
    virtual ~AssignmentAction () {}
    void activate () override { AbstractAssignment::do_assignment(_src, _dst, _propagate); };
    void deactivate () override {}
  private:
    AbstractProperty* _src;
    AbstractProperty* _dst;
    bool _propagate;
  };

  public:
    AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    static void do_assignment (AbstractProperty* src, AbstractProperty* dst, bool propagate);
  protected:
    AbstractProperty* _src;
    AbstractProperty* _dst;
    unique_ptr<AssignmentAction> _action;
  };

  class Assignment : public Process, AbstractAssignment {
  public:
    Assignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    Assignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void activate ();
    void deactivate () { _action->deactivation (); }
    virtual ~Assignment (); 
  };


  class PausedAssignment : public Process, AbstractAssignment {
  public:
    PausedAssignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    PausedAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel);
    void activate ();
    void deactivate () { _action->deactivation (); }
    virtual ~PausedAssignment ();
  };

}
