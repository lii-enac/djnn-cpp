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

#include "dictionary.h"

#include "core/core-dev.h" // graph add/remove edge

namespace djnn
{
  void
  Dictionary::FindAction::impl_activate ()
  {
    Dictionary* d = (Dictionary*)get_parent ();
    auto * key = d->key ()->get_value ();
    auto & map = d->get_map ();
    auto it = map.find (key);
    if (it != map.end()) {
      d->set_value (it->second);
    } else {
      d->set_value (nullptr);
    }
  }

  void
  Dictionary::DelEntryAction::impl_activate ()
  {
    Dictionary* d = (Dictionary*)get_parent ();
    auto * key = d->key ()->get_value ();
    auto & map = d->get_map ();
    auto it = map.find (key);
    if (it != map.end()) {
      map.erase(it);
    }
    FatProcess *p = nullptr;
    d->value ()->set_value (p, false);
    d->key ()->set_value (p, false);
  }

  void
  Dictionary::AddEntryAction::impl_activate ()
  {
    Dictionary* d = (Dictionary*)get_parent ();
    auto * key = d->key ()->get_value ();
    auto * value = d->value ()->get_value ();
    d->get_map ()[key] = value;
  }

  Dictionary::Dictionary (CoreProcess* parent, const string& name)
  : FatProcess (name), 
    _key (this, "key", nullptr),
    _value (this, "value", nullptr),
     _add (this, "add"),
    _del (this, "del"),
    _find_action (this, "find_action"),
     _add_action (this, "add_action"),
    _del_action (this, "del_action"),
    _c_add (&_add, ACTIVATION, &_add_action, ACTIVATION),
    _c_del (&_del, ACTIVATION, &_del_action, ACTIVATION),
    _c_find (&_key, ACTIVATION, &_find_action, ACTIVATION),
    _c_add_find (&_add, ACTIVATION, &_find_action, DEACTIVATION)

  {
    _c_add.disable ();
    _c_del.disable ();
    _c_find.disable ();
    _c_add_find.disable ();
    graph_add_edge (&_find_action, &_value);
    finalize_construction (parent, name);
  }
  void
  Dictionary::impl_activate ()
  {
    _c_add.enable ();
    _c_del.enable ();
    _c_find.enable ();
    _c_add_find.enable ();
  }
  void
  Dictionary::impl_deactivate ()
  {
    _c_add.disable ();
    _c_del.disable ();
    _c_find.disable ();
    _c_add_find.disable ();
  }
  void
  Dictionary::add_entry (CoreProcess* key, CoreProcess *value)
  {
    _map[key] = value;
  }

  CoreProcess*
  Dictionary::get_value (CoreProcess* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      return it->second;
    }
    return nullptr;
  }

  void
  Dictionary::del_entry (CoreProcess* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      _map.erase (it);
    }
  }

  Dictionary::~Dictionary () {
    graph_remove_edge (&_find_action, &_value);
  }
}
