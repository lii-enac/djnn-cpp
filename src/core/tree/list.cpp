/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <stdexcept>

#include "list.h"

#include "spike.h"
#include "core/ontology/coupling.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "core/tree/structure_observer.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"

#include <algorithm>

#if !defined(DJNN_NO_DEBUG)
//#include <__iostream>
#endif

namespace djnn
{
  using namespace std;

  AbstractList::AbstractList (ParentProcess* parent, const std::string& name)
  :
    Container (parent, name),
    _added (nullptr, "_added", nullptr),
    _removed (nullptr, "_removed", nullptr),
    _size (nullptr, "_size", 0)
  {
  }

#if !defined(DJNN_NO_DEBUG)
  void
  AbstractList::dump (int level)
  {
    //cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ())  << " [ index=" << _children.size () << " ]" << endl ;
    loginfonofl ( (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) + " [ index=" + __to_string(_children.size ()) + " ]");
    //FIXME: indent problem
    //for (auto c : _children)
    //  c->dump(level);
  }
#endif

  void
  AbstractList::add_child (FatChildProcess* c, const std::string& name)
  {
    if (c == nullptr)
      return;
    _children.push_back (c);
    for (auto s: structure_observer_list) {
      s->add_child_to_container (this, c, _children.size () - 1);
    }
    finalize_child_insertion (c);
  }

  void
  AbstractList::insert (FatChildProcess* c, const std::string& spec)
  {
    int index = _children.size () - 1;
    if (spec.compare (">") == 0) {
      add_child (c, "");
      for (auto s: structure_observer_list) {
        s->add_child_to_container (this, c, index);
      }
      return;
    }
    if (spec.compare ("<") == 0) {
      auto it = _children.begin ();
      it = _children.insert (it, c);
      finalize_child_insertion (c);
      index = 0;
      for (auto s: structure_observer_list) {
        s->add_child_to_container (this, c, index);
      }
      return;
    }
    try {
      index = std::stoi (spec.substr (1, string::npos)) - 1;
      if (spec.at (0) == '<') {
        auto it = _children.begin () + index;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        for (auto s: structure_observer_list) {
          s->add_child_to_container (this, c, index);
        }
        return;
      } else if (spec.at (0) == '>') {
        auto it = _children.begin () + index + 1;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        for (auto s: structure_observer_list) {
          s->add_child_to_container (this, c, index + 1);
        }
        return;
      } else {
        goto label_error;
      }
    }
    catch (invalid_argument& arg) {
      goto label_error;
    }
    label_error: 
      warning (this, "invalid specification '" + spec + "' for insertion in list '" + get_name () + "'");
  }

  void
  AbstractList::clear ()
  {
    /* empty _children without calling delete on each element IF they are pointers */
    _children.clear ();
    _size.set_value (0, true);
  }

  void
  AbstractList::remove_child (FatChildProcess* c)
  {
    auto newend = children ().end ();
    for (auto s: structure_observer_list) {
      s->remove_child_from_container (this, c);
    }
    /* remove if 'c' is found in the vector */
    newend = std::remove_if (_children.begin (), _children.end (), 
        [c](ordered_children_t::iterator::value_type v) { return v == c; });

    /* check if end has changed and erase if necessary */
    if (newend != _children.end ()){
      _children.erase(newend, _children.end ());
       _removed.set_value (c, true);
      _size.set_value (_size.get_value () - 1, true);
    }
  }

  void
  AbstractList::remove_child (const std::string& name)
  {
    size_t index;
    try {
      /* from user index to internal index : -1 */
      index = std::stoi (name, nullptr) - 1;
      if (index < _children.size ()) {
        auto * c = _children.at (index);
        remove_child (c);
      } else {
         /* we have to dispay index as the API user index */
         warning (this, "index " + std::to_string(index+1) + " is out of bound for list '" + get_name () + "'");
      }
    }
    catch (invalid_argument& arg) {
      warning (this, "invalid child name '" + name + "' for list '" + get_name () + "'");
    }
  }

  FatChildProcess*
  AbstractList::find_child_impl (const std::string& path)
  {
    if (path.compare ("$added") == 0)
      return &_added;
    else if (path.compare ("$removed") == 0)
      return &_removed;
    else if (path.compare ("size") == 0)
      return &_size;
    else {
      try {
        string::size_type sz;
        size_t index = std::stoi (path, &sz) - 1;
        if (index < _children.size ()) {
          auto * c = _children.at (index);
          if (path.length () > sz) {
            return c->find_child_impl (path.substr (sz + 1));
          } else
            return c;
        } else {
          /* we have to dispay index as the API user index */
          warning (this,  "index " + std::to_string(index+1) + " is out of bound for list \'" + get_name () + "\'");
        }
      }
      catch (invalid_argument& arg) {
        //FIXME: comment the warning for now because it polluted the find_child ("//*")
        //warning (this, "invalid child path '" + path + "' for list '" + get_name () + "'");
      }
    }
    return nullptr;
  }

  FatChildProcess*
  AbstractList::find_child_impl (int index)
  {
    if ((index - 1) < (int)_children.size ()) {
      return _children.at (index - 1);

    } else {
      /* we have to dispay index as the API user index */
      warning (this, "index " + std::to_string (index) + " is out of bound for list \'" + get_name () + "\'");
    }
    return nullptr;
  }

