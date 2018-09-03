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

#include <algorithm>

#include "switch_list.h"
#include "../core/control/coupling.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"
#include "../core/error.h"

namespace djnn
{
  using namespace std;

  void
  SwitchList::Next::activate ()
  {
    int i = _sw->index ()->get_value ();
    if (i < _sw->children ().size ()) {
      if (_sw->item ())
        _sw->item ()->deactivation ();
      Process *next = _sw->children ()[i];
      i++;
      _sw->set_item (next);
      next->activation ();
      _sw->index ()->set_value (i, true);
    }
  }

  void
  SwitchList::Previous::activate ()
  {
    int i = _sw->index ()->get_value ();
    if (i - 1 >= 1 && !_sw->children ().empty ()) {
      if (_sw->item ())
        _sw->item ()->deactivation ();
      Process *previous = _sw->children ()[i - 2];
      i--;
      _sw->set_item (previous);
      previous->activation ();
      _sw->index ()->set_value (i, true);
    }
  }

  void
  SwitchList::ChangeIndex::activate ()
  {
    int i = _sw->index ()->get_value ();
    if ((i - 1) < _sw->children ().size () && (i - 1) >= 0) {
      if (_sw->item ())
        _sw->item ()->deactivation ();
      Process *next = _sw->children ()[i - 1];
      _sw->set_item (next);
      next->activation ();
    }
  }

  void
  SwitchList::init ()
  {
    _index = make_shared<IntProperty> (1);
    _next = make_shared<Spike> ();
    _previous = make_shared<Spike> ();
    _next_action = make_shared<Next> (this);
    _previous_action = make_shared<Previous> (this);
    _change_index_action = make_shared<ChangeIndex> (this);

    _c_next = make_unique < Coupling > (_next.get (), ACTIVATION, _next_action.get (), ACTIVATION);
    _c_previous = make_unique < Coupling > (_previous.get (), ACTIVATION, _previous_action.get (), ACTIVATION);
    _c_index = make_unique < Coupling > (_index.get (), ACTIVATION, _change_index_action.get (), ACTIVATION);
    Graph::instance ().add_edge (_next.get (), _next_action.get ());
    Graph::instance ().add_edge (_previous.get (), _previous_action.get ());
    Graph::instance ().add_edge (_index.get (), _change_index_action.get ());
  }

  SwitchList::SwitchList () :
      AbstractList (), _cur_item (nullptr)
  {
    init ();
  }

  SwitchList::SwitchList (Process* parent, const string& name) :
      AbstractList (parent, name), _cur_item (nullptr)
  {
    init ();
    Process::finalize ();
  }

  SwitchList::~SwitchList ()
  {
    Graph::instance ().remove_edge (_next.get (), _next_action.get ());
    Graph::instance ().remove_edge (_previous.get (), _previous_action.get ());
    Graph::instance ().remove_edge (_index.get (), _change_index_action.get ());
  }

  void
  SwitchList::activate ()
  {
    _c_next->enable ();
    _c_previous->enable ();
    _c_index->enable ();
    if (_children.empty ())
      return;
    if (_cur_item == nullptr)
      _cur_item = _children[0];
    _cur_item->activation ();
  }

  void
  SwitchList::deactivate ()
  {
    _c_next->disable ();
    _c_previous->disable ();
    _c_index->disable ();
    if (_cur_item)
      _cur_item->deactivation ();
  }

  void
  SwitchList::finalize_child_insertion (Process *c)
  {
    c->set_parent (this);
    _added->set_value (c, true);
    _size->set_value (_size->get_value () + 1, true);
  }

  Process*
  SwitchList::find_component (const string& path)
  {
    if (path.compare ("next") == 0)
      return _next.get ();
    else if (path.compare ("previous") == 0)
      return _previous.get ();
    else if (path.compare ("index") == 0)
      return _index.get ();
    else
      return AbstractList::find_component (path);
  }

  Process*
  SwitchList::clone ()
  {
    SwitchList* clone = new SwitchList ();
    for (auto c : _children) {
      clone->add_child (c->clone (), c->get_name ());
    }
    return clone;
  }

  void
  SwitchList::serialize (const string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:switch-list");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    for (auto c : _children)
      c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

}
