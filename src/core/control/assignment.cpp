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

#include "../tree/bool_property.h"
#include "assignment.h"

#include "../tree/abstract_property.h"
#include "../tree/int_property.h"
#include "../tree/double_property.h"
#include "../tree/text_property.h"
#include "../tree/ref_property.h"
#include "../error.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  AbstractAssignment::AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                               bool isModel)
  {
    if (src == 0) {
      error ("src argument cannot be null in assignment creation");
    }
    if (dst == 0) {
      error ("dst argument cannot be null in assignment creation");
    }

    Process *f = src->find_component (ispec);
    if (f == 0) {
      error ("source not found in assignment creation");
    }
    _src = dynamic_cast<AbstractProperty*> (f);
    if (_src == nullptr) {
      warning ("the source of an assignment must be a property");
    }
    f = dst->find_component (dspec);
    if (f == 0) {
      error ("destination not found in assignment creation");
    }
    _dst = dynamic_cast<AbstractProperty*> (f);
    if (_dst == nullptr) {
      warning ("the destination of an assignment must be a property");
    }
  }

  void
  AbstractAssignment::do_assignment (AbstractProperty* src_p, AbstractProperty* dst_p, bool propagate)
  {
    switch (src_p->type ())
      {
      case Integer:
        {
          IntProperty* ip = dynamic_cast<IntProperty*> (src_p);
          dst_p->set_value (ip->get_value (), propagate);
          break;
        }
      case Boolean:
        {
          BoolProperty* bp = dynamic_cast<BoolProperty*> (src_p);
          dst_p->set_value (bp->get_value (), propagate);
          break;
        }
      case Double:
        {
          DoubleProperty* dp = dynamic_cast<DoubleProperty*> (src_p);
          dst_p->set_value (dp->get_value (), propagate);
          break;
        }
      case String:
        {
          TextProperty* tp = dynamic_cast<TextProperty*> (src_p);
          dst_p->set_value (string (tp->get_value ()), propagate);
          break;
        }
      case Reference:
        {
          RefProperty* rp = dynamic_cast<RefProperty*> (src_p);
          dst_p->set_value (rp->get_value (), propagate);
          break;
        }
      default:
        cout << "Unknown property type\n";
        return;
      }
  }

  Assignment::Assignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                          bool isModel) : 
      AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, true);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec,
                          Process* dst, const string &dspec, bool isModel) :
      Process (parent, name), AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, true);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  void
  Assignment::activate ()
  {
    /* here we have an activation issue, an assignment must do its action when activated, but
     * when the left side of an assignment is the result of an expression, we have to wait
     * for the evaluation of the expression. The solution is now to trigger the calculation
     * of the operators with an Activator component when they are on the left side of an assignment.
     */
    _action->activation ();
  }

  Assignment::~Assignment ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  
  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst,
                                      const string &dspec, bool isModel) :
    AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, false);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src,
                                      const string &ispec, Process* dst, const string &dspec, bool isModel) :
      Process (parent, name), AbstractAssignment (src, ispec, dst, dspec, isModel) 
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, false);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  void
  PausedAssignment::activate ()
  {
    /* here we have an activation issue, an assignment must do its action when activated, but
     * when the left side of an assignment is the result of an expression, we have to wait
     * for the evaluation of the expression. The solution is now to trigger the calculation
     * of the operators with an Activator component when they are on the left side of an assignment.
     */
    _action->activation ();
  }

  PausedAssignment::~PausedAssignment ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }
}


