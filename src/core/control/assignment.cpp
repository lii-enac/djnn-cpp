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

  void
  AbstractAssignment::init_AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec) 
  {
    if (src == 0) {
        error (this, "SOURCE argument cannot be null in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }
      if (dst == 0) {
        error (this, "DESTINATION argument cannot be null in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }

      Process *f = src->find_component (ispec);
      if (f == 0) {
        error (this, "SOURCE not found in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }
      _src = dynamic_cast<AbstractProperty*> (f);
      if (_src == nullptr) {
        warning (this, "the SOURCE of an (Paused)assignment must be a property ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }
      f = dst->find_component (dspec);
      if (f == 0) {
        error (this, "DESTINATION not found in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }
      _dst = dynamic_cast<AbstractProperty*> (f);
      if (_dst == nullptr) {
        warning (this, "the DESTINATION of an (Paused)assignment must be a property ( name: " + _name + ", src spec: " + ispec + ", dst spec:" + dspec + ")\n");
      }
  }

  
  AbstractAssignment::AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                               bool isModel) : Process (isModel)
  {
    init_AbstractAssignment (src, ispec, dst, dspec);
  }

  AbstractAssignment::AbstractAssignment (Process *p, const string &n, Process* src, const string &ispec, Process* dst, const string &dspec,
                                 bool isModel) : Process (p, n, isModel)
  {
    init_AbstractAssignment (src, ispec, dst, dspec);   
  }

  void
  AbstractAssignment::do_assignment (AbstractProperty* src_p, AbstractProperty* dst_p, bool propagate)
  {
    switch (src_p->get_prop_type ())
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

  void
  Assignment::init_Assignment (){
    _action = new AssignmentAction (this, "assignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", _src, _dst, true);
    Graph::instance ().add_edge (_src, _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _dst);
  }

  Assignment::Assignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                          bool isModel) : 
      AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    init_Assignment ();
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec,
                          Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    init_Assignment ();
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
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
    
    if (_action) { delete _action; _action = nullptr;}
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

  void
  PausedAssignment::init_PausedAssignment ()
  {
    _action = new AssignmentAction (this, "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", _src, _dst, false);
    Graph::instance ().add_edge (_src, _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _dst);
  }

  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst,
                                      const string &dspec, bool isModel) :
    AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    init_PausedAssignment ();
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src,
                                      const string &ispec, Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    init_PausedAssignment ();
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
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
    
    if (_action) { delete _action; _action = nullptr;}
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


