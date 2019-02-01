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

#include "binding.h"
#include "../execution/graph.h"
#include "../error.h"
#include "../serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  Binding::init_binding (Process* src, const string & ispec, bool on_activation, Process* dst, const string & dspec,
			 bool to_activate)
  {
    if (src == 0) {
      error (this,
	     "src argument cannot be null in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }
    if (dst == 0) {
      error (this,
	     "dst argument cannot be null in binding creation (" + get_name () + ", " + ispec + ", " + dspec + ")");
    }
    if (ispec.length () > 0) {
      _src = src->find_component (ispec);
      if (_src == 0) {
	error (this,
	       "source child " + ispec + " not found in binding (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
    } else
      _src = src;
    if (dspec.length () > 0) {
      _dst = dst->find_component (dspec);
      if (_dst == 0) {
	error (
	    this,
	    "destination child " + ispec + " not found in binding (" + get_name () + ", " + ispec + ", " + dspec + ")");
      }
    } else
      _dst = dst;
    _action = new BindingAction (this, "binding_" + _src->get_name () + "_to_" + _dst->get_name () + "_action", to_activate);
    if (on_activation)
      _c_src = new Coupling (_src, ACTIVATION, _action, ACTIVATION);
    else
      _c_src = new Coupling (_src, DEACTIVATION, _action, ACTIVATION);
    Graph::instance ().add_edge (_src, _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _dst);
    _c_src->disable ();
  }

  Binding::Binding (Process* src, const string & ispec, Process* dst, const string & dspec)
  {
    init_binding (src, ispec, true, dst, dspec, true);
  }

  Binding::Binding (Process* parent, const string & name, Process* src, const string & ispec, Process* dst,
		    const string & dspec) :
      Process (parent, name)
  {
    init_binding (src, ispec, true, dst, dspec, true);
    Process::finalize ();
  }

  Binding::Binding (Process* parent, const string &name, Process* src, const string & ispec, bool on_activation,
		    Process* dst, const string & dspec, bool activate) :
      Process (parent, name)
  {
    init_binding (src, ispec, on_activation, dst, dspec, activate);
    Process::finalize ();
  }

  Binding::~Binding ()
  {

    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _dst);
    Graph::instance ().remove_edge (_src, _dst);

    delete _c_src;
    delete _action;

  }

  void
  Binding::serialize (const string& format)
  {

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:binding");
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
