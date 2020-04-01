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
  AbstractDeref::AbstractDeref (Process *parent, const std::string& name, Process *ref, const std::string& path)
  : Process (name),
  _path (this, "path", path),
  _action (this, "action"),
  _set_dst (this, "set_dst"),
  _set_src (this, "set_src"),
  _cref (ref, ACTIVATION, &_action, ACTIVATION),
  _cpath (&_path, ACTIVATION, &_action, ACTIVATION),
  _csrc_to_dst ()
  {
    _ref = dynamic_cast<RefProperty*> (ref);
    if (_ref == nullptr) {
      warning (this, "Deref is only applicable to RefProperty");
      return;
    }
    Graph::instance ().add_edge (_ref, &_action);
    Graph::instance ().add_edge (&_path, &_action);
  }

  AbstractDeref::~AbstractDeref ()
  {
    remove_state_dependency (get_parent (), &_action);
    Graph::instance ().remove_edge (_ref, &_action);
    Graph::instance ().remove_edge (&_path, &_action);
  }

  void
  AbstractDeref::impl_deactivate ()
  {
    _cref.disable ();
    _cpath.disable ();
    _csrc_to_dst.disable ();
  }
  void
  AbstractDeref::update_src ()
  {
    Process* unref = _ref->get_value ();
    if (unref != nullptr) {
      Process* src = unref->find_child (_path.get_value());
      if (src != nullptr) {
        _csrc_to_dst.uninit ();
        _csrc_to_dst.init (src, ACTIVATION, &_set_dst, ACTIVATION, true);
      }
      change_src (src);
    }
  }

  void
  AbstractDeref::set_parent (Process* p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (p, &_action);
    Process::set_parent (p);
  }

  void
  AbstractDeref::impl_activate ()
  {
    _cref.enable ();
    _cpath.enable ();
    _csrc_to_dst.enable ();
  }

  Deref::Deref (Process *parent, const std::string& name, Process *ref, const std::string& path)
  : AbstractDeref (parent, name, ref, path),
  _activation (this, "activation"),
  _src (nullptr),
  _cdst_to_src (&_activation, ACTIVATION, &_set_src, ACTIVATION, true)
  {
	AbstractDeref::update_src ();
    Process::finalize_construction (parent, name);
  }

  void
  Deref::impl_activate ()
  {
    AbstractDeref::impl_activate ();
    _cdst_to_src.enable ();
  }

  void
  Deref::impl_deactivate ()
  {
    AbstractDeref::impl_deactivate ();
    _cdst_to_src.disable ();
  }

  void
  Deref::set_to_src ()
  {
    if (_src)
      _src->activate ();
  }

  void
  Deref::set_to_dst ()
  {
    _activation.activate();
  }
  void
  Deref::change_src (Process *src)
  {
    _src = src;
  }

  DerefString::DerefString (Process *parent, const std::string& name, Process *ref, const std::string& path)
  : AbstractDeref (parent, name, ref, path),
  _value (this, "value", ""),
  _src (nullptr),
  _cdst_to_src (&_value, ACTIVATION, &_set_src, ACTIVATION, true)
  {
    AbstractDeref::update_src ();
    Process::finalize_construction (parent, name);
  }

  void
  DerefString::set_to_dst ()
  {
    if (_src)
      _value.set_value (_src->get_string_value (), true);
  }
  void
  DerefString::set_to_src ()
  {
    if (_src)
      _src->set_value (_value.get_value(), true);
  }

  void
  DerefString::change_src(Process *src)
  {
    _src = dynamic_cast<AbstractProperty*> (src);
  }

  void
  DerefString::impl_activate ()
  {
    AbstractDeref::impl_activate ();
    _cdst_to_src.enable ();
  }

  void
  DerefString::impl_deactivate ()
  {
    AbstractDeref::impl_deactivate ();
    _cdst_to_src.disable ();
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
  void
  DerefString::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:deref-string");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("path", _path.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
#endif
}
