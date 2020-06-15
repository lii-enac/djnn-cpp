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

namespace djnn
{
  AbstractDeref::AbstractDeref (Process *parent, const std::string& name, Process *ref, const std::string& path, djnn_dir_t dir)
  : Process (name),
  _path (this, "path", path),
  _action (this, "action"),
  _set (this, "set_action"),
  _get (this, "get_action"),
  _cref (ref, ACTIVATION, &_action, ACTIVATION, true),
  _cpath (&_path, ACTIVATION, &_action, ACTIVATION, true),
  _cget (),
  _cset (),
  _dir (dir),
  _propagating (false)
  {
    _ref = dynamic_cast<RefProperty*> (ref);
    if (_ref == nullptr) {
      warning (this, "Deref is only applicable to RefProperty");
      return;
    }
  }

  AbstractDeref::~AbstractDeref ()
  {
    Process *old_src = _cset.get_src();
    if (old_src) {
      Graph::instance().remove_edge (old_src, &_get);
    }
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
    Process* unref = _ref->get_value ();
    Process *old_src = _cget.get_src ();
    if (old_src) {
      Graph::instance().remove_edge (&_set, old_src);
    }
    _cget.uninit ();
    _cget.change_source (nullptr);
    if (unref != nullptr) {
      Process* src = unref->find_child (_path.get_value ());
      if (src != nullptr) {
        _cget.init (src, ACTIVATION, &_get, ACTIVATION);
        Graph::instance().add_edge (&_set, src);
      }
      change_src (src);
    }
  }

  void
  AbstractDeref::impl_activate ()
  {
    _cref.enable ();
    _cpath.enable ();
    _cset.enable ();
    _cget.enable ();
  }

  Deref::Deref (Process *parent, const std::string& name, Process *ref, const std::string& path, djnn_dir_t dir)
  : AbstractDeref (parent, name, ref, path, dir),
    _activation (this, "activation"),
   _src (nullptr)
  {
    _cset.init (&_activation, ACTIVATION, &_set, ACTIVATION);
    Graph::instance ().add_edge (&_activation, &_set);
    Graph::instance ().add_edge (&_get, &_activation);
	  AbstractDeref::update_src ();
    Process::finalize_construction (parent, name);
  }

  Deref::~Deref ()
  {
    Graph::instance ().remove_edge (&_activation, &_set);
    Graph::instance ().remove_edge (&_get, &_activation);
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
  Deref::change_src (Process *src)
  {
    _src = src;
  }

  DerefString::DerefString (Process *parent, const std::string& name, Process *ref, const std::string& path, djnn_dir_t dir)
  : AbstractDeref (parent, name, ref, path, dir),
    _value (this, "value", ""),
  _src (nullptr)
  {
    _cset.init (&_value, ACTIVATION, &_set, ACTIVATION);
    Graph::instance ().add_edge (&_value, &_set);
    Graph::instance ().add_edge (&_get, &_value);
    AbstractDeref::update_src ();
    Process::finalize_construction (parent, name);
  }

  DerefString::~DerefString ()
  {
    Graph::instance ().remove_edge (&_value, &_set);
    Graph::instance ().remove_edge (&_get, &_value);
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
  DerefString::change_src(Process *src)
  {
    _src = dynamic_cast<AbstractProperty*> (src);
    if (_src) {
      if (_dir == DJNN_GET_ON_CHANGE)
        get ();
      else if (_dir == DJNN_SET_ON_CHANGE)
        set ();
    }
  }

  DerefDouble::DerefDouble (Process *parent, const std::string &name, Process *ref, const std::string &path, djnn_dir_t dir) :
      AbstractDeref (parent, name, ref, path, dir),
      _value (this, "value", 0),
      _src (nullptr)
  {
    _cset.init (&_value, ACTIVATION, &_set, ACTIVATION);
    Graph::instance ().add_edge (&_value, &_set);
    Graph::instance ().add_edge (&_get, &_value);
    AbstractDeref::update_src ();
    Process::finalize_construction (parent, name);
  }

  DerefDouble::~DerefDouble()
  {
    Graph::instance ().remove_edge (&_value, &_set);
    Graph::instance ().remove_edge (&_get, &_value);
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
  DerefDouble::change_src (Process *src)
  {
    _src = dynamic_cast<AbstractProperty*> (src);
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
