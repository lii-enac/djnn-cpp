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

  Switch::Switch (Process *parent, const string &name, const string &initial) :
      Container (parent, name)
  {
    init_switch (initial);
    Process::finalize_construction ();
  }

  Switch::Switch (const string &initial)
  {
    init_switch (initial);
  }

  void
  Switch::init_switch (const string &initial)
  {
    _initial = initial;
    /*  added it symTable but not in _children */
    _branch_name = new TextProperty (initial);
    add_symbol ("state", _branch_name);
    _action = new SwitchAction (this, get_name ());
    _state_dependency = _action;
    _c_branch = new Coupling (_branch_name, ACTIVATION, _action, ACTIVATION, true);

    // Note: this is made by Container::addchild () --> set_parent () --> add_state_dependency ()
    //add_state_dependency (_parent, _action);

    //DEBUG
    // std::cerr << "init_switch - ADD_STATE (if exist) : " <<
    //   ( _parent ? _parent->get_name () + "/" : "/" ) << get_name () <<  " _parent->state_dep: " <<  ( _parent ? _parent->state_dependency () : nullptr) << " ----- " <<
    //   ( _parent ? _parent->get_name () + "/" : "PARENT_NULL" ) << " - " << _action->get_name () << endl;

    _cur_branch = nullptr;
  }

  Switch::~Switch ()
  {
     //DEBUG
     // std::cerr << "~switch- REMOVE_STATE (if exist)  : " <<
     //  ( _parent ? _parent->get_name () + "/" : "/" ) << get_name () <<  " _parent->state_dep: " <<  ( _parent ? _parent->state_dependency () : nullptr) << " ----- " <<
     //  ( _parent ? _parent->get_name () + "/" : "PARENT_NULL" ) << " - " << _action->get_name () << endl;

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
    //set_activated ();
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
