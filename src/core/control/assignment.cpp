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

#if !defined(DJNN_NO_DEBUG)
#include <iostream>
#endif

namespace djnn
{
  using namespace std;

  SimpleAssignment::~SimpleAssignment () {}

  void
  SimpleAssignment::perform_action () {
      get_dst()->set_activation_source (get_src());
      bool propagate = _propagate;
      AbstractProperty *src_p = dynamic_cast<AbstractProperty*> (get_src()); // FIXME should be done once and for all
      if(!src_p) { warning (src_p, "not a property"); return; }
      AbstractProperty *dst_p = dynamic_cast<AbstractProperty*> (get_dst());
      if(!dst_p) { warning (dst_p, "not a property"); return; }
      switch (src_p->get_prop_type ())
        {
        case Integer:
          {
            AbstractIntProperty* ip = dynamic_cast<AbstractIntProperty*> (src_p);
            if (ip) dst_p->set_value (ip->get_value (), propagate);
            break;
          }
        case Boolean:
          {
            AbstractBoolProperty* bp = dynamic_cast<AbstractBoolProperty*> (src_p);
            if (bp) dst_p->set_value (bp->get_value (), propagate);
            break;
          }
        case Double:
          {
            AbstractDoubleProperty* dp = dynamic_cast<AbstractDoubleProperty*> (src_p);
            if (dp) dst_p->set_value (dp->get_value (), propagate);
            break;
          }
        case String:
          {
            AbstractTextProperty* tp = dynamic_cast<AbstractTextProperty*> (src_p);
            if (tp) dst_p->set_value (string (tp->get_value ()), propagate);
            break;
          }
        case Reference:
          {
            RefProperty* rp = dynamic_cast<RefProperty*> (src_p);
            if (rp) dst_p->set_value (rp->get_value (), propagate);
            break;
          }
        default:
          warning (src_p, "Unknown property type\n");
          return;
        }
      get_dst()->activate ();
  }

  #if !defined(DJNN_NO_SERIALIZE)
  void
  SimpleAssignment::serialize (const std::string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:pausedassignment");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), get_src (), buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), get_dst (), buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

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
  AbstractAssignment::check_init(const std::string& ispec, const std::string& dspec)
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

  AbstractAssignment::Init::Init (AbstractAssignment *p, const std::string& name, Process* src, const std::string& ispec, Process* dst, const std::string& dspec,
    std::string src_ref_spec, std::string dst_ref_spec)
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
    //p->_ref_info_src._spec = ref_src_pair.second;
    src_ref_spec = ref_src_pair.second;

    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    p->_ref_info_dst._ref = ref_dst_pair.first;
    //p->_ref_info_dst._spec = ref_dst_pair.second;
    dst_ref_spec = ref_dst_pair.second;
  }

  AbstractAssignment::AbstractAssignment (Process *parent, const std::string& name, Process* src, const std::string& ispec, Process* dst, const std::string& dspec, bool isModel,
    std::string src_ref_spec, std::string dst_ref_spec)
  :
  SrcToDstLink (parent, name, isModel),
  _init(this, name, src, ispec, dst, dspec, src_ref_spec, dst_ref_spec),
  _src(!_ref_info_src.is_ref() && src ? src->find_child (ispec) : nullptr),
  _dst(!_ref_info_dst.is_ref() && dst ? dynamic_cast<AbstractProperty*>(dst->find_child (dspec)) : nullptr),
  _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, src_ref_spec, (Process**)&_src) : ref_update()),
  _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, dst_ref_spec, (Process**)&_dst) : ref_update())
  {
    check_init(ispec, dspec);
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
            if (ip) dst_p->set_value (ip->get_value (), propagate);
            break;
          }
        case Boolean:
          {
            AbstractBoolProperty* bp = dynamic_cast<AbstractBoolProperty*> (src_p);
            if (bp) dst_p->set_value (bp->get_value (), propagate);
            break;
          }
        case Double:
          {
            AbstractDoubleProperty* dp = dynamic_cast<AbstractDoubleProperty*> (src_p);
            if (dp) dst_p->set_value (dp->get_value (), propagate);
            break;
          }
        case String:
          {
            AbstractTextProperty* tp = dynamic_cast<AbstractTextProperty*> (src_p);
            if (tp) dst_p->set_value (string (tp->get_value ()), propagate);
            break;
          }
        case Reference:
          {
            RefProperty* rp = dynamic_cast<RefProperty*> (src_p);
            if (rp) dst_p->set_value (rp->get_value (), propagate);
            break;
          }
        default:
          warning (src, "Unknown property type");
          return;
        }
    } else if (dst_p->get_prop_type () == Reference) {
      ((RefProperty*) dst_p)->set_value (src, propagate);
    } else {
      warning (nullptr, "incompatible source and destination in (Paused)assignment \n");
    }
  }

  void
  Assignment::update_graph ()
  {
    /* remove edge has been made in about_to_update_graph */
    if (_src && _dst) {
      //cerr "Assignment::"<< __FUNCTION__ << " add src/dst edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().add_edge (_src, _dst);
    }
  }

  void 
  Assignment::about_to_update_graph ()
  {
    if (_src && _dst) {
      //cerr << endl << "Assignment::"<< __FUNCTION__ <<  " remove old src/dst edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().remove_edge (_src, _dst);
    }
  }

  void
  Assignment::init_Assignment ()
  {
    if (_src && _dst)
      Graph::instance ().add_edge (_src, _dst);
  }

  Assignment::Assignment (Process* parent, const std::string& name, Process* src, const std::string& ispec, Process* dst,
                          const std::string& dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel),
      _action (this, "assignment_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", &_src, &_dst, true)
  {
    init_Assignment ();
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
    Process::finalize_construction (parent, name);
  }

  void
  Assignment::impl_activate ()
  {
    //FIXME : 
    /* when an assignment is in a deactivate fsm branch and src/dst has changed. it is not aware of it.
       we have to re-evaluate them */
    /* BUT : calling update on scr and dst each time make update_graph twice (so add egde and remove edge for nothing) */
    if (_ref_info_src._ref)
      _ref_update_src._update.impl_activate ();

    if (_ref_info_dst._ref)
      _ref_update_dst._update.impl_activate ();

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

#if !defined(DJNN_NO_SERIALIZE)
  void
  Assignment::serialize (const std::string& format)
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
#endif

  void
  PausedAssignment::init_PausedAssignment ()
  {
    Graph::instance ().add_edge (_src, _dst);
    if (_src && _dst) {
      add_state_dependency (get_parent (), _dst);
    }
  }

  PausedAssignment::PausedAssignment (Process* parent, const std::string& name, Process* src, const std::string& ispec,
                                      Process* dst, const std::string& dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel),
      _action (this, "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false)
  {
    init_PausedAssignment ();
    Process::finalize_construction (parent, name);
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

#if !defined(DJNN_NO_SERIALIZE)
  void
  PausedAssignment::serialize (const std::string& format)
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
#endif

}

