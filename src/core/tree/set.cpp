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

#include "core/execution/graph.h"
#include "core/serializer/serializer.h"
#include "core/utils/error.h"

#include <algorithm>

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include <iostream>
#endif

namespace djnn
{
  using namespace std;

  Set::Set (Process* parent, const std::string& name)
  :
    Process (name),
    _added (nullptr, "_added", nullptr),
    _removed (nullptr, "_removed", nullptr),
    _size (nullptr, "size", 0)
  {
    Process::finalize_construction (parent, name);
  }

  Set::~Set ()
  {
  }

  void
  Set::add_child (Process* c, const std::string& name)
  {
    /* test if the child is already in the set
     * it == children ().end () if the element is not find 
     * only then we add it in the symtable to avoid duplicate
     */
    map<string, Process*>::iterator it;
    it = children ().find(name);
    if (it == children ().end ()) {
      Process::add_child (c, name);
      c->set_parent (this);

      if (get_activation_state () == ACTIVATED) {
        c->activate ();
      } else if (c->get_activation_state () == ACTIVATED) {
        c->deactivate ();
      }
      _added.set_value (c, true);
      _size.set_value (_size.get_value () + 1, true);
    }

  }

  void
  Set::remove_child (Process* c)
  {
    bool found = false;
    symtable_t::iterator it;
    string symbol;
    for (it = children ().begin (); it != children ().end (); ++it) {
      if (it->second == c) {
        symbol = it->first;
        found = true;
        continue;
      }
    }
    if (found) {
      remove_symbol (symbol);
      _removed.set_value (c, true);
      _size.set_value (_size.get_value () - 1, true);
    }
  }

  void
  Set::remove_child (const std::string& name)
  {
    Process *found = nullptr;
    symtable_t::iterator it;
    for (it = children ().begin (); it != children ().end (); ++it) {
      if (it->second->get_name ().compare (name) == 0) {
        remove_symbol (it->first);
        found = it->second;
        continue;
      }
    }
    if (found) {
      _removed.set_value (found, true);
      _size.set_value (_size.get_value () - 1, true);
    }
  }

  void
  Set::impl_activate ()
  {
    symtable_t::iterator it;
    for (it = children ().begin (); it != children ().end (); ++it) {
      it->second->activate ();
    }
  }

  void
  Set::impl_deactivate ()
  {
    symtable_t::iterator it;
    for (it = children ().begin (); it != children ().end (); ++it) {
      it->second->deactivate ();
    }
  }

  Process*
  Set::find_child (const std::string& path)
  {
    if (path.compare ("$added") == 0)
      return &_added;
    else if (path.compare ("$removed") == 0)
      return &_removed;
    else if (path.compare ("size") == 0)
      return &_size;
    else {
      return Process::find_child (path);
    }
    return nullptr;
  }

#if !defined(DJNN_NO_DEBUG)
  void
  Set::dump (int level)
  {
    cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ())  << " [ cardinality=" << _size.get_value () << " ]" << endl ;
  }
#endif

#if !defined(DJNN_NO_SERIALIZE)
  void
  Set::serialize (const std::string& type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:set");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    symtable_t::iterator it;
    for (it = children ().begin (); it != children ().end (); ++it)
      it->second->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

  SetIterator::SetIterator (Process *parent, const std::string& name, Process *set, Process *action, bool model) :
      Process (name, model), _set (set), _action (action)
  {
    Set *s = dynamic_cast<Set*> (set);
    if (s == nullptr)
      error (this, "The set argument must be a Set component in set iterator " + name);
    Process::finalize_construction (parent, name);
  }

  void
  SetIterator::impl_activate ()
  {
    symtable_t::iterator it;
    std::map<std::string, Process*>& map = _set->children ();
    for (it = map.begin (); it != map.end (); ++it) {
      _action->set_data (it->second);
      _action->activate ();
    }
    notify_activation ();
  }

  void
  SetIterator::post_activate ()
  {
    Process::Post_activate_auto_deactivate ();
  }
}

