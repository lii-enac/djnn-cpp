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
#include "../core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  Switch::Switch (Process *parent, const string &name, const string &initial) :
      Container (parent, name)
  {
    init_switch (initial);
    Process::finalize ();
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
    //Graph::instance ().add_edge (_branch_name, _action);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);

    _cur_branch = nullptr;
  }

  Switch::~Switch ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    //Graph::instance ().remove_edge (_branch_name, _action);
   
    delete _c_branch;
    delete _branch_name;
    /* note :
     * Here, we can delete _action because is has not been add into _children
     * avoid to add the action in Container::_children list
     * otherwise there is a side effect on ~switch which 
     * occured after ~container which already deleted _children */
    delete _action;
  }

  void
  Switch::activate ()
  {
    _c_branch->enable ();
    change_branch ();
    set_activated ();
  }

  void
  Switch::deactivate ()
  {
    _c_branch->disable ();
    if (_cur_branch != nullptr)
      _cur_branch->deactivation ();
  }

  void
  Switch::draw ()
  {
    if (is_deactivation_requested ())
      return;
    if (_cur_branch != nullptr)
      _cur_branch->draw ();
  }

  void
  Switch::pick ()
  {
    if (is_deactivation_requested ())
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
        if (_cur_branch->is_deactivated ())
          _cur_branch->activation ();
        return;
      } else {
        if (_cur_branch != nullptr && _cur_branch->is_activated ())
          _cur_branch->deactivation ();
        _cur_branch = it->second;
        (it->second)->activation ();
      }
    }
    else {
      /* note: 
       * if the new _branch_name is not an existing branch of the switch
       * the state is set at _branch_name and the _cur_branch is deactivated
       */
      if (_cur_branch)
        _cur_branch->deactivation ();
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
