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
#include "core/control/assignment.h"
#include "core/execution/graph.h"
#include "core/utils/error.h"
#include "core/serializer/serializer.h"
#include "core/utils/utils-dev.h"


namespace djnn
{
  Connector::ConnectorAction::ConnectorAction (FatProcess* parent, const std::string& name, AbstractProperty** src, AbstractProperty** dst, bool propagate) :
        Action (parent, name), _src (src), _dst (dst), _propagate (propagate) 
  {
  }

  void
  Connector::ConnectorAction::impl_activate ()
  {
    /* do we have to check if the source is activable? */
    if (*_src && *_dst) {
      AbstractAssignment::do_assignment (*_src, *_dst, _propagate);
    }
  }

  Connector::Init::Init(Connector * c, FatProcess* src, const std::string&  ispec, FatProcess* dst, const std::string&  dspec,
    std::string& src_ref_spec, std::string& dst_ref_spec)
  {
    if (src == 0) {
      error (c,
       (std::string("src argument cannot be null in connector creation (") + c->get_name () + ", " + ispec + ", " + dspec + ")").c_str());
    }
    if (dst == 0) {
      error (c,
       (std::string("dst argument cannot be null in connector creation (") + c->get_name () + ", " + ispec + ", " + dspec + ")").c_str());
    }

    std::pair<RefProperty*, std::string> ref_src_pair = check_for_ref (src, ispec);
    c->_ref_info_src._ref = ref_src_pair.first;
    //c->_ref_info_src._spec = ref_src_pair.second;
    src_ref_spec = ref_src_pair.second;

    std::pair<RefProperty*, std::string> ref_dst_pair = check_for_ref (dst, dspec);
    c->_ref_info_dst._ref = ref_dst_pair.first;
    //c->_ref_info_dst._spec = ref_dst_pair.second;
    dst_ref_spec = ref_dst_pair.second;
  }

  void
  Connector::check_init(const std::string& ispec, const std::string& dspec)
  {
    if(!_ref_info_src._ref) {
      if (_src == 0) {
        error (this, (std::string("SOURCE not found in connector creation or SOURCE is not a property (") + get_name () + ", " + ispec + ", " + dspec + ")").c_str());
      }
    }

    if(!_ref_info_dst._ref) {
      if (_dst == 0) {
        error (
            this,
            (std::string("DESTINATION not found in (Paused)assignment creation or the DESTINATION of an (Paused)assignment must be a property ( name: ") + get_name () + ", src spec: " + ispec
                + ", dst spec:" + dspec + ")\n").c_str());
      }
    }
  }




  Connector::Connector (FatProcess *parent, const std::string& name, FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec, bool copy_on_activation,
    std::string src_ref_spec, std::string dst_ref_spec)
  :
    SrcToDstLink (parent, name),
    _init(this, src, ispec, dst, dspec, src_ref_spec, dst_ref_spec),
    _src(!_ref_info_src.is_ref() && src ? dynamic_cast<AbstractProperty*>(src->find_child (ispec)) : nullptr),
    _dst(!_ref_info_dst.is_ref() && dst ? dynamic_cast<AbstractProperty*>(dst->find_child (dspec)) : nullptr),
    _ref_update_src(_ref_info_src.is_ref() ? ref_update(this, _ref_info_src, src_ref_spec, (FatProcess**)&_src) : ref_update()), // uses copy constructor
    _ref_update_dst(_ref_info_dst.is_ref() ? ref_update(this, _ref_info_dst, dst_ref_spec, (FatProcess**)&_dst) : ref_update()),
    _action(this, "connector_" + (_src ? _src->get_name () : "") + "_to_" + ( _dst ? _dst->get_name () : "") + "_action", &_src, &_dst, true),
    _c_src(_src ? Coupling(_src, ACTIVATION, &_action, ACTIVATION, true) : Coupling()),
    _has_coupling (false),
    _copy_on_activation (copy_on_activation)
  {
    if (_src) {
      _c_src.disable ();
      if(_dst) {
        Graph::instance ().add_edge (_src, _dst);
        _has_coupling = true;
      }
    }
    if (_ref_info_src.is_ref()) _ref_update_src._update.impl_activate ();
    if (_ref_info_dst.is_ref()) _ref_update_dst._update.impl_activate ();
    check_init(ispec, dspec);
    
    finalize_construction (parent, name);
  }

