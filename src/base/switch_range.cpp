/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "switch_range.h"
#include "core/core.h"
#include "core/execution/graph.h"
#include "core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  SwitchRangeBranch::SwitchRangeBranch (Process *parent, const string &name, double lower, bool left_open, double upper,
                                        bool right_open) 
  : Container (parent, name),
  _left_open (left_open),
  _right_open (right_open),
  _lower (nullptr, "_lower", lower),
  _upper (nullptr, "_upper", upper)
  {
    add_symbol ("lower", &_lower);
    add_symbol ("upper", &_upper);
    Process::finalize_construction (parent, name);
  }

  SwitchRangeBranch::~SwitchRangeBranch ()
  {
  }

  void
  SwitchRangeBranch::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:switch-range-branch");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("lower", _lower.get_value ());
    AbstractSerializer::serializer->float_attribute ("upper", _upper.get_value ());
    AbstractSerializer::serializer->int_attribute ("left-open", _left_open);
    AbstractSerializer::serializer->int_attribute ("right-open", _right_open);
    for (auto c : _children)
      c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  bool
  SwitchRangeBranch::is_in_range (double v)
  {
    bool l = _left_open ? v > _lower.get_value () : v >= _lower.get_value ();
    bool u = _right_open ? v < _upper.get_value () : v <= _upper.get_value ();
    return l && u;
  }


  SwitchRange::SwitchRangeAction::SwitchRangeAction (SwitchRange *parent, const string &name) :
      Action (parent, name),  _sw (parent) 
  {
  }

  /* note:
   * added _branch_range to symTable but not in _children 
   */
  SwitchRange::SwitchRange (Process *parent, const string &name, double initial) 
  : Container (parent, name),
  _initial (initial),
  _branch_range (nullptr, "switch_state", initial),
  _action (this, "switch_range_action"),
  _c_branch (&_branch_range, ACTIVATION, &_action, ACTIVATION, true),
  _cur_branch (nullptr)
  {
    add_symbol ("state", &_branch_range);
    _c_branch.disable ();
    Process::finalize_construction (parent, name, &_action);
  }

  /* note:
   * added _branch_range to symTable but not in _children 
   */
  SwitchRange::SwitchRange (double initial)
  : _initial (initial),
  _branch_range (nullptr, "switch_state", initial),
  _action (this, "switch_range_action"),
  _c_branch (&_branch_range, ACTIVATION, &_action, ACTIVATION, true),
  _cur_branch (nullptr)
  {
    add_symbol ("state", &_branch_range);
    _c_branch.disable ();
    set_state_dependency (&_action);
  }

  SwitchRange::~SwitchRange ()
  {
    remove_state_dependency (get_parent (), state_dependency ());

    /* note:
     * We have to delete all content BEFORE deleting _action and _branch_name
     * especially all the state_dependency
     */
    Container::clean_up_content ();

    /* note :
     * Here, we can delete _action because is has not been add into _children
     * avoid to add the action in Container::_children list
     * otherwise there is a side effect on ~switch which 
     * occured after ~container which already deleted _children */
  }

  void
  SwitchRange::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), state_dependency ());
    }

    add_state_dependency (p, state_dependency ());
    Process::set_parent (p); 
  }

  void
  SwitchRange::impl_activate ()
  {
    _c_branch.enable ();
    change_branch ();
    //set_activated ();
    set_activation_state (ACTIVATED);
  }

  void
  SwitchRange::impl_deactivate ()
  {
    _c_branch.disable ();
    if (_cur_branch != nullptr)
      _cur_branch->deactivate ();
  }

  void
  SwitchRange::draw ()
  {
    if (get_activation_flag () == DEACTIVATION)
      return;
    if (_cur_branch != nullptr)
      _cur_branch->draw ();
  }

  void
  SwitchRange::pick ()
  {
    if (get_activation_flag() == DEACTIVATION)
      return;
    if (_cur_branch != nullptr)
      _cur_branch->pick ();
  }

  void
  SwitchRange::change_branch ()
  {
    double v = _branch_range.get_value ();
    for (auto c: _children) {
      if (((SwitchRangeBranch*)c)->is_in_range (v)) {
        if (_cur_branch == c) {
          if (_cur_branch->get_activation_state () == DEACTIVATED)
             _cur_branch->activate ();
        } else {
          if (_cur_branch != nullptr && _cur_branch->get_activation_state () == ACTIVATED)
            _cur_branch->deactivate ();
          _cur_branch = c;
          c->activate ();
        }
        return;
      }
    }
    /* note:
     * if the value is out of any ranges of the switch range
     * the _cur_branch is deactivated
     */
     if (_cur_branch)
       _cur_branch->deactivate ();
  }

  void
  SwitchRange::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:switch-range");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("initial", _initial);

    for (auto c : _children)
      c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}
