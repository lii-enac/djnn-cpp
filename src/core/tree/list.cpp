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

#include "list.h"
#include "spike.h"
#include "../control/coupling.h"
#include "../execution/graph.h"
#include <algorithm>
#include "../execution/component_observer.h"
#include "../serializer/serializer.h"
#include "../error.h"

namespace djnn
{
  using namespace std;

  List::List () :
      Container ()
  {
    _cpnt_type = COMPONENT;
    _added = std::unique_ptr<RefProperty> (new RefProperty (nullptr));
    _removed = std::unique_ptr<RefProperty> (new RefProperty (nullptr));
    _size = std::unique_ptr<IntProperty> (new IntProperty (0));
  }

  List::List (Process* parent, const string& name) :
      Container (parent, name)
  {
    _cpnt_type = COMPONENT;
    _added = std::unique_ptr<RefProperty> (new RefProperty (nullptr));
    _removed = std::unique_ptr<RefProperty> (new RefProperty (nullptr));
    _size = std::unique_ptr<IntProperty> (new IntProperty (0));
    Process::finalize ();
  }

  List::~List ()
  {
  }
  
  void
  List::dump (int level)
  {
    cout << _name  << " [ index=" << _children.size () << " ]" << endl ;

    //FIXME: indent problem
    //for (auto c : _children)
    //  c->dump(level);
  }

  void
  List::finalize_child_insertion (Process *c)
  {
    c->set_parent (this);

    if (get_state () == activated && c->get_state () == deactivated) {
      c->activation ();
    } else if (get_state () == deactivated && c->get_state () == activated) {
      c->deactivation ();
    }
    _added->set_value (c, true);
    _size->set_value (_size->get_value () + 1, true);
  }

  void
  List::add_child (Process* c, const std::string& name)
  {
    if (c == nullptr)
      return;
    _children.push_back (c);

    finalize_child_insertion (c);
  }

  void
  List::insert (Process* c, const std::string& spec)
  {
    if (spec.compare (">") == 0) {
      add_child (c, "");
      return;
    }
    std::vector<Process*>::iterator it;
    if (spec.compare ("<") == 0) {
      it = _children.begin ();
      it = _children.insert (it, c);
      finalize_child_insertion (c);
      return;
    }
    try {
      int index = std::stoi (spec.substr (1, string::npos)) - 1;
      if (spec.at (0) == '<') {
        it = _children.begin () + index;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        return;
      } else if (spec.at (0) == '>') {
        it = _children.begin () + index + 1;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        return;
      } else {
        goto label_error;
      }
    }
    catch (invalid_argument& arg) {
      goto label_error;
    }
    label_error: 
      warning ("invalid specification '" + spec + "' for insertion in list '" + _name + "'");
  }

  void
  List::remove_child (Process* c)
  {
    std::vector<Process*>::iterator newend = _children.end ();

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
  List::remove_child (const std::string& name)
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
         warning ( "index " + std::to_string(index+1) + " is out of bound for list '" + _name + "'");
      }
    }
    catch (invalid_argument& arg) {
      warning ("invalid child name '" + name + "' for list '" +_name + "'");
    }
  }

  Process*
  List::find_component (const string& path)
  {
    if (path.compare ("$added") == 0)
      return _added.get ();
    else if (path.compare ("$removed") == 0)
      return _removed.get ();
    else if (path.compare ("size") == 0)
      return _size.get ();
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
          warning ( "index " + std::to_string(index+1) + " is out of bound for list \'" + _name + "\'");
        }
      }
      catch (invalid_argument& arg) {
        warning ("invalid child path '" + path + "' for list '" + _name + "'");
      }
    }
    return nullptr;
  }

  Process*
  List::clone () {
    List* clone = new List ();
    for (auto c: _children) {
      clone->add_child (c->clone (), c->get_name ());
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
                                                     shared_ptr<RefProperty> iter, shared_ptr<IntProperty> index,
                                                     bool forward) :
      Process (parent, name), _list (list), _iter (iter), _index (index), _forward (forward)
  {
  }

  void
  BidirectionalListIterator::IterAction::activate ()
  {
    if (_parent->get_state () > activated)
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
                                                       shared_ptr<IntProperty> index) :
      Process (parent, name), _index (index)
  {
  }

  void
  BidirectionalListIterator::ResetAction::activate ()
  {
    if (_parent->get_state () > activated)
      return;
    _index->set_value (1, true);
  }

  BidirectionalListIterator::BidirectionalListIterator (Process *parent, const string& name,
                                                        Process* list) :
      Process (parent, name)
  {
    _list = dynamic_cast<List*> (list);
    if (_list == nullptr) {
      warning ("list iterator must take a List as its third argument\n");
      return;
    }
    _next = make_shared<Spike> (this, "next");
    _previous = make_shared<Spike> (this, "previous");
    _reset = make_shared<Spike> (this, "reset");
    _iter = make_shared<RefProperty> (this, "iter", nullptr);
    _index = make_shared<IntProperty> (this, "index", 1);
    _next_action = make_unique<IterAction> (this, name + "_next_action", _list, _iter, _index, true);
    _previous_action = make_unique<IterAction> (this, name + "_previous_action", _list, _iter, _index, false);
    _reset_action = make_unique<ResetAction> (this, name + "_reset_action", _index);
    _c_next = make_unique<Coupling> (_next.get (), ACTIVATION, _next_action.get (), ACTIVATION);
    _c_next->disable ();
    _c_previous = make_unique<Coupling> (_previous.get (), ACTIVATION, _previous_action.get (), ACTIVATION);
    _c_previous->disable ();
    _c_reset = make_unique<Coupling> (_reset.get (), ACTIVATION, _reset_action.get (), ACTIVATION);
    _c_next->disable ();
    Graph::instance ().add_edge (_next.get (), _next_action.get ());
    Graph::instance ().add_edge (_previous.get (), _previous_action.get ());
    Graph::instance ().add_edge (_reset.get (), _reset_action.get ());
    Process::finalize ();
  }

  BidirectionalListIterator::~BidirectionalListIterator ()
  {
    Graph::instance ().remove_edge (_next.get (), _next_action.get ());
    Graph::instance ().remove_edge (_previous.get (), _previous_action.get ());
    Graph::instance ().remove_edge (_reset.get (), _reset_action.get ());
  }

  void
  BidirectionalListIterator::activate ()
  {
    _c_next->enable ();
    _c_previous->enable ();
    _c_reset->enable ();
  }

  void
  BidirectionalListIterator::deactivate ()
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
    List *l = dynamic_cast<List*> (list);
    if (l == nullptr)
      error ("The list argument must be a List component in list iterator " + name);
    _list = l;
    Process::finalize ();
  }

  void
  ListIterator::activate ()
  {
    for (auto p : _list->children ()) {
      _action->set_data (p);
      _action->activation ();
    }
    notify_activation ();
  }

  void
  ListIterator::post_activate ()
  {
    _activation_state = deactivated;
  }
}

