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

#include "list.h"

#include "spike.h"
#include "../ontology/coupling.h"
#include "../execution/graph.h"
#include "../serializer/serializer.h"
#include "../utils/error.h"
#include "../core.h"

#include <algorithm>


namespace djnn
{
  using namespace std;

  AbstractList::AbstractList () :
      Container ()
  {
    _added = new RefProperty (nullptr);
    _removed = new RefProperty (nullptr);
    _size = new IntProperty (0);
  }

  AbstractList::AbstractList (Process* parent, const string& name) :
    Container (parent, name)
  {
    _added = new RefProperty (nullptr);
    _removed = new RefProperty (nullptr);
    _size = new IntProperty (0);
  }

  void
  AbstractList::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name)  << " [ index=" << _children.size () << " ]" << endl ;

    //FIXME: indent problem
    //for (auto c : _children)
    //  c->dump(level);
  }

  void
  AbstractList::add_child (Process* c, const std::string& name)
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
  AbstractList::insert (Process* c, const std::string& spec)
  {
    int index = _children.size () - 1;
    if (spec.compare (">") == 0) {
      add_child (c, "");
      for (auto s: structure_observer_list) {
        s->add_child_to_container (this, c, index);
      }
      return;
    }
    std::vector<Process*>::iterator it;
    if (spec.compare ("<") == 0) {
      it = _children.begin ();
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
        it = _children.begin () + index;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        for (auto s: structure_observer_list) {
          s->add_child_to_container (this, c, index);
        }
        return;
      } else if (spec.at (0) == '>') {
        it = _children.begin () + index + 1;
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
      warning (this, "invalid specification '" + spec + "' for insertion in list '" + _name + "'");
  }

  void
  AbstractList::remove_child (Process* c)
  {
    std::vector<Process*>::iterator newend = _children.end ();
    for (auto s: structure_observer_list) {
      s->remove_child_from_container (this, c);
    }
    /* remove if 'c' is found in the vector */
    newend = std::remove_if (_children.begin (), _children.end (), 
        [c](std::vector<Process*>::iterator::value_type v) { return v == c; });

    /* check if end has changed and erase if necessary */
    if (newend != _children.end ()){
      _children.erase(newend, _children.end ());
       _removed->set_value (c, true);
      _size->set_value (_size->get_value () - 1, true);
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
        Process* c = _children.at (index);
        remove_child (c);
      } else {
         /* we have to dispay index as the API user index */
         warning (this, "index " + std::to_string(index+1) + " is out of bound for list '" + _name + "'");
      }
    }
    catch (invalid_argument& arg) {
      warning (this, "invalid child name '" + name + "' for list '" +_name + "'");
    }
  }

  List::List () :
      AbstractList ()
  {
  }

  List::List (Process* parent, const string& name) :
    AbstractList (parent, name)
  {
    Process::finalize_construction (parent);
  }

  List::~List ()
  {
    delete _added; 
    delete _removed;
    delete _size;
  }

  void
  List::finalize_child_insertion (Process *c)
  {
    c->set_parent (this);

    if (get_activation_state () == ACTIVATED && c->get_activation_state () == DEACTIVATED) {
      c->activate ();
    } else if (get_activation_state () == DEACTIVATED && c->get_activation_state () == ACTIVATED) {
      c->deactivate ();
    }
    _added->set_value (c, true);
    _size->set_value (_size->get_value () + 1, true);
  }

  Process*
  AbstractList::find_component (const string& path)
  {
    if (path.compare ("$added") == 0)
      return _added;
    else if (path.compare ("$removed") == 0)
      return _removed;
    else if (path.compare ("size") == 0)
      return _size;
    else {
      try {
        string::size_type sz;
        size_t index = std::stoi (path, &sz) - 1;
        if (index < _children.size ()) {
          Process* c = _children.at (index);
          if (path.length () > sz) {
            return c->find_component (path.substr (sz + 1));
          } else
            return c;
        } else {
          /* we have to dispay index as the API user index */
          warning (this,  "index " + std::to_string(index+1) + " is out of bound for list \'" + _name + "\'");
        }
      }
      catch (invalid_argument& arg) {
        warning (this, "invalid child path '" + path + "' for list '" + _name + "'");
      }
    }
    return nullptr;
  }

  Process*
  List::clone () {
    List* clone = new List ();
    for (auto c: _children) {
      clone->add_child (c->clone (), "");
    }
    return clone;
  }

  void
  List::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:list");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  BidirectionalListIterator::IterAction::IterAction (Process *parent, const string& name, List *list,
                                                     RefProperty *iter, IntProperty *index,
                                                     bool forward) :
      Process (parent, name), _list (list), _iter (iter), _index (index), _forward (forward)
  {
  }

  void
  BidirectionalListIterator::IterAction::impl_activate ()
  {
    if (_parent->somehow_deactivating () )
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

  BidirectionalListIterator::ResetAction::ResetAction (Process *parent, const string& name,
                                                       IntProperty *index) :
      Process (parent, name), _index (index)
  {
  }

  void
  BidirectionalListIterator::ResetAction::impl_activate ()
  {
    if (_parent->somehow_deactivating ())
      return;
    _index->set_value (1, true);
  }

  BidirectionalListIterator::BidirectionalListIterator (Process *parent, const string& name,
                                                        Process* list) :
      Process (parent, name)
  {
    _list = dynamic_cast<List*> (list);
    if (_list == nullptr) {
      warning (this, "list iterator must take a List as its third argument\n");
      return;
    }
    _next = new Spike (this, "next");
    _previous = new Spike (this, "previous");
    _reset = new Spike (this, "reset");
    _iter = new RefProperty (this, "iter", nullptr);
    _index = new IntProperty (this, "index", 1);
    _next_action = new IterAction (this, name + "_next_action", _list, _iter, _index, true);
    _previous_action = new IterAction (this, name + "_previous_action", _list, _iter, _index, false);
    _reset_action = new ResetAction (this, name + "_reset_action", _index);
    _c_next = new Coupling (_next, ACTIVATION, _next_action, ACTIVATION);
    _c_next->disable ();
    _c_previous = new Coupling (_previous, ACTIVATION, _previous_action, ACTIVATION);
    _c_previous->disable ();
    _c_reset = new Coupling (_reset, ACTIVATION, _reset_action, ACTIVATION);
    _c_next->disable ();
    Graph::instance ().add_edge (_next, _next_action);
    Graph::instance ().add_edge (_previous, _previous_action);
    Graph::instance ().add_edge (_reset, _reset_action);
    Process::finalize_construction (parent);
  }

  BidirectionalListIterator::~BidirectionalListIterator ()
  {
    remove_state_dependency (_parent, _next_action);
    remove_state_dependency (_parent, _previous_action);
    remove_state_dependency (_parent, _reset_action);
    Graph::instance ().remove_edge (_next, _next_action);
    Graph::instance ().remove_edge (_previous, _previous_action);
    Graph::instance ().remove_edge (_reset, _reset_action);

    delete _c_reset;
    delete _c_previous;
    delete _c_next;
    delete _reset_action;
    delete _previous_action;
    delete _next_action;
    delete _index;
    delete _iter;
    delete _reset;
    delete _previous;
    delete _next;
  }

  void
  BidirectionalListIterator::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent) {
       remove_state_dependency (_parent, _next_action);
       remove_state_dependency (_parent, _previous_action);
       remove_state_dependency (_parent, _reset_action);
    }
    
    add_state_dependency (p, _next_action);
    add_state_dependency (p, _previous_action);
    add_state_dependency (p, _reset_action);

    _parent = p; 
  }

  void
  BidirectionalListIterator::impl_activate ()
  {
    _c_next->enable ();
    _c_previous->enable ();
    _c_reset->enable ();
  }

  void
  BidirectionalListIterator::impl_deactivate ()
  {
    _c_next->disable ();
    _c_previous->disable ();
    _c_reset->disable ();
  }

  void
  BidirectionalListIterator::serialize (const string& type) {
   
    string buf;

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:bidirectionallistiterator");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), _list, buf);
    AbstractSerializer::serializer->text_attribute ("list", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  ListIterator::ListIterator (Process *parent, const string &name, Process *list, Process *action, bool model) :
      Process (parent, name, model), _action (action)
  {
    Container *l = dynamic_cast<Container*> (list);
    if (l == nullptr)
      error (this, "The list argument must be a List component in list iterator " + name);
    _list = l;
    Process::finalize_construction (parent);
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
    set_activation_state (DEACTIVATED);
  }
}

