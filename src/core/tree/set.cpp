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
#include "../serializer/serializer.h"
#include "../utils/error.h"

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
      Process (name)
  {
    _added = new RefProperty (nullptr);
    _removed = new RefProperty (nullptr);
    _size = new IntProperty (nullptr, "size", 0);
    Process::finalize_construction (parent);
  }

  Set::~Set ()
  {
    delete _size;
    delete _removed;
    delete _added;
  }

  void
  Set::add_child (Process* c, const std::string& name)
  {
    /* test if the child is already in the set
     * it == symtable ().end () if the element is not find 
     * only then we add it in the symtable to avoid duplicate
     */
    map<string, Process*>::iterator it;
    it = symtable ().find(name);
    if (it == symtable ().end ()) {
      Process::add_child (c, name);
      c->set_parent (this);

      if (get_activation_state () == ACTIVATED) {
        c->activate ();
      } else if (c->get_activation_state () == ACTIVATED) {
        c->deactivate ();
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
    for (it = symtable ().begin (); it != symtable ().end (); ++it) {
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
    for (it = symtable ().begin (); it != symtable ().end (); ++it) {
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
  Set::impl_activate ()
  {
    std::map<std::string, Process*>::iterator it;
    for (it = symtable ().begin (); it != symtable ().end (); ++it) {
      it->second->activate ();
    }
  }

  void
  Set::impl_deactivate ()
  {
    std::map<std::string, Process*>::iterator it;
    for (it = symtable ().begin (); it != symtable ().end (); ++it) {
      it->second->deactivate ();
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
  Set::dump (int level)
  {
    cout << (get_parent () ? get_parent ()->find_component_name(this) : get_name ())  << " [ cardinality=" << _size->get_value () << " ]" << endl ;
  }

  void
  Set::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:set");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    std::map<std::string, Process*>::iterator it;
    for (it = symtable ().begin (); it != symtable ().end (); ++it)
      it->second->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

  SetIterator::SetIterator (Process *parent, const string &name, Process *set, Process *action, bool model) :
      Process (name, model), _set (set), _action (action)
  {
    Set *s = dynamic_cast<Set*> (set);
    if (s == nullptr)
      error (this, "The set argument must be a Set component in set iterator " + name);
    Process::finalize_construction (parent);
  }

  void
  SetIterator::impl_activate ()
  {
    std::map<std::string, Process*>::iterator it;
    std::map<std::string, Process*>& map = _set->symtable ();
    for (it = map.begin (); it != map.end (); ++it) {
      _action->set_data (it->second);
      _action->activate ();
    }
    notify_activation ();
  }

  void
  SetIterator::post_activate ()
  {
    set_activation_state (DEACTIVATED);
  }
}

