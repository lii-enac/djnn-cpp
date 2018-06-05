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
 *
 */

#include "../tree/bool_property.h"
#include "assignment.h"

#include "../tree/abstract_property.h"
#include "../tree/int_property.h"
#include "../tree/double_property.h"
#include "../tree/text_property.h"
#include "../tree/ref_property.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  int
  Assignment::init_assignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                               bool isModel)
  {
    if (src == 0) {
      cerr << "src argument cannot be null in assignment creation\n";
      return 0;
    }
    if (dst == 0) {
      cerr << "dst argument cannot be null in assignment creation\n";
      return 0;
    }

    Process *f = src->find_component (ispec);
    if (f == 0) {
      cerr << "source not found in assignment creation\n";
      return 0;
    }
    _src = dynamic_cast<AbstractProperty*> (f);
    if (_src == nullptr) {
      cerr << "Warning: the source of an assignment must be a property\n";
      return 0;
    }
    f = dst->find_component (dspec);
    if (f == 0) {
      cerr << "destination not found in assignment creation\n";
      return 0;
    }
    _dst = dynamic_cast<AbstractProperty*> (f);
    if (_dst == nullptr) {
      cerr << "Warning: the destination of an assignment must be a property\n";
      return 0;
    }
    return 1;
  }

  Assignment::Assignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                          bool isModel)
  {
    _model = isModel;
    init_assignment (src, ispec, dst, dspec, isModel);
    _action = make_unique<AssignmentAction> (this,
                                             "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, true);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec,
                          Process* dst, const string &dspec, bool isModel) :
      Process (parent, name)
  {
    _model = isModel;
    init_assignment (src, ispec, dst, dspec, isModel);
    _action = make_unique<AssignmentAction> (this,
                                             "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, true);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action.get ());
    Process::finalize ();
  }

  Assignment::~Assignment ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
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

  void
  Assignment::do_assignment (AbstractProperty* src_p, AbstractProperty* dst_p, bool propagate)
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

  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst,
                                      const string &dspec, bool isModel) :
      Assignment (src, ispec, dst, dspec, isModel)
  {
    _action = make_unique<AssignmentAction> (this, "_action", _src, _dst, false);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src,
                                      const string &ispec, Process* dst, const string &dspec, bool isModel) :
      Assignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    _action = make_unique<AssignmentAction> (this, "_action", _src, _dst, false);
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (parent && parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (parent->state_dependency (), _action.get ());
  }

  PausedAssignment::~PausedAssignment ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }
}
