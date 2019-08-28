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

#include "switch.h"
#include "../core/core.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  Switch::SwitchAction::SwitchAction (Switch *parent, const string &name) :
        Action (parent, name),  _sw (parent) 
  {
      /* note:
      * avoid to add the action in Container::_children list
      * otherwise there is a side effect on ~switch which 
      * occured after ~container which already deleted _children
      */ 
       
    if (parent) {
        _state_dependency = parent->state_dependency ();
        Process::set_parent (parent);
      }
  }

  Switch::Switch (Process *parent, const string &name, const string &initial) :
      Container (parent, name)
  {
    init_switch (initial);
    Process::finalize_construction (parent, _action);
  }

  Switch::Switch (const string &initial)
  {
    init_switch (initial);
    _state_dependency = _action;
  }

  void
  Switch::init_switch (const string &initial)
  {
    _initial = initial;
    /*  be carefull :
        we have to add branch_name in the process symTable but NOT in the container _children list 
        so _branch_name has to be create with nullptr as _parent with a "fake" name (but really usefull in debug mode)
        then added to symtable with his real name
    */
    _branch_name = new TextProperty (nullptr, "switch_state", initial);
    add_symbol ("state", _branch_name);
    _action = new SwitchAction (this, "switch_action");
    _c_branch = new Coupling (_branch_name, ACTIVATION, _action, ACTIVATION, true);
    _c_branch->disable ();

    _cur_branch = nullptr;
  }

  Switch::~Switch ()
  {
    remove_state_dependency (_parent, _state_dependency);

    /* note:
     * We have to delete all content BEFORE deleting _action and _branch_name
     * especially all the state_dependency
     */
    Container::clean_up_content ();
   
    delete _c_branch;
    /* note :
     * Here, we can delete _action because is has not been add into _children
     * avoid to add the action in Container::_children list
     * otherwise there is a side effect on ~switch which 
     * occured after ~container which already deleted _children */
    delete _action;
    delete _branch_name;
  }

  void
  Switch::impl_activate ()
  {
    _c_branch->enable ();
    change_branch ();
    set_activation_state (ACTIVATED);
  }

  void
  Switch::impl_deactivate ()
  {
    _c_branch->disable ();
    if (_cur_branch != nullptr)
      _cur_branch->deactivate ();
  }

  void
  Switch::draw ()
  {
    if (get_activation_flag () == DEACTIVATION)
      return;
    if (_cur_branch != nullptr)
      _cur_branch->draw ();
  }

  void
  Switch::pick ()
  {
    if (get_activation_flag() == DEACTIVATION)
      return;
    if (_cur_branch != nullptr)
      _cur_branch->pick ();
  }

  void
  Switch::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent){
       remove_state_dependency (_parent, _state_dependency);
    }

    add_state_dependency (p, _state_dependency);
    _parent = p; 
  }

  void
  Switch::change_branch ()
  {
    string key = _branch_name->get_value ();
    map<string, Process*>::iterator it = _symtable.find (key);
    if (it != _symtable.end ()) {
      if (_cur_branch == it->second) {
        if (_cur_branch->get_activation_state () == DEACTIVATED)
          _cur_branch->activate ();
        return;
      } else {
        if (_cur_branch != nullptr && _cur_branch->get_activation_state () == ACTIVATED)
          _cur_branch->deactivate ();
        _cur_branch = it->second;
        (it->second)->activate ();
      }
    }
    else {
      /* note: 
       * if the new _branch_name is not an existing branch of the switch
       * the state is set at _branch_name and the _cur_branch is deactivated
       */
      if (_cur_branch)
        _cur_branch->deactivate ();
    }
  }

  void
  Switch::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:switch");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("initial", _initial);

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}
