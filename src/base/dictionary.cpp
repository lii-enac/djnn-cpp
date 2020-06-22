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

#include "core/execution/graph.h"

namespace djnn
{
  void
  Dictionary::FindAction::impl_activate ()
  {
    Dictionary* d = (Dictionary*)get_parent ();
    auto * key = dynamic_cast<FatProcess*>(d->key ()->get_value ());
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
    auto * key = dynamic_cast<FatProcess*>(d->key ()->get_value ());
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
    auto * key = dynamic_cast<FatProcess*>(d->key ()->get_value ());
    auto * value = dynamic_cast<FatProcess*>(d->value ()->get_value ());
    d->get_map ()[key] = value;
  }

  Dictionary::Dictionary (ParentProcess* parent, const std::string& name)
  : FatProcess (name), 
    _key (this, "key", nullptr),
    _value (this, "value", nullptr),
     _add (this, "add"),
    _del (this, "delete"),
    _find_action (this, "find_action"),
     _add_action (this, "add_action"),
    _del_action (this, "del_action"),
    _c_add (&_add, ACTIVATION, &_add_action, ACTIVATION, true),
    _c_del (&_del, ACTIVATION, &_del_action, ACTIVATION, true),
    _c_find (&_key, ACTIVATION, &_find_action, ACTIVATION, true)

  {
    _c_add.disable ();
    _c_del.disable ();
    _c_find.disable ();
    Graph::instance ().add_edge (&_add, &_add_action);
    Graph::instance ().add_edge (&_del, &_del_action);
    Graph::instance ().add_edge (&_key, &_find_action);
    finalize_construction (parent, name);
  }
  void
  Dictionary::impl_activate ()
  {
    _c_add.enable ();
    _c_del.enable ();
    _c_find.enable ();
  }
  void
  Dictionary::impl_deactivate ()
  {
    _c_add.disable ();
    _c_del.disable ();
    _c_find.disable ();
  }
  void
  Dictionary::add_entry (FatProcess* key, FatProcess *value)
  {
    _map[key] = value;
  }

  FatProcess*
  Dictionary::get_value (FatProcess* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      return it->second;
    }
    return nullptr;
  }

  void
  Dictionary::del_entry (FatProcess* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      _map.erase (it);
    }
  }

  Dictionary::~Dictionary () {
    Graph::instance ().remove_edge (&_add, &_add_action);
    Graph::instance ().remove_edge (&_del, &_del_action);
    Graph::instance ().remove_edge (&_key, &_find_action);
  }
}
