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
#include "core/execution/graph.h"
#include "core/utils/error.h"
#include "core/serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Synchronizer::Init::Init(Synchronizer * s, Process *parent, const string &name, Process* dst, const string & dspec)
  {
    if (dst == nullptr) {
      error (s, "dst argument cannot be null in synchronizer creation (" + s->get_name () + ", " + dspec + ")");
      return;
    }
    s->_dst = dst->find_component (dspec);
    if (s->_dst == nullptr) {
      error (s, "destination child " + dspec + " not found in synchronizer (" + s->get_name () + ", " + dspec + ")");
      return;
    }
  }

  Synchronizer::Synchronizer (Process *parent, const string &name, Process* dst, const string & dspec)
  :
    Process (name),
    _dst(nullptr),
    _init (this, parent, name, dst, dspec),
    _action (this, "synchronizer_" + _dst->get_name () + "_action")
  {
    Graph::instance ().add_edge (&_action, _dst);
    Process::finalize_construction (parent, name);
  }

  Synchronizer::~Synchronizer ()
  {
    _action.remove_all_native_edges ();
    Graph::instance ().remove_edge (&_action, _dst);
    //for (auto c : _c_list) {
    int sz = _c_list.size ();
    for (int i = sz - 1; i >= 0; i--) {
      auto * c = _c_list[i];
      Process* src = c->get_src ();
      Graph::instance ().remove_edge (src, &_action);
      delete c;
    }
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
    _action.add_native_edge (src, dst);
  }

  void
  Synchronizer::add_source (Process *src, const string &ispec)
  {
    if (src == nullptr) {
      error (this,
             "src argument cannot be null in source addition to synchronizer (" + get_name () + ", " + ispec + ")");
    }
    Process* _src = src->find_component (ispec);
    Coupling *cpl = new Coupling (_src, ACTIVATION, &_action, ACTIVATION);
    /* 
      if the synchronizer is already activated => cpl->enable (by default)
      if the synchronizer is not activated => cpl->disable (it will be enable by synchronizer::impl_activate () function)
    */
    if (!this->somehow_activating ())
      cpl->disable ();
    Graph::instance ().add_edge (_src, &_action);

    _c_list.push_back (cpl);
  }

  void
  Synchronizer::propagate ()
  {
    _dst->activate ();
  }
}

