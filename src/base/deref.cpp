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
#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"

namespace djnn
{
  AbstractDeref::AbstractDeref (ParentProcess *parent, const std::string& name, CoreProcess *ref, const std::string& path, djnn_dir_t dir)
  : FatProcess (name),
  _path (this, "path", path),
  _action (this, "action"),
  _set (this, "set_action"),
  _get (this, "get_action"),
  _cref (ref, ACTIVATION, &_action, ACTIVATION),
  _cpath (&_path, ACTIVATION, &_action, ACTIVATION),
  _cget (),
  _cset (),
  _dir (dir),
  _propagating (false),
  _src_updated (false)
  {
    _ref = djnn_dynamic_cast<RefProperty*> (ref);
    if (_ref == nullptr) {
      warning (this, "Deref is only applicable to RefProperty");
      return;
    }
    graph_add_edge (_ref, &_action);
    graph_add_edge (&_path, &_action);
  }

  AbstractDeref::~AbstractDeref ()
  {
    auto *old_src = _cset.get_src();
    if (old_src) {
      graph_remove_edge (old_src, &_get);
      graph_remove_edge (&_set, old_src);
    }
    auto* unref = _ref->get_value ();
    if (_src_updated && unref != nullptr) {
      auto* src = unref->find_child_impl (_path.get_value ());
      graph_remove_edge (&_set, src);
    }
    graph_remove_edge (&_path, &_action);
    graph_remove_edge (_ref, &_action);
  }

  void
  AbstractDeref::impl_deactivate ()
  {
    _cref.disable ();
    _cpath.disable ();
    _cset.disable ();
    _cget.disable ();
  }

  void
  AbstractDeref::update_src ()
  {

    auto* unref = _ref->get_value ();
    auto *old_src = _cget.get_src ();
    auto *new_src = unref == nullptr? nullptr : unref->find_child_impl (_path.get_value ());
    if (old_src == new_src)
      return;
    if (old_src != nullptr) {
      graph_remove_edge (&_set, old_src);
    }
    _cget.uninit ();
    _cget.set_src (nullptr);
    if (unref != nullptr) {
      auto* src = unref->find_child_impl (_path.get_value ());
      if (src != nullptr) {
        _cget.init (src, ACTIVATION, &_get, ACTIVATION);
        graph_add_edge (&_set, src);
        _src_updated = true ;
      }
      change_src (src);
    }
  }

  void
  AbstractDeref::set_parent (ParentProcess* parent)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);
    FatProcess::set_parent (parent);
  }

  void
  AbstractDeref::impl_activate ()
  {
    _cref.enable ();
    _cpath.enable ();
    _cset.enable ();
    _cget.enable ();
    update_src ();
  }

  Deref::Deref (ParentProcess* parent, const std::string& name, CoreProcess *ref, const std::string& path, djnn_dir_t dir)
  : AbstractDeref (parent, name, ref, path, dir),
    _activation (this, "activation"),
   _src (nullptr)
  {
    _cset.init (&_activation, ACTIVATION, &_set, ACTIVATION);
    graph_add_edge (&_activation, &_set);
    graph_add_edge (&_get, &_activation);
    finalize_construction (parent, name);
  }

  Deref::~Deref ()
  {
    graph_remove_edge (&_get, &_activation);
    graph_remove_edge (&_activation, &_set);
  }

  void
  Deref::set ()
  {
    if (_src) {
      _cget.disable ();
      _src->activate ();
      _cget.enable ();
    }
  }

  void
  Deref::get ()
  {
    _cset.disable ();
    _activation.activate();
    _cset.enable ();
  }

  void
  Deref::change_src (CoreProcess *src)
  {
    _src = src;
  }

  DerefString::DerefString (ParentProcess* parent, const std::string& name, CoreProcess *ref, const std::string& path, djnn_dir_t dir)
  : AbstractDeref (parent, name, ref, path, dir),
    _value (this, "value", ""),
  _src (nullptr)
  {
    _cset.init (&_value, ACTIVATION, &_set, ACTIVATION);
    graph_add_edge (&_value, &_set);
    graph_add_edge (&_get, &_value);
    finalize_construction (parent, name);
  }

  DerefString::~DerefString ()
  {
    graph_remove_edge (&_value, &_set);
    graph_remove_edge (&_get, &_value);
  }

  void
  DerefString::get ()
  {
    if (_src) {
      _cset.disable ();
      _value.set_value (_src->get_string_value (), true);
      _cset.enable ();
    }
  }
  void
  DerefString::set ()
  {
    if (_src) {
      _cget.disable ();
      _src->set_value (_value.get_value (), true);
      _cget.enable ();
    }
  }

  void
  DerefString::change_src (CoreProcess *src)
  {
    _src = djnn_dynamic_cast<AbstractProperty*> (src);
    if (_src) {
      if (_dir == DJNN_GET_ON_CHANGE)
        get ();
      else if (_dir == DJNN_SET_ON_CHANGE)
        set ();
    }
  }

  DerefDouble::DerefDouble (ParentProcess* parent, const std::string &name, CoreProcess *ref, const std::string &path, djnn_dir_t dir) :
      AbstractDeref (parent, name, ref, path, dir),
      _value (this, "value", 0),
      _src (nullptr)
  {
    _cset.init (&_value, ACTIVATION, &_set, ACTIVATION);
    graph_add_edge (&_value, &_set);
    graph_add_edge (&_get, &_value);
    finalize_construction (parent, name);
  }

  DerefDouble::~DerefDouble()
  {
    graph_remove_edge (&_value, &_set);
    graph_remove_edge (&_get, &_value);
  }

  void
  DerefDouble::get ()
  {
    if (_src) {
      _cset.disable ();
      _value.set_value (_src->get_double_value (), true);
      _cset.enable ();
    }
  }
  void
  DerefDouble::set ()
  {
    if (_src) {
      _cget.disable ();
      _src->set_value (_value.get_value (), true);
      _cget.enable ();
    }
  }

  void
  DerefDouble::change_src (CoreProcess *src)
  {
    _src = djnn_dynamic_cast<AbstractProperty*> (src);
    if (_src) {
      if (_dir == DJNN_GET_ON_CHANGE)
        get ();
      else if (_dir == DJNN_SET_ON_CHANGE)
        set ();
    }
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
  void
  DerefDouble::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:deref-double");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("path", _path.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }
#endif
}