  List::List (ParentProcess* parent, const std::string& name) :
    AbstractList (parent, name)
  {
    finalize_construction (parent, name);
  }

  List::~List ()
  {
  }

  void
  List::finalize_child_insertion (FatChildProcess *c)
  {
    c->set_parent (this);

    if (get_activation_state () == ACTIVATED && c->get_activation_state () == DEACTIVATED) {
      c->activate ();
    } else if (get_activation_state () == DEACTIVATED && c->get_activation_state () == ACTIVATED) {
      c->deactivate ();
    }
    _added.set_value (c, true); // fixme
    _size.set_value (_size.get_value () + 1, true);
  }

  List*
  List::clone () {
    List* clone = new List (nullptr, get_name ());
    for (auto c: _children) {
      clone->add_child (c->clone (), "");
    }
    return clone;
  }

#if !defined(DJNN_NO_SERIALIZE)
  void
  List::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  ListIterator::ListIterator (ParentProcess* parent, const std::string& name, CoreProcess *list, CoreProcess *action, bool model)
  :
    FatProcess (name, model),
    _action (action)
  {
    List *l = dynamic_cast<List*> (list);
    if (l == nullptr)
      error (this, "The list argument must be a List component in list iterator " + name);
    _list = l;
    finalize_construction (parent, name);
  }

  void
  ListIterator::impl_activate ()
  {
    for (auto p : _list->children ()) {
      _action->set_data (p);
      _action->activate ();
    }
    notify_activation ();
  }

  void
  ListIterator::post_activate ()
  {
    post_activate_auto_deactivate ();
  }

  BidirectionalListIterator::IterAction::IterAction (ParentProcess* parent, const std::string& name, CoreProcess *list,
                                                     RefProperty *iter, IntProperty *index,
                                                     bool forward) :
      Action (parent, name), _list (dynamic_cast<List*>(list)), _iter (iter), _index (index), _forward (forward)
  {
    if (_list == nullptr)
      error (this, "The set argument must be a List component in set iterator " + name);
  }

  void
  BidirectionalListIterator::IterAction::impl_activate ()
  {
    if (get_parent ()->somehow_deactivating () )
      return;
    int index = _index->get_value ();
    if (_forward) {
      if (_list->size () > index) {
        _iter->set_value (_list->children ().at (index - 1), true);
        _index->set_value (index + 1, true);
      }
    } else {
      if (index >= 1) {
        _iter->set_value (_list->children ().at (index - 1), true);
        _index->set_value (index - 1, true);
      }
    }
  }

  BidirectionalListIterator::ResetAction::ResetAction (ParentProcess* parent, const std::string& name,
                                                       IntProperty *index) :
      Action (parent, name), _index (index)
  {
  }

  void
  BidirectionalListIterator::ResetAction::impl_activate ()
  {
    if (get_parent ()->somehow_deactivating ())
      return;
    _index->set_value (1, true);
  }

  BidirectionalListIterator::BidirectionalListIterator (ParentProcess* parent, const std::string& name,
                                                        List* list)
  :
  FatProcess (name),
  _list (dynamic_cast<List*> (list)),
  _next (this, "next"),
  _previous (this, "previous"),
  _reset (this, "reset"),
  _iter (this, "iter", nullptr),
  _index (this, "index", 1),
  _next_action (this, name + "_next_action", _list, &_iter, &_index, true),
  _c_next (&_next, ACTIVATION, &_next_action, ACTIVATION),
  _previous_action (this, name + "_previous_action", _list, &_iter, &_index, false),
  _c_previous (&_previous, ACTIVATION, &_previous_action, ACTIVATION),
  _reset_action (this, name + "_reset_action", &_index),

  _c_reset (&_reset, ACTIVATION, &_reset_action, ACTIVATION)
  {
    
    if (_list == nullptr) {
      warning (this, "list iterator must take a List as its third argument\n");
      return;
    }
    
    _c_next.disable ();
    _c_previous.disable ();
    _c_reset.disable ();
    finalize_construction (parent, name);
  }

  BidirectionalListIterator::~BidirectionalListIterator ()
  {
    remove_state_dependency (get_parent (), &_next_action);
    remove_state_dependency (get_parent (), &_previous_action);
    remove_state_dependency (get_parent (), &_reset_action);
  }

  void
  BidirectionalListIterator::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_next_action);
       remove_state_dependency (get_parent (), &_previous_action);
       remove_state_dependency (get_parent (), &_reset_action);
    }

    add_state_dependency (parent, &_next_action);
    add_state_dependency (parent, &_previous_action);
    add_state_dependency (parent, &_reset_action);

    FatProcess::set_parent (parent); 
  }

  void
  BidirectionalListIterator::impl_activate ()
  {
    _c_next.enable ();
    _c_previous.enable ();
    _c_reset.enable ();
  }

  void
  BidirectionalListIterator::impl_deactivate ()
  {
    _c_next.disable ();
    _c_previous.disable ();
    _c_reset.disable ();
  }

#if !defined(DJNN_NO_SERIALIZE)
  void
  BidirectionalListIterator::serialize (const std::string& type) {
   
    string buf;

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:bidirectionallistiterator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _list, buf);
    AbstractSerializer::serializer->text_attribute ("list", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

}

