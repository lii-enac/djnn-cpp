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

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"

#include <algorithm>



namespace djnn
{
  

  Set::Set (CoreProcess* parent, const string& name)
  :
    FatProcess (name),
    _added (nullptr, "_added", nullptr),
    _removed (nullptr, "_removed", nullptr),
    _size (nullptr, "_size", 0)
  {
    remove_from_parentless_name (&_added);
    remove_from_parentless_name (&_removed);
    remove_from_parentless_name (&_size);
    finalize_construction (parent, name);
  }

  Set::~Set ()
  {
  }

  void
  Set::add_child (CoreProcess* c, const string& name)
  {
    /* test if the child is already in the set
     * it == symtable ().end () if the element is not find 
     * only then we add it in the symtable to avoid duplicate
     */
    auto it = symtable ().find(name);
    if (it == symtable ().end ()) {
      FatProcess::add_child (c, name);
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
  Set::remove_child (CoreProcess* c)
  {
    bool found = false;
    //symtable_t::iterator it;
    string symbol;
    for (auto it = symtable ().begin (); it != symtable ().end (); ++it) {
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
  Set::remove_child (const string& name)
  {
    CoreProcess *found = nullptr;
    string symbol;
    for (auto it = symtable ().begin (); it != symtable ().end (); ++it) {
      //if (it->second->get_name ().compare (name) == 0) {
      if (it->first.compare (name) == 0) {
        symbol = it->first;
        found = it->second;
        continue;
      }
    }
    if (found) {
      remove_symbol (symbol);
      _removed.set_value (found, true);
      _size.set_value (_size.get_value () - 1, true);
    }
  }

  void
  Set::impl_activate ()
  {
    for (auto it = symtable ().begin (); it != symtable ().end (); ++it) {
      it->second->activate ();
    }
  }

  void
  Set::impl_deactivate ()
  {
    for (auto it = symtable ().begin (); it != symtable ().end (); ++it) {
      it->second->deactivate ();
    }
  }

  CoreProcess*
  Set::find_child_impl (const string& path)
  {
    if (path.compare ("$added") == 0)
      return &_added;
    else if (path.compare ("$removed") == 0)
      return &_removed;
    else if (path.compare ("size") == 0)
      return &_size;
    else {
      return FatProcess::find_child_impl (path);
    }
    return nullptr;
  }

#ifndef DJNN_NO_DEBUG
  void
  Set::dump (int level)
  {
    loginfonocr ( (get_parent () ? get_parent ()->find_child_name(this) : get_name ())  + " [ cardinality=" + __to_string(_size.get_value ()) + " ]\n") ;
    for (auto it = symtable ().begin (); it != symtable ().end (); ++it) {
      loginfonocr ( string("\t- ") + it->first + "\n");
    }
  }
#endif

#if !defined(DJNN_NO_SERIALIZE)
  void
  Set::serialize (const string& type)
  {
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:set");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto it = symtable ().begin (); it != symtable ().end (); ++it)
      it->second->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }
#endif

  SetIterator::SetIterator (CoreProcess* parent, const string& name, CoreProcess *set, CoreProcess *action, bool model) :
      FatProcess (name, model), _set (dynamic_cast<Set*> (set)), _action (action)
  {
    if (_set == nullptr)
      error (this, "The set argument must be a Set component in set iterator " + name);

    finalize_construction (parent, name);
  }

  void
  SetIterator::impl_activate ()
  {
    auto & map = _set->symtable ();
    for (auto it = map.begin (); it != map.end (); ++it) {
      _action->set_data (it->second);
      _action->activate ();
    }
    notify_activation ();
  }

  void
  SetIterator::post_activate ()
  {
    post_activate_auto_deactivate ();
  }
}

