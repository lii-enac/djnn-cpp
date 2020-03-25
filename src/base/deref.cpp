/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "deref.h"

#include "core/serializer/serializer.h"
#include "core/execution/graph.h"
#include "core/utils/error.h"

#include <iostream>

namespace djnn
{
  Deref::Deref (Process *parent, const std::string& name, Process *ref, const std::string& path)
  : Process (name),
  _path (this, "path", path),
  _activation (this, "activation"),
  _action (this, "action"),
  _cref (ref, ACTIVATION, &_action, ACTIVATION),
  _cpath (&_path, ACTIVATION, &_action, ACTIVATION),
  _cactivation ()
  {
    _ref = dynamic_cast<RefProperty*> (ref);
    if (_ref == nullptr) {
      warning (this, "Deref is only applicable to RefProperty");
      return;
    }
    Graph::instance ().add_edge (_ref, &_action);
    Graph::instance ().add_edge (&_path, &_action);

    update_src ();
    Process::finalize_construction (parent, name);
  }

  Deref::~Deref ()
  {
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (_ref, &_action);
    Graph::instance ().remove_edge (&_path, &_action);
  }

  void
  Deref::update_src ()
  {
    Process* unref = _ref->get_value ();
    if (unref != nullptr) {
      Process* dst = unref->find_child (_path.get_value());
      if (dst != nullptr) {
        _cactivation.uninit();
        _cactivation.init (dst, ACTIVATION, &_activation, ACTIVATION, true);
      }
    }
  }
  void
  Deref::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    Process::set_parent (p); 
  }

  void
  Deref::impl_activate ()
  {
    _cref.enable ();
    _cpath.enable ();
    _cactivation.enable ();
  }

  void
  Deref::impl_deactivate ()
  {
    _cref.disable ();
    _cpath.disable ();
    _cactivation.disable ();;
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Deref::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:deref");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("path", _path.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
#endif
}
