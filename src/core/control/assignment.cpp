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


#include "../tree/abstract_property.h"
#include "../tree/bool_property.h"
#include "../tree/int_property.h"
#include "../tree/double_property.h"
#include "../tree/text_property.h"
#include "../tree/ref_property.h"

#include "../execution/graph.h"

#include "../utils/error.h"
#include "../serializer/serializer.h"
#include "../utils/utils-dev.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  UpdateSrcOrDst::activate ()
  {
    Process* v = _prop->get_value ();
    if (!v) {
      *_to_update = nullptr;
      ((SrcToDstLink*) _parent)->update_graph ();
      return;
    }
    AbstractProperty *res = dynamic_cast<AbstractProperty*> (v->find_component (_spec));
    if (!res) {
      warning (this, "Source or destination in (Paused)assignment is not a property");
      * _to_update = nullptr;
      return;
    }
    *_to_update = res;
   ((SrcToDstLink*) _parent)->update_graph ();
  }

  void
  AbstractAssignment::init_AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec)
  {
    if (src == 0) {
      error (
          this,
          "SOURCE argument cannot be null in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec
              + ", dst spec:" + dspec + ")\n");
    }
    if (dst == 0) {
      error (
          this,
          "DESTINATION argument cannot be null in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec
              + ", dst spec:" + dspec + ")\n");
    }

    pair<RefProperty*, string> ref_src_pair = check_for_ref (src, ispec);
    if (ref_src_pair.first != nullptr) {
      _ref_src = ref_src_pair.first;
      _update_src = new UpdateSrcOrDst (this, "update_src_action", ref_src_pair.first, ref_src_pair.second, &_src);
      _update_src->activate ();
      _c_src = new Coupling (ref_src_pair.first, ACTIVATION, _update_src, ACTIVATION, true);
    } else {
      Process *f = src->find_component (ispec);
      if (f == 0) {
        error (
            this,
            "SOURCE not found in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec + ", dst spec:"
                + dspec + ")\n");
      }
      _src = f; //dynamic_cast<AbstractProperty*> (f);
      /*
       if (_src == nullptr) {
       warning (
       this,
       "the SOURCE of an (Paused)assignment must be a property ( name: " + _name + ", src spec: " + ispec
       + ", dst spec:" + dspec + ")\n");
       }
       */
    }
    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    if (ref_dst_pair.first != nullptr) {
      _ref_dst = ref_dst_pair.first;
      _update_dst = new UpdateSrcOrDst (this, "update_dst_action", ref_dst_pair.first, ref_dst_pair.second, (Process**)&_dst);
      _update_dst->activate ();
      _c_dst = new Coupling (ref_dst_pair.first, ACTIVATION, _update_dst, ACTIVATION);
      Graph::instance ().add_edge (ref_dst_pair.first, _update_dst);
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().add_edge (_parent->state_dependency (), _update_dst);
    } else {
      Process *f = dst->find_component (dspec);
      if (f == 0) {
        error (
            this,
            "DESTINATION not found in (Paused)assignment creation ( name: " + _name + ", src spec: " + ispec
                + ", dst spec:" + dspec + ")\n");
      }
      _dst = dynamic_cast<AbstractProperty*> (f);
      if (_dst == nullptr) {
        warning (
            this,
            "the DESTINATION of an (Paused)assignment must be a property ( name: " + _name + ", src spec: " + ispec
                + ", dst spec:" + dspec + ")\n");
      }
    }
  }

  void
  AbstractAssignment::update_graph ()
  {
  }

  AbstractAssignment::AbstractAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
					  bool isModel) :
      SrcToDstLink (isModel), _ref_src (nullptr), _ref_dst (nullptr), _update_src (nullptr), _update_dst (nullptr), _c_src (
	  nullptr), _c_dst (nullptr)
  {
    init_AbstractAssignment (src, ispec, dst, dspec);
  }

  AbstractAssignment::AbstractAssignment (Process *p, const string &n, Process* src, const string &ispec, Process* dst,
					  const string &dspec, bool isModel) :
      SrcToDstLink (p, n, isModel), _ref_src (nullptr), _ref_dst (nullptr), _update_src (nullptr), _update_dst (
	  nullptr), _c_src (nullptr), _c_dst (nullptr)
  {
    init_AbstractAssignment (src, ispec, dst, dspec);
  }

  AbstractAssignment::~AbstractAssignment ()
  {
    if (_update_src) {
      delete _c_src;
      delete _update_src;
    }
    if (_update_dst) {
      delete _c_dst;
      delete _update_dst;
    }
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
    string src_name = _src ? _src->get_name () : "";
    string dst_name = _dst ? _dst->get_name () : "";
    _action = new AssignmentAction (this, "assignment_" + src_name + "_to_" + dst_name + "_action", &_src, &_dst, true);
    if (_src && _dst) {
      Graph::instance ().add_edge (_src, _dst);
      _has_coupling = true;
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().add_edge (_parent->state_dependency (), _dst);
    }
  }

  Assignment::Assignment (Process* src, const string &ispec, Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    init_Assignment ();
  }

  Assignment::Assignment (Process* parent, const string &name, Process* src, const string &ispec, Process* dst,
                          const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    init_Assignment ();
    Process::finalize_construction ();
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

    delete _action;
  }

  void
  Assignment::serialize (const string& format)
  {

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:assignment");
    AbstractSerializer::serializer->text_attribute ("id", _name);
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
    _action = new AssignmentAction (this, "pausedAssignment_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false);
    Graph::instance ().add_edge (_src, _dst);
    if (_src && _dst) {
      if (_parent && _parent->state_dependency () != nullptr)
        Graph::instance ().add_edge (_parent->state_dependency (), _dst);
    }
  }

  PausedAssignment::PausedAssignment (Process* src, const string &ispec, Process* dst, const string &dspec,
                                      bool isModel) :
      AbstractAssignment (src, ispec, dst, dspec, isModel)
  {
    init_PausedAssignment ();
  }

  PausedAssignment::PausedAssignment (Process* parent, const string &name, Process* src, const string &ispec,
                                      Process* dst, const string &dspec, bool isModel) :
      AbstractAssignment (parent, name, src, ispec, dst, dspec, isModel)
  {
    init_PausedAssignment ();
    Process::finalize_construction ();
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

    delete _action;
  }

  void
  PausedAssignment::serialize (const string& format)
  {

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:pausedassignment");
    AbstractSerializer::serializer->text_attribute ("id", _name);
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