  void
  Connector::impl_activate ()
  {
    /* when an connector is in a deactivate fsm branch and src has changed. it is not aware of it.
       we have to re-evaluate it */
    if(_ref_info_src.is_ref()) {
      _ref_update_src._update.impl_activate ();
    }
    _c_src.enable();
    if (_copy_on_activation) {
      _action.activate ();
    }
  }

  void
  Connector::impl_deactivate ()
  {
    _c_src.disable ();
    _action.deactivate ();
  }



  void
  Connector::set_parent (FatProcess* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent () && _dst) {
       remove_state_dependency (get_parent (), _dst);
    }

    if (_dst)
      add_state_dependency (p, _dst);

    FatProcess::set_parent (p); 
  }

  void
  Connector::update_graph ()
  {
    if (_has_coupling) {
      /* remove_edge is made before in about_to_update_graph hook */
      _c_src.uninit ();
    }
    if (_src && _dst) {
      if(_has_coupling) {
        _c_src.set_src(_src);
      } else {
        _c_src.init(_src, ACTIVATION, &_action, ACTIVATION, true);
      }
      //cerr << "Connector::"<< __FUNCTION__ << " add src/dst edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().add_edge (_src, _dst);
      _has_coupling = true;
      if ( get_activation_state()==ACTIVATED ) {
        _action.activate ();
      } else {
        _c_src.disable ();
      }
    }
  }

  void 
  Connector::about_to_update_graph ()
  {
    if (_src && _dst) {
      //cerr << endl << "Connector::"<< __FUNCTION__ <<  " remove old src/dst edge:" << _src->get_name () << " - "  << _dst->get_name () << endl;
      Graph::instance ().remove_edge (_src, _dst);
    }
  }

  Connector::~Connector ()
  {
    remove_state_dependency (get_parent (), _dst);
    Graph::instance ().remove_edge (_src, _dst);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Connector::serialize (const std::string& format)
  {
    std::string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:connector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

  PausedConnector::PausedConnector (FatProcess *parent, const std::string& name, FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec,
                                    bool copy_on_activation) :
      FatProcess (name), _c_src (nullptr), _copy_on_activation (copy_on_activation)
  {
    init_pausedconnector (src, ispec, dst, dspec);
    finalize_construction (parent, name);
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
  PausedConnector::init_pausedconnector (FatProcess *src, const std::string& ispec, FatProcess *dst, const std::string& dspec)
  {
    if (src == 0) {
      error ( this, (std::string("src argument cannot be null in pausedconnector creation (") + get_name () + ", " + ispec + ", " + dspec + ")").c_str());
      return;
    }

    if (dst == 0) {
      error ( this, (std::string("dst argument cannot be null in pausedconnector creation (") + get_name () + ", " + ispec + ", " + dspec + ")").c_str());
      return;
    }

    FatProcess* c_src = src->find_child (ispec);

    if (c_src == 0) {
      error (this, (std::string("source not found in pausedconnector creation (") + get_name () + ", " + ispec + ", " + dspec + ")").c_str());
      return;
    }

    FatProcess* c_dst = dst->find_child (dspec);

    if (c_dst == 0) {
      error (this,
       (std::string("destination not found in pausedconnector creation (") + get_name () + ", " + ispec + ", " + dspec + ")").c_str());
      return;
    }

    _src = dynamic_cast<AbstractProperty*> (c_src);
    _dst = dynamic_cast<AbstractProperty*> (c_dst);
    if (!_src || !_dst) {
      warning (this,
       (std::string("invalid source or destination in pausedconnector (" + get_name () + "," + ispec + " " + dspec + ")").c_str()));
    }

    _action = new Connector::ConnectorAction (
      this, "pausedconnector_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", &_src, &_dst, false);
    _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION, true);

    Graph::instance ().add_edge (_src, _dst);
    _c_src->disable ();

  }

  PausedConnector::~PausedConnector ()
  {
    remove_state_dependency (get_parent (), _dst);
    Graph::instance ().remove_edge (_src, _dst);

    delete _c_src;
    delete _action;
  }

  void
  PausedConnector::set_parent (FatProcess* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), _dst);
    }

    add_state_dependency (p, _dst);
    FatProcess::set_parent (p); 
  }

#ifndef DJNN_NO_SERIALIZE
  void
  PausedConnector::serialize (const std::string& format)
  {
    std::string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("base:pausedconnector");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif
}
