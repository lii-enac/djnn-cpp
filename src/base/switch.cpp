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
    _branch_name = new TextProperty (initial);
    add_symbol ("state", _branch_name);
    _action = new SwitchAction (this, get_name ());
    _state_dependency = _action;
    _c_branch = new Coupling (_branch_name, ACTIVATION, _action, ACTIVATION);
    Graph::instance ().add_edge (_branch_name, _action);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action);

    _cur_branch = nullptr;
  }

  Switch::~Switch ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action);
    Graph::instance ().remove_edge (_branch_name, _action);
   
    set_vertex (nullptr);
    if (_c_branch) { delete _c_branch; _c_branch = nullptr;}
    if (_action) { delete _action; _action = nullptr;}
    if (_branch_name) { delete _branch_name; _branch_name = nullptr;}
  }

  void
  Switch::activate ()
  {
    _c_branch->enable ();
    change_branch ();
    _activation_state = activated;
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
    if (_activation_flag > activated)
      return;
    if (_cur_branch != nullptr)
      _cur_branch->draw ();
  }

  void
  Switch::change_branch ()
  {
    string key = _branch_name->get_value ();
    map<string, Process*>::iterator it = _symtable.find (key);
    if (it != _symtable.end ()) {
      if (_cur_branch == it->second) {
        if (_cur_branch->get_state () == deactivated)
          _cur_branch->activation ();
        return;
      } else {
        if (_cur_branch != nullptr && _cur_branch->get_state () == activated)
          _cur_branch->deactivation ();
        _cur_branch = it->second;
        (it->second)->activation ();
      }
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
