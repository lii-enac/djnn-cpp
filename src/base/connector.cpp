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

#include "connector.h"
#include "../core/execution/graph.h"
#include "../core/error.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Connector::Connector (Process *p, string n, Process *src, string ispec, Process *dst,
                        string dspec) :
      Process (p, n), _c_src (nullptr)
  {
    init_connector (src, ispec, dst, dspec);
    Process::finalize ();
  }

  Connector::Connector (Process *src, string ispec, Process *dst, string dspec) :
      _c_src (nullptr)
  {
    init_connector (src, ispec, dst, dspec);
  }

  void
  Connector::init_connector (Process *src, string ispec, Process *dst, string dspec)
  {
    if (src == 0) {
      error ("src argument cannot be null in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }
    if (dst == 0) {
      error ("dst argument cannot be null in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }
    Process* c_src = src->find_component (ispec);
    if (c_src == 0) {
      error ("source not found in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }

    Process* c_dst = dst->find_component (dspec);
    if (c_dst == 0) {
      error ("destination not found in connector creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }
    _src = dynamic_cast<AbstractProperty*> (c_src);
    _dst = dynamic_cast<AbstractProperty*> (c_dst);
    if (!_src || !_dst) {
      warning ("invalid source or destination in connector (" + get_name() + "," + ispec + " " + dspec + ")");
    }

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
}
