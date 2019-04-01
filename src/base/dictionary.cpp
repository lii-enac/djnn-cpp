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

#include <iostream>

namespace djnn
{
  using namespace std;

  void
  Dictionary::FindAction::activate ()
  {
    Dictionary* d = (Dictionary*)_parent;
    Process* key = d->key ()->get_value ();
    auto it = d->get_map ().find (key);
    if (it != d->get_map ().end()) {
      d->set_value (it->second);
    } else {
      d->set_value (nullptr);
    }
  }

  void
  Dictionary::DelEntryAction::activate ()
  {
    Dictionary* d = (Dictionary*)_parent;
    Process* key = d->key ()->get_value ();
    auto it = d->get_map ().find (key);
    if (it != d->get_map ().end()) {
      d->get_map ().erase(it);
    }
    Process *p = nullptr;
    d->value ()->set_value (p, false);
    d->key ()->set_value (p, false);
  }

  void
  Dictionary::AddEntryAction::activate ()
  {
    Dictionary* d = (Dictionary*)_parent;
    Process* key = d->key ()->get_value ();
    Process* value = d->value ()->get_value ();
    d->get_map ()[key] = value;
  }

  Dictionary::Dictionary (Process *parent, const string &name) : Process (parent, name)
  {
    _key = new RefProperty (this, "key", nullptr);
    _value = new RefProperty (this, "value", nullptr);
    _find_action = new FindAction (this, "find_action");
    _del_action = new DelEntryAction (this, "del_action");
    _add_action = new AddEntryAction (this, "add_action");
    _add = new Spike (this, "add");
    _del = new Spike (this, "delete");
    _c_add = new Coupling (_add, ACTIVATION, _add_action, ACTIVATION, true);
    _c_add->disable ();
    _c_del = new Coupling (_del, ACTIVATION, _del_action, ACTIVATION, true);
    _c_del->disable ();
    _c_find = new Coupling (_key, ACTIVATION, _find_action, ACTIVATION, true);
    _c_find->disable ();
    Graph::instance ().add_edge (_add, _add_action);
    Graph::instance ().add_edge (_del, _del_action);
    Graph::instance ().add_edge (_key, _find_action);
    Process::finalize ();
  }
  void
  Dictionary::activate ()
  {
    _c_add->enable ();
    _c_del->enable ();
    _c_find->enable ();
  }
  void
  Dictionary::deactivate ()
  {
    _c_add->disable ();
    _c_del->disable ();
    _c_find->disable ();
  }
  void
  Dictionary::add_entry (Process* key, Process *value)
  {
    _map[key] = value;
  }

  Process*
  Dictionary::get_value (Process* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      return it->second;
    }
    return nullptr;
  }

  void
  Dictionary::del_entry (Process* key)
  {
    auto it = _map.find (key);
    if (it != _map.end()) {
      _map.erase (it);
    }
  }

  Dictionary::~Dictionary () {
    Graph::instance ().remove_edge (_add, _add_action);
    Graph::instance ().remove_edge (_del, _del_action);
    Graph::instance ().remove_edge (_key, _find_action);
    delete _c_add;
    delete _c_find;
    delete _c_del;
    delete _key;
    delete _value;
    delete _find_action;
    delete _del_action;
    delete _add_action;
    delete _add;
    delete _del;
  }
}
