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
#include "../error.h"
#include "../serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Synchronizer::Synchronizer (Process *p, const string &n, Process* dst, const string & dspec) :
      Process (p, n)
  {
    if (dst == 0) {
      error (this, "dst argument cannot be null in synchronizer creation (" + n + ", " + dspec + ")");
    }
    _dst = dst->find_component (dspec);
    if (_dst == 0) {
      error (this, "destination child " + dspec + " not found in synchronizer (" + get_name () + ", " + dspec + ")");
    }

    _action = new SynchronizerAction (this, "synchronizer_" + _dst->get_name () + "_action");
    Process::finalize ();

  }

  Synchronizer::~Synchronizer ()
  {
    for (auto c : _c_list) {
      Process* src = c->get_src ();
      Graph::instance ().remove_edge (src, _action);
      delete c;
    }
    delete _action;
  }

  void
  Synchronizer::activate ()
  {
    for (auto c: _c_list) {
      c->enable ();
    }
  }

  void
  Synchronizer::deactivate ()
  {
    for (auto c: _c_list) {
      c->disable ();
    }
  }

  void
  Synchronizer::add_source (Process *src, const string &ispec)
  {
    if (src == 0) {
      error (this,
             "src argument cannot be null in source addition to synchronizer (" + get_name () + ", " + ispec + ")");
    }
    Process* _src = src->find_component (ispec);
    Coupling *cpl = new Coupling (_src, ACTIVATION, _action, ACTIVATION);
    Graph::instance ().add_edge (_src, _action);
    _c_list.push_back (cpl);
  }

  void
  Synchronizer::propagate ()
  {
    _dst->activation ();
  }
}

