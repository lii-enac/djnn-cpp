/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "assignment.h"

#include "core/tree/abstract_property.h"
#include "core/tree/bool_property.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"

#include "core/execution/graph.h"

#include "core/utils/error.h"
#include "core/serializer/serializer.h"
#include "core/utils/utils-dev.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  AbstractAssignment::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      if (_ref_info_dst._ref)
        remove_state_dependency (get_parent (), &_ref_update_dst._update);
      if (_dst)
        remove_state_dependency (get_parent (), _dst);
    }

    if (_ref_info_dst._ref)
      add_state_dependency (p, &_ref_update_dst._update);
    if (_dst)
      add_state_dependency (p, _dst);

    Process::set_parent (p); 
  }

  void
  AbstractAssignment::update_graph ()
  {
  }

  void
  AbstractAssignment::check_init(const string& ispec, const string& dspec)
  {
    if(!_ref_info_src._ref) {
      if (_src == 0) {
        error (
            this,
            "SOURCE not found in (Paused)assignment creation ( name: " + get_name () + ", src spec: " + ispec + ", dst spec:"
                + dspec + ")\n");
      }
    }

    if(!_ref_info_dst._ref) {
      if (_dst == 0) {
        error (
            this,
            "DESTINATION not found in (Paused)assignment creation or the DESTINATION of an (Paused)assignment must be a property ( name: " + get_name () + ", src spec: " + ispec
                + ", dst spec:" + dspec + ")\n");
      }
    }
  }

  AbstractAssignment::Init::Init (AbstractAssignment *p, const string& name, Process* src, const string &ispec, Process* dst, const string &dspec)
  {
    if (src == 0) {
      error (
          p,
          "SOURCE argument cannot be null in (Paused)assignment creation ( name: " + name + ", src spec: " + ispec
              + ", dst spec:" + dspec + ")\n");
    }
    if (dst == 0) {
      error (
          p,
          "DESTINATION argument cannot be null in (Paused)assignment creation ( name: " + name + ", src spec: " + ispec
              + ", dst spec:" + dspec + ")\n");
    }
    pair<RefProperty*, string> ref_src_pair = check_for_ref (src, ispec);
    p->_ref_info_src._ref = ref_src_pair.first;
    p->_ref_info_src._name = ref_src_pair.second;

    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    p->_ref_info_dst._ref = ref_dst_pair.first;
    p->_ref_info_dst._name = ref_dst_pair.second;
  }

  AbstractAssignment::AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel)
  :
  SrcToDstLink (isModel),
  _init(this, "", src, ispec, dst, dspec),
  _src(!_ref_info_src.is_ref() && src ? src->find_component (ispec) : nullptr),
  _dst(!_ref_info_dst.is_ref() && dst ? dynamic_cast<AbstractProperty*>(dst->find_component (dspec)) : nullptr),
  _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, (Process**)&_src) : ref_update()), // uses copy constructor
  _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, (Process**)&_dst) : ref_update())
  {
    check_init(ispec, dspec);
    if(_ref_info_dst._ref) {
      Graph::instance ().add_edge (_ref_info_dst._ref, &_ref_update_dst._update);
    }
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
  }

  AbstractAssignment::AbstractAssignment (Process *p, const string &n, Process* src, const string &ispec, Process* dst,
					  const string &dspec, bool isModel)
  :
  SrcToDstLink (p, n, isModel),
  _init(this, n, src, ispec, dst, dspec),
  _src(!_ref_info_src.is_ref() && src ? src->find_component (ispec) : nullptr),
  _dst(!_ref_info_dst.is_ref() && dst ? dynamic_cast<AbstractProperty*>(dst->find_component (dspec)) : nullptr),
  _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, (Process**)&_src) : ref_update()),
  _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, (Process**)&_dst) : ref_update())
  {
    check_init(ispec, dspec);
    if(_ref_info_dst._ref) {
      Graph::instance ().add_edge (_ref_info_dst._ref, &_ref_update_dst._update);
    } 
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
  }

  AbstractAssignment::~AbstractAssignment ()
  {
  }

  void
  AbstractAssignment::do_assignment (Process* src, AbstractProperty* dst_p, bool propagate)
  {
    AbstractProperty *src_p = dynamic_cast<AbstractProperty*> (src);
    if (src_p) {
      switch (src_p->get_prop_type ())
        {
        case Integer:
          {
            AbstractIntProperty* ip = dynamic_cast<AbstractIntProperty*> (src_p);
            dst_p->set_value (ip->get_value (), propagate);
            break;
          }
        case Boolean:
          {
            AbstractBoolProperty* bp = dynamic_cast<AbstractBoolProperty*> (src_p);
            dst_p->set_value (bp->get_value (), propagate);
            break;
          }
        case Double:
          {
            AbstractDoubleProperty* dp = dynamic_cast<AbstractDoubleProperty*> (src_p);
            dst_p->set_value (dp->get_value (), propagate);
            break;
          }
        case String:
          {
            AbstractTextProperty* tp = dynamic_cast<AbstractTextProperty*> (src_p);
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
    } else if (dst_p->get_prop_type () == Reference) {
      ((RefProperty*) dst_p)->set_value (src, propagate);
    } else {
      warning (nullptr, "incompatible source and destination in (Paused)assignment \n");
    }
  }

  void
  Assignment::init_Assignment ()
  {
    if (_src && _dst) {
      Graph::instance ().add_edge (_src, _dst);
      _has_coupling = true;
    }
  }

  Assignment::Assignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (src, ispec, dst, dspec, isModel),
      _action (this, "assignment_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", &_src, &_dst, true)
  {
    init_Assignment ();
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst,
                          const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel),
      _action (this, "assignment_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", &_src, &_dst, true)
  {
    init_Assignment ();
    Process::finalize_construction (parent);
  }

  void
  Assignment::impl_activate ()
  {
    /* when an assignment is in a deactivate fsm branch and src has changed. it is not aware of it.
       we have to re-evaluate it */
    if (_ref_info_src._ref)
      _ref_update_src._update.impl_activate ();

    /* here we have an activation issue, an assignment must do its action when activated, but
     * when the left side of an assignment is the result of an expression, we have to wait
     * for the evaluation of the expression. The solution is now to trigger the calculation
     * of the operators with an Activator component when they are on the left side of an assignment.
     */
    _action.activate ();
  }

  Assignment::~Assignment ()
  {
    remove_state_dependency (get_parent (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
  }

  void
  Assignment::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:assignment");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  void
  PausedAssignment::init_PausedAssignment ()
  {
    Graph::instance ().add_edge (_src, _dst);
    if (_src && _dst) {
      add_state_dependency (get_parent (), _dst);
    }
  }

  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                                      bool isModel) :
      AbstractAssignment (src, ispec, dst, dspec, isModel),
      _action (this, "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false)
  {
    init_PausedAssignment ();
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src, const string &ispec,
                                      Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel),
      _action (this, "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false)
  {
    init_PausedAssignment ();
    Process::finalize_construction (parent);
  }

  void
  PausedAssignment::impl_activate ()
  {
    /* here we have an activation issue, an assignment must do its action when activated, but
     * when the left side of an assignment is the result of an expression, we have to wait
     * for the evaluation of the expression. The solution is now to trigger the calculation
     * of the operators with an Activator component when they are on the left side of an assignment.
     */
    _action.activate ();
  }

  PausedAssignment::~PausedAssignment ()
  {
    remove_state_dependency (get_parent (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
  }

  void
  PausedAssignment::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:pausedassignment");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
}

