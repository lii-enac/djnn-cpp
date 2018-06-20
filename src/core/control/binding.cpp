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

#include "binding.h"
#include "../execution/graph.h"
#include "../error.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  Binding::init_binding (Process* src, const string & ispec, Process* dst, const string & dspec)
  {
    if (src == 0) {
      error ("src argument cannot be null in binding creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }
    if (dst == 0) {
      error ("dst argument cannot be null in binding creation (" + get_name() + ", " + ispec + ", " + dspec + ")");
    }
    if (ispec.length () > 0) {
      _src = src->find_component (ispec);
      if (_src == 0) {
        error ("source child " + ispec + " not found in binding (" + get_name() + ", " + ispec + ", " + dspec + ")");
      }
    } else
      _src = src;
    if (dspec.length () > 0) {
      _dst = dst->find_component (dspec);
      if (_dst == 0) {
        error ("destination child " + ispec + " not found in binding (" + get_name() + ", " + ispec + ", " + dspec + ")");
      }
    } else
      _dst = dst;
    _action = make_unique<BindingAction> (this, "binding_" + _src->get_name () + "_to_" + _dst->get_name () + "_action",
                                          _dst);
    _c_src = std::unique_ptr<Coupling> (new Coupling (_src, ACTIVATION, _action.get (), ACTIVATION));
    Graph::instance ().add_edge (_src, _action.get ());
    Graph::instance ().add_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    _c_src->disable ();
  }

  Binding::Binding (Process* src, const string & ispec, Process* dst, const string & dspec)
  {
    init_binding (src, ispec, dst, dspec);
  }

  Binding::Binding (Process* parent, const string & name, Process* src, const string & ispec,
                    Process* dst, const string & dspec) :
      Process (parent, name)
  {
    init_binding (src, ispec, dst, dspec);
    Process::finalize ();
  }

  Binding::~Binding ()
  {
    Graph::instance ().remove_edge (_src, _action.get ());
    Graph::instance ().remove_edge (_action.get (), _dst);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }

}
