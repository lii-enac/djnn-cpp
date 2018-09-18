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
#include "../core/tree/bool_property.h"
#include "../core/error.h"

namespace djnn
{
  using namespace std;

  void
  SwitchList::Next::activate ()
  {
    /* note:
     * next and previous only compute the correct index and then assign it
     * the action on changed index cause the branches switch
     */
    int icur = _sw->index ()->get_value ();
    int inext = icur + 1;
    if (_sw->loop ()->get_value () && (inext > _sw->children ().size ()) ) {
      inext = 1;
    }
    if (!_sw->loop ()->get_value () && (inext >= _sw->children ().size ()) ) {
      inext = _sw->children ().size ();
    }
    _sw->index ()->set_value (inext, true);
  }

  void
  SwitchList::Previous::activate ()
  {
    /* note: 
     * next and previous only compute the correct index and then assign it
     * the action on changed index cause the branches switch
     */
    int icur = _sw->index ()->get_value ();
    int iprev = icur - 1;
    if (_sw->loop ()->get_value () && (iprev <= 0) ) {
      iprev = _sw->children ().size ();
    }
    if (!_sw->loop ()->get_value () && (iprev < 1) ) {
      iprev = 1;
    }
    _sw->index ()->set_value (iprev, true);
  }

  void
  SwitchList::ChangeIndex::activate ()
  {
    /* note:
     * next and previous only compute the correct index and then assign it
     * the action on changed index cause the branches switch ONLY if the
     * index is different from the current one.
     */
    int i = _sw->index ()->get_value ();
    if ((i - 1) < _sw->children ().size () && (i - 1) >= 0) {
      Process* next = _sw->children ()[i - 1];
      if (_sw->item () && _sw->item () != next) {
        _sw->item ()->deactivation ();
        _sw->set_item (next);
        next->activation ();
      }
    }
  }

  void
  SwitchList::init (bool loop)
  {
    /* 
     * note:
     * do not add directly Property into the symbol switchlist's table 
     * switchlist sympbol table should only containt branch
     */
    _loop = make_unique<BoolProperty> (loop);
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

  SwitchList::SwitchList (bool loop) :
      AbstractList (), _cur_item (nullptr)
  {
    init (loop);
  }

  SwitchList::SwitchList (Process* parent, const string& name, bool loop) :
      AbstractList (parent, name), _cur_item (nullptr)
  {
    init (loop);
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
    else if (path.compare ("loop") == 0)
      return (Process *) _loop.get ();
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
    AbstractSerializer::serializer->text_attribute ("loop", _loop ? "true" : "false");

    for (auto c : _children)
      c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
  }

}
