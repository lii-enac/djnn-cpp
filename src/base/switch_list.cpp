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
#include "../core/ontology/coupling.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"
#include "../core/tree/bool_property.h"
#include "../core/utils/error.h"

namespace djnn
{
  using namespace std;

  SwitchList::Next::Next  (SwitchList *parent) : 
    Action (parent, "_next_action"), _sw (parent)
  {
  }

  void
  SwitchList::Next::impl_activate ()
  {
    /* note:
     * next and previous only compute the correct index and then assign it
     * the action on changed index cause the branches switch
     */
    unsigned int icur = _sw->index ()->get_value ();
    unsigned int inext = icur + 1;
    if (_sw->loop ()->get_value () && (inext > _sw->children ().size ()) ) {
      inext = 1;
    }
    if (!_sw->loop ()->get_value () && (inext >= _sw->children ().size ()) ) {
      inext = _sw->children ().size ();
    }
    _sw->index ()->set_value ((int)inext, true);
  }

  SwitchList::Previous::Previous  (SwitchList *parent) : 
    Action (parent, "_previous_action"), _sw (parent)
  {
  }

  void
  SwitchList::Previous::impl_activate ()
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

  SwitchList::ChangeIndex::ChangeIndex  (SwitchList *parent) : 
    Action (parent, "_change_index_action"), _sw (parent)
  {
  }

  void
  SwitchList::ChangeIndex::impl_activate ()
  {
    /* note:
     * next and previous only compute the correct index and then assign it
     * the action on changed index cause the branches switch ONLY if the
     * index is different from the current one.
     */
    unsigned int i = _sw->index ()->get_value ();
    if ((i - 1) < _sw->children ().size () && (i - 1) >= 0) {
      Process* next = _sw->children ()[i - 1];
      if (_sw->item () && _sw->item () != next) {
        _sw->item ()->deactivate ();
        _sw->set_item (next);
        next->activate ();
      }
    }
  }

  void
  SwitchList::init (bool loop)
  {
    /* 
     * note:
     * do not add directly Property, spikes and actions into the symbol switchlist's table 
     * switchlist sympbol table should only containt branch
     */
    _loop = new BoolProperty (nullptr, "_loop", loop);
    _index = new IntProperty (nullptr, "_index", 1);
    _next = new Spike (nullptr, "_next");
    _previous = new Spike (nullptr, "_previous");
    _next_action = new Next (this);
    _previous_action = new Previous (this);
    _change_index_action = new ChangeIndex (this);

    _c_next = new Coupling (_next, ACTIVATION, _next_action, ACTIVATION, true);
    _c_previous = new Coupling (_previous, ACTIVATION, _previous_action, ACTIVATION, true);
    _c_index = new Coupling (_index, ACTIVATION, _change_index_action, ACTIVATION, true);
    _c_next->disable ();
    _c_previous->disable ();
    _c_index->disable ();
  }

  SwitchList::SwitchList (bool loop) :
      AbstractList (), _cur_item (nullptr)
  {
    init (loop);
    _state_dependency = _change_index_action;
  }

  SwitchList::SwitchList (Process* parent, const string& name, bool loop) :
      AbstractList (parent, name), _cur_item (nullptr)
  {
    init (loop);
    Process::finalize_construction (parent, _change_index_action);
  }

  void
  SwitchList::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent){
       remove_state_dependency (_parent, _state_dependency);
    }

    add_state_dependency (p, _state_dependency);
    _parent = p; 
  }

  SwitchList::~SwitchList ()
  {
    Container::clean_up_content ();

    remove_state_dependency (_parent, _state_dependency);

    delete _c_index;
    delete _c_previous;
    delete _c_next;
    delete _change_index_action;
    delete _previous_action;
    delete _next_action;
    delete _previous;
    delete _next;
    delete _index;
    delete _loop;
  }

  void
  SwitchList::impl_activate ()
  {
    _c_next->enable ();
    _c_previous->enable ();
    _c_index->enable ();
    if (_children.empty ())
      return;
    if (_cur_item == nullptr)
      _cur_item = _children[0];
    _cur_item->activate ();
  }

  void
  SwitchList::impl_deactivate ()
  {
    _c_next->disable ();
    _c_previous->disable ();
    _c_index->disable ();
    if (_cur_item)
      _cur_item->deactivate ();
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
      return _next;
    else if (path.compare ("previous") == 0)
      return _previous;
    else if (path.compare ("index") == 0)
      return _index;
    else if (path.compare ("loop") == 0)
      return (Process *) _loop;
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
