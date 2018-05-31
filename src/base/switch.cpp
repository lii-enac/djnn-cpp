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

#include "switch.h"

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
    _branch_name = shared_ptr<TextProperty> (new TextProperty (initial));
    add_symbol ("state", _branch_name.get ());
    _action = make_shared<SwitchAction> (this, get_name ());
    Graph::instance ().add_edge (_branch_name.get (), _action.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _action.get ());
    _state_dependency = _branch_name.get ();
    _c_branch = make_unique<Coupling> (_branch_name.get (), ACTIVATION, _action.get (), ACTIVATION);
    _cur_branch = nullptr;
  }

  Switch::~Switch ()
  {
    Graph::instance ().remove_edge (_branch_name.get (), _action.get ());
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _action.get ());
  }
  void
  Switch::activate ()
  {
    _activation_state = activating;
    _c_branch.get ()->enable ();
    change_branch ();
    _activation_state = activated;
  }

  void
  Switch::deactivate ()
  {
    _activation_state = deactivating;
    _branch_name->set_value (_initial, true);
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
    string key = _branch_name.get ()->get_value ();
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

}
