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
 *
 */

#include "list.h"
#include "spike.h"
#include "coupling.h"
#include "execution/graph.h"
#include <algorithm>
#include "execution/component_observer.h"

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
        it = _children.begin () + index - 1;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        return;
      } else if (spec.at (0) == '>') {
        it = _children.begin () + index + 1;
        it = _children.insert (it, c);
        finalize_child_insertion (c);
        return;
      } else {
        goto error;
      }
    }
    catch (invalid_argument& arg) {
      goto error;
    }
    error: cerr << "Invalid specification " << spec << " for insertion in list " << _name << endl;
  }

  void
  List::remove_child (Process* c)
  {
    bool found = false;
    for (auto cpnt : _children) {
      if (cpnt == c) {
        found = true;
        continue;
      }
    }
    if (found) {
      _children.erase (std::remove (_children.begin (), _children.end (), c), _children.end ());
      _removed->set_value (c, true);
      _size->set_value (_size->get_value () - 1, true);
    }
  }

  void
  List::remove_child (const std::string& name)
  {
    size_t index;
    try {
      index = std::stoi (name, nullptr) + 1;
      if (index < _children.size ()) {
        Process* c = _children.at (index);
        std::remove (_children.begin (), _children.end (), c);
        _removed->set_value (c, true);
      } else {
        cerr << "Index " << index << " out of bound for list " << _name << endl;
      }
    }
    catch (invalid_argument& arg) {
      cerr << "Invalid child name '" << name << "' for list " << _name << endl;
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
          if (sz != 0) {
            return c->find_component (path.substr (sz + 1));
          } else
            return c;
        } else {
          cerr << "Index " << index << " out of bound for list " << _name << endl;
        }
      }
      catch (invalid_argument& arg) {
        cerr << "Invalid child name '" << path << "' for list " << _name << endl;
      }
    }
    return nullptr;
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
    if (_parent->get_state () != activated)
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
    if (_parent->get_state () != activated)
      return;
    _index->set_value (1, true);
  }

  BidirectionalListIterator::BidirectionalListIterator (Process *parent, const string& name,
                                                        Process* list) :
      Process (parent, name)
  {
    _list = dynamic_cast<List*> (list);
    if (_list == nullptr) {
      cerr << "WARNING: list iterator must take a List as its third argument\n";
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
}

