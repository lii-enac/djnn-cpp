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
#include "../core/error.h"
#include "../core/serializer/serializer.h"


#include <iostream>

namespace djnn
{
  using namespace std;

  void
  Connector::ConnectorAction::activate () { 
    /* do we have to check if the source is activable? */
    if (_parent->get_state () < deactivating)
      AbstractAssignment::do_assignment (_src, _dst, _propagate);   
  }

  Connector::Connector (Process *p, string n, Process *src, string ispec, Process *dst,
                        string dspec, bool copy_on_activation) :
      Process (p, n), _c_src (nullptr), _copy_on_activation(copy_on_activation)
  {
    init_connector (src, ispec, dst, dspec);
    Process::finalize ();
  }

  Connector::Connector (Process *src, string ispec, Process *dst, string dspec,
                        bool copy_on_activation) :
      _c_src (nullptr), _copy_on_activation(copy_on_activation)
  {
    init_connector (src, ispec, dst, dspec);
  }

  void
  Connector::activate () 
  { 
    _c_src->enable ();
    if (_copy_on_activation)
      _action->activation ();
  }

  void
  Connector::deactivate ()
  { 
    _c_src->disable();
    _action->deactivation ();
  }

  void
  Connector::init_connector (Process *src, string ispec, Process *dst, string dspec)
  {
    if (src == 0) {
      error (this, "src argument cannot be null in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    if (dst == 0) {
      error (this, "dst argument cannot be null in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_src = src->find_component (ispec);
    if (c_src == 0) {
      error (this, "source not found in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_dst = dst->find_component (dspec);
    if (c_dst == 0) {
      error (this, "destination not found in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    _src = dynamic_cast<AbstractProperty*> (c_src);
    _dst = dynamic_cast<AbstractProperty*> (c_dst);
    if (!_src)
      warning (this, "invalid source (not a Property) in connector (" + get_name() + "," + ispec + ", " + dspec + ")");
    if (!_dst)
      warning (this, "invalid destination (not a Property) in connector (" + get_name() + "," + ispec + ", " + dspec + ")");
  

    _action = shared_ptr<Process> (
        new ConnectorAction (this, "connector_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", _src,
                             _dst, true));

    _c_src = unique_ptr<Coupling> (new Coupling (_src, ACTIVATION, _action.get (), ACTIVATION));

    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    
    _c_src.get ()->disable ();
    _c_src->enable ();
  }

  Connector::~Connector ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  Connector::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("base:connector");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }


  PausedConnector::PausedConnector (Process *p, string n, Process *src, string ispec, Process *dst,
                        string dspec, bool copy_on_activation) :
      Process (p, n), _c_src (nullptr), _copy_on_activation(copy_on_activation)
  {
    init_pausedconnector (src, ispec, dst, dspec);
    Process::finalize ();
  }

  PausedConnector::PausedConnector (Process *src, string ispec, Process *dst, string dspec, 
                                    bool copy_on_activation) :
      _c_src (nullptr), _copy_on_activation(copy_on_activation)
  {
    init_pausedconnector (src, ispec, dst, dspec);
  }

  void
  PausedConnector::activate () 
  { 
    _c_src->enable ();
    if (_copy_on_activation) 
      _action->activation ();
  }

  void
  PausedConnector::deactivate ()
  { 
    _c_src->disable();
    _action->deactivation ();
  }

  void
  PausedConnector::init_pausedconnector (Process *src, string ispec, Process *dst, string dspec)
  {
    if (src == 0) {
      error (this, "src argument cannot be null in pausedconnector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    if (dst == 0) {
      error (this, "dst argument cannot be null in pausedconnector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_src = src->find_component (ispec);
    if (c_src == 0) {
      error (this, "source not found in pausedconnector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_dst = dst->find_component (dspec);
    if (c_dst == 0) {
      error (this, "destination not found in pausedconnector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    _src = dynamic_cast<AbstractProperty*> (c_src);
    _dst = dynamic_cast<AbstractProperty*> (c_dst);
    if (!_src || !_dst) {
      warning (this, "invalid source or destination in pausedconnector (" + get_name() + "," + ispec + " " + dspec + ")");
    }

    _action = shared_ptr<Process> (
        new Connector::ConnectorAction (this, "pausedconnector_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", _src,
                             _dst, false));
    _c_src = unique_ptr<Coupling> (new Coupling (_src, ACTIVATION, _action.get (), ACTIVATION));
    
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    
    _c_src.get ()->disable ();
    _c_src->enable ();
    
  }

  PausedConnector::~PausedConnector ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

  void
  PausedConnector::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("base:pausedconnector");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("source", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
    AbstractSerializer::serializer->text_attribute ("destination", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

}
