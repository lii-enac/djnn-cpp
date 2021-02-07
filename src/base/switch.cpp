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
#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  Switch::SwitchAction::SwitchAction (Switch * parent, const std::string& name) :
        Action (parent, name),  _sw (parent) 
  {
  }

   /*  be carefull :
        we have to add branch_name in the process symTable but NOT in the container _children list 
        so _branch_name has to be create with nullptr as _parent with a "fake" name (but really usefull in debug mode)
        then added to symtable with his real name
    */
  Switch::Switch (ParentProcess* parent, const std::string& name, const std::string& initial) 
  : Container (parent, name),
  _initial (initial),
  _branch_name (nullptr, "switch_state", initial),
  _action (this, "switch_action"),
  _c_branch (&_branch_name, ACTIVATION, &_action, ACTIVATION, true),
  _cur_branch (nullptr)
  {
    add_symbol ("state", &_branch_name);
    _c_branch.disable ();
    finalize_construction (parent, name, &_action);
  }
  
  Switch::~Switch ()
  {
    remove_state_dependency (get_parent (), state_dependency ());

    /* note:
     * We have to delete all content BEFORE deleting _action and _branch_name call by this destructor
     * especially all the state_dependency
     */
    Container::clean_up_content ();
   
  }

  void
  Switch::impl_activate ()
  {
    _c_branch.enable ();
    change_branch ();
    set_activation_state (ACTIVATED);
  }

  void
  Switch::impl_deactivate ()
  {
    _c_branch.disable ();
    if (_cur_branch != nullptr)
      _cur_branch->deactivate ();
  }

  void
  Switch::draw ()
  {
    if (somehow_deactivating ())
      return;

    if (_cur_branch != nullptr)
      _cur_branch->draw ();
  }

  void
  Switch::pick ()
  {
    if (somehow_deactivating ())
      return;

    if (_cur_branch != nullptr)
      _cur_branch->pick ();
  }

  AbstractGShape*
  Switch::pick_analytical (PickAnalyticalContext& pac)
  {
    if (somehow_deactivating ())
      return nullptr;

    if (_cur_branch != nullptr)
      return _cur_branch->pick_analytical (pac);
    return nullptr;
  }

  void
  Switch::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), state_dependency ());
    }

    add_state_dependency (parent, state_dependency ());
    FatProcess::set_parent (parent); 
  }

  void
  Switch::change_branch ()
  {
    string key = _branch_name.get_value ();
    symtable_t::iterator it = find_child_iterator (key);
    if (it != children_end ()) {
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

#ifndef DJNN_NO_SERIALIZE
  void
  Switch::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:switch");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("initial", _initial);

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

}
