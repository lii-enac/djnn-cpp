/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "synchronizer.h"
#include "../execution/graph.h"
#include "../utils/error.h"
#include "../serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Synchronizer::Synchronizer (Process *p, const string &n, Process* dst, const string & dspec) :
      Process (n)
  {
    if (dst == nullptr) {
      error (this, "dst argument cannot be null in synchronizer creation (" + n + ", " + dspec + ")");
    }
    _dst = dst->find_component (dspec);
    if (_dst == nullptr) {
      error (this, "destination child " + dspec + " not found in synchronizer (" + get_name () + ", " + dspec + ")");
    }

    _action = new SynchronizerAction (this, "synchronizer_" + _dst->get_name () + "_action");
    Graph::instance ().add_edge (_action, _dst);
    Process::finalize_construction (p);
  }

  Synchronizer::~Synchronizer ()
  {
    for (auto c : _c_list) {
      Process* src = c->get_src ();
      Graph::instance ().remove_edge (src, _action);
      delete c;
    }
    Graph::instance ().remove_edge (_action, _dst);
    delete _action;
  }

  void
  Synchronizer::impl_activate ()
  {
    for (auto c: _c_list) {
      c->enable ();
    }
  }

  void
  Synchronizer::impl_deactivate ()
  {
    for (auto c: _c_list) {
      c->disable ();
    }
  }

  void 
  Synchronizer::add_native_edge (Process *src, Process* dst)
  {
    _action->add_native_edge (src, dst);
  }

  void
  Synchronizer::add_source (Process *src, const string &ispec)
  {
    if (src == nullptr) {
      error (this,
             "src argument cannot be null in source addition to synchronizer (" + get_name () + ", " + ispec + ")");
    }
    Process* _src = src->find_component (ispec);
    Coupling *cpl = new Coupling (_src, ACTIVATION, _action, ACTIVATION);
    /* 
      if the synchronizer is already activated => cpl->enable (by default)
      if the synchronizer is not activated => cpl->disable (it will be enable by synchronizer::impl_activate () function)
    */
    if (!this->somehow_activating ())
    cpl->disable ();
    Graph::instance ().add_edge (_src, _action);

    _c_list.push_back (cpl);
  }

  void
  Synchronizer::propagate ()
  {
    _dst->activate ();
  }
}

