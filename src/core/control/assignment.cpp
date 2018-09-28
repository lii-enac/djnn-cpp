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
#include "../serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  AbstractAssignment::AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                               bool isModel)
  {
    if (src == 0) {
      error (this, "src argument cannot be null in (Paused)assignment creation");
    }
    if (dst == 0) {
      error (this, "dst argument cannot be null in (Paused)assignment creation");
    }

    Process *f = src->find_component (ispec);
    if (f == 0) {
      error (this, "source not found in (Paused)assignment creation");
    }
    _src = dynamic_cast<AbstractProperty*> (f);
    if (_src == nullptr) {
      warning (this, "the source of an (Paused)assignment must be a property");
    }
    f = dst->find_component (dspec);
    if (f == 0) {
      error (this, "destination not found in (Paused)assignment creation");
    }
    _dst = dynamic_cast<AbstractProperty*> (f);
    if (_dst == nullptr) {
      warning (this, "the destination of an (Paused)assignment must be a property");
    }
  }

  AbstractAssignment::AbstractAssignment (Process *p, const string &n, Process* src, const string &ispec, Process* dst, const string &dspec,
                                 bool isModel) : Process (p, n)
    {
      if (src == 0) {
        error (this, "src argument cannot be null in (Paused)assignment creation");
      }
      if (dst == 0) {
        error (this, "dst argument cannot be null in (Paused)assignment creation");
      }

      Process *f = src->find_component (ispec);
      if (f == 0) {
        error (this, "source not found in (Paused)assignment creation");
      }
      _src = dynamic_cast<AbstractProperty*> (f);
      if (_src == nullptr) {
        warning (this, "the source of an (Paused)assignment must be a property");
      }
      f = dst->find_component (dspec);
      if (f == 0) {
        error (this, "destination not found in (Paused)assignment creation");
      }
      _dst = dynamic_cast<AbstractProperty*> (f);
      if (_dst == nullptr) {
        warning (this, "the destination of an (Paused)assignment must be a property");
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
    Graph::instance ().add_edge (_src, _dst);
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec,
                          Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, true);
    Graph::instance ().add_edge (_src, _dst);
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
    Graph::instance ().remove_edge (_src, _dst);
  }

  void
  Assignment::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:assignment");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", _model ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  
  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst,
                                      const string &dspec, bool isModel) :
    AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, false);
    Graph::instance ().add_edge (_src, _dst);
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src,
                                      const string &ispec, Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    _model = isModel;
    _action = make_unique<AbstractAssignment::AssignmentAction> (this,
                                             "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                             _src, _dst, false);
    Graph::instance ().add_edge (_src, _dst);
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
    Graph::instance ().remove_edge (_src, _dst);
  }

  void
  PausedAssignment::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:pausedassignment");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", _model ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}


