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

#include "connector.h"
#include "../core/execution/graph.h"
#include "../core/utils/error.h"
#include "../core/serializer/serializer.h"
#include "../core/utils/utils-dev.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Connector::ConnectorAction::ConnectorAction (Process* parent, const string &n, AbstractProperty** src, AbstractProperty** dst, bool propagate) :
        Action (parent, n), _src (src), _dst (dst), _propagate (propagate) 
  {
  }

  void
  Connector::ConnectorAction::impl_activate ()
  {
    /* do we have to check if the source is activable? */
    if (_parent->somehow_activating () && *_src && *_dst)
      AbstractAssignment::do_assignment (*_src, *_dst, _propagate);
  }

  Connector::Connector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec,
			bool copy_on_activation) :
      SrcToDstLink (p, n), _src (nullptr), _dst (nullptr), _ref_src (nullptr), _ref_dst (nullptr), _c_src (nullptr), _update_src (
	  nullptr), _update_dst (nullptr), _copy_on_activation (copy_on_activation)
  {
    init_connector (src, ispec, dst, dspec);
    Process::finalize_construction (p);
  }

  Connector::Connector (Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation) :
    _src (nullptr), _dst (nullptr), _ref_src (nullptr), _ref_dst (nullptr), _c_src (nullptr), _update_src (nullptr), _update_dst (nullptr), _copy_on_activation (copy_on_activation)
  {
    init_connector (src, ispec, dst, dspec);
  }

  void
  Connector::impl_activate ()
  {
    if (_c_src)
      _c_src->enable ();
    if (_copy_on_activation)
      _action->activate ();
  }

  void
  Connector::impl_deactivate ()
  {
    if (_c_src)
      _c_src->disable ();
    _action->deactivate ();
  }

  void
  Connector::init_connector (Process *src, string ispec, Process *dst, string dspec)
  {
    if (src == 0) {
      error (this,
             "src argument cannot be null in connector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    if (dst == 0) {
      error (this,
             "dst argument cannot be null in connector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    /* if src is a refProperty->$value */

    pair<RefProperty*, string> ref_src_pair = check_for_ref (src, ispec);
    if (ref_src_pair.first != nullptr) {
      _ref_src = ref_src_pair.first;
      _update_src = new UpdateSrcOrDst (this, "update_src_action", ref_src_pair.first, ref_src_pair.second, (Process**)&_src);

      _c_update_src = new Coupling (ref_src_pair.first, ACTIVATION, _update_src, ACTIVATION, true);
    } else {
      Process* c_src = src->find_component (ispec);
      if (c_src == 0) {
        error (this, "source not found in connector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
      _src = dynamic_cast<AbstractProperty*> (c_src);
      if (!_src)
        warning (this,
                 "invalid source (not a Property) in connector (" + get_name () + "," + ispec + ", " + dspec + ")");
    }

    /* if dst is a refProperty->$value */

    pair<RefProperty*, string> ref_dst_pair = check_for_ref (dst, dspec);
    if (ref_dst_pair.first != nullptr) {
      _ref_dst = ref_dst_pair.first;
      _update_dst = new UpdateSrcOrDst (this, "update_dst_action", ref_dst_pair.first, ref_dst_pair.second, (Process**)&_dst);

      _c_update_dst = new Coupling (ref_dst_pair.first, ACTIVATION, _update_dst, ACTIVATION, true);
    } else {
      Process* c_dst = dst->find_component (dspec);
      if (c_dst == 0) {
        error (this, "destination not found in connector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
      _dst = dynamic_cast<AbstractProperty*> (c_dst);
      if (!_dst)
        warning (
            this,
            "invalid destination (not a Property) in connector (" + get_name () + "," + ispec + ", " + dspec + ")");
    }

    string src_name = _src ? _src->get_name () : "";
    string dst_name = _dst ? _dst->get_name () : "";
    _action = new ConnectorAction (this, "connector_" + src_name + "_to_" + dst_name + "_action", &_src, &_dst,
                                   true);
    if (_src) {
      _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);
      _c_src->disable ();
      if (_dst) {
        Graph::instance ().add_edge (_src, _dst);
        _has_coupling = true;
      }
    }
    if (_update_src) _update_src->impl_activate ();
    if (_update_dst) _update_dst->impl_activate ();
  }

  void
  Connector::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent && _dst){
       remove_state_dependency (_parent, _dst);
    }

    if (_dst)
      add_state_dependency (p, _dst);

    _parent = p; 
  }

  void
  Connector::update_graph ()
  {
    if (_has_coupling) {
      delete _c_src;
      _c_src = nullptr;
    }
    if (_src && _dst) {
      _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);
      _has_coupling = true;
      if ( get_activation_state()==ACTIVATED ) {
        _action->activate ();
      } else {
        _c_src->disable ();
      }
    } else {
      _has_coupling = false;
    }
  }

  Connector::~Connector ()
  {
    remove_state_dependency (_parent, _dst);
    Graph::instance ().remove_edge (_src, _dst);

    delete _c_src;
    delete _action;
    if (_update_src) {
      delete _c_update_src;
      delete _update_src;
    }
    if (_update_dst) {
      delete _c_update_dst;
      delete _update_dst;
    }
  }

  void
  Connector::serialize (const string& format)
  {
    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:connector");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  PausedConnector::PausedConnector (Process *p, string n, Process *src, string ispec, Process *dst, string dspec,
                                    bool copy_on_activation) :
      Process (p, n), _c_src (nullptr), _copy_on_activation (copy_on_activation)
  {
    init_pausedconnector (src, ispec, dst, dspec);
    Process::finalize_construction (p);
  }

  PausedConnector::PausedConnector (Process *src, string ispec, Process *dst, string dspec, bool copy_on_activation) :
      _c_src (nullptr), _copy_on_activation (copy_on_activation)
  {
    init_pausedconnector (src, ispec, dst, dspec);
  }

  void
  PausedConnector::impl_activate ()
  {
    _c_src->enable ();
    if (_copy_on_activation)
      _action->activate ();
  }

  void
  PausedConnector::impl_deactivate ()
  {
    _c_src->disable ();
    _action->deactivate ();
  }

  void
  PausedConnector::init_pausedconnector (Process *src, string ispec, Process *dst, string dspec)
  {
    if (src == 0) {
      error ( this, "src argument cannot be null in pausedconnector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    if (dst == 0) {
      error ( this, "dst argument cannot be null in pausedconnector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_src = src->find_component (ispec);

    if (c_src == 0) {
      error (this, "source not found in pausedconnector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_dst = dst->find_component (dspec);

    if (c_dst == 0) {
      error (this,
       "destination not found in pausedconnector creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }

    _src = dynamic_cast<AbstractProperty*> (c_src);
    _dst = dynamic_cast<AbstractProperty*> (c_dst);
    if (!_src || !_dst) {
      warning (this,
       "invalid source or destination in pausedconnector (" + get_name () + "," + ispec + " " + dspec + ")");
    }

    _action = new Connector::ConnectorAction (
      this, "pausedconnector_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false);
    _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);

    Graph::instance ().add_edge (_src, _dst);
    _c_src->disable ();

  }

  PausedConnector::~PausedConnector ()
  {
    remove_state_dependency (_parent, _dst);
    Graph::instance ().remove_edge (_src, _dst);

    delete _c_src;
    delete _action;
  }

  void
  PausedConnector::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent){
       remove_state_dependency (_parent, _dst);
    }

    add_state_dependency (p, _dst);
    _parent = p; 
  }

  void
  PausedConnector::serialize (const string& format)
  {

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:pausedconnector");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

}
