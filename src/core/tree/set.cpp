/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "set.h"
#include "../execution/graph.h"
#include "../execution/component_observer.h"
#include "../serializer/serializer.h"
#include "../error.h"

#include <algorithm>

namespace djnn
{
  using namespace std;

  Set::Set () :
      Process ()
  {
    _added = new RefProperty (nullptr);
    _removed = new RefProperty (nullptr);
    _size = new IntProperty (0);
  }

  Set::Set (Process* parent, const string& name) :
      Process (parent, name)
  {
    _added = new RefProperty (nullptr);
    _removed = new RefProperty (nullptr);
    _size = new IntProperty (0);
    Process::finalize ();
  }

  Set::~Set ()
  {
    _added = nullptr;
    _removed = nullptr;

    delete _size;
    delete _removed;
    delete _added;
  }

  void
  Set::add_child (Process* c, const std::string& name)
  {
    /* test if the child is already in the set
     * it == _symtable.end () if the element is not find 
     * only then we add it in the symtable to avoid duplicate
     */
    map<string, Process*>::iterator it;
    it = _symtable.find(name);
    if (it == _symtable.end ()) {
      Process::add_child (c, name);
      c->set_parent (this);

      //if (get_state () == activated) {
      if (is_activated () ) {
        c->activation ();
      //} else if (c->get_state () == activated) {
      } else if (c->is_activated ()) {
        c->deactivation ();
      }
      _added->set_value (c, true);
      _size->set_value (_size->get_value () + 1, true);
    }

  }

  void
  Set::remove_child (Process* c)
  {
    bool found = false;
    std::map<std::string, Process*>::iterator it;
    string symbol;
    for (it = _symtable.begin (); it != _symtable.end (); ++it) {
      if (it->second == c) {
        symbol = it->first;
        found = true;
        continue;
      }
    }
    if (found) {
      remove_symbol (symbol);
      _removed->set_value (c, true);
      _size->set_value (_size->get_value () - 1, true);
    }
  }

  void
  Set::remove_child (const std::string& name)
  {
    Process *found = nullptr;
    std::map<std::string, Process*>::iterator it;
    for (it = _symtable.begin (); it != _symtable.end (); ++it) {
      if (it->second->get_name ().compare (name) == 0) {
        remove_symbol (it->first);
        found = it->second;
        continue;
      }
    }
    if (found) {
      _removed->set_value (found, true);
      _size->set_value (_size->get_value () - 1, true);
    }
  }

  void
  Set::activate ()
  {
    std::map<std::string, Process*>::iterator it;
    for (it = _symtable.begin (); it != _symtable.end (); ++it) {
      it->second->activation ();
    }
  }

  void
  Set::deactivate ()
  {
    std::map<std::string, Process*>::iterator it;
    for (it = _symtable.begin (); it != _symtable.end (); ++it) {
      it->second->deactivation ();
    }
  }

  Process*
  Set::find_component (const string& path)
  {
    if (path.compare ("$added") == 0)
      return _added;
    else if (path.compare ("$removed") == 0)
      return _removed;
    else if (path.compare ("size") == 0)
      return _size;
    else {
      return Process::find_component (path);
    }
    return nullptr;
  }

  void
  Set::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:set");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    std::map<std::string, Process*>::iterator it;
    for (it = _symtable.begin (); it != _symtable.end (); ++it)
      it->second->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  SetIterator::SetIterator (Process *parent, const string &name, Process *set, Process *action, bool model) :
      Process (parent, name, model), _set (set), _action (action)
  {
    Set *s = dynamic_cast<Set*> (set);
    if (s == nullptr)
      error (this, "The set argument must be a Set component in set iterator " + name);
    Process::finalize ();
  }

  void
  SetIterator::activate ()
  {
    std::map<std::string, Process*>::iterator it;
    std::map<std::string, Process*>& map = _set->symtable ();
    for (it = map.begin (); it != map.end (); ++it) {
      _action->set_data (it->second);
      _action->activation ();
    }
    notify_activation ();
  }

  void
  SetIterator::post_activate ()
  {
    //_activation_state = deactivated;
    set_deactivated ();
  }
}

