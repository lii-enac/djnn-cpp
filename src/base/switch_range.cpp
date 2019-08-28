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
#include "../core/core.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  SwitchRangeBranch::SwitchRangeBranch (Process *parent, const string &name, double lower, bool left_open, double upper,
                                        bool right_open) :
      Container (parent, name), _left_open (left_open), _right_open (right_open)
  {
    _lower = new DoubleProperty (lower);
    _upper = new DoubleProperty (upper);
    add_symbol ("lower", _lower);
    add_symbol ("upper", _upper);
    Process::finalize_construction ();
  }

  SwitchRangeBranch::~SwitchRangeBranch ()
  {
    delete _lower;
    delete _upper;
  }

  void
  SwitchRangeBranch::serialize (const string& type) {

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:switch-range-branch");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("lower", _lower->get_value ());
    AbstractSerializer::serializer->float_attribute ("upper", _upper->get_value ());
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
    bool l = _left_open ? v > _lower->get_value () : v >= _lower->get_value ();
    bool u = _right_open ? v < _upper->get_value () : v <= _upper->get_value ();
    return l && u;
  }

  SwitchRange::SwitchRange (Process *parent, const string &name, double initial) :
      Container (parent, name)
  {
    init_switch_range (initial);
    Process::finalize_construction ();
  }

  SwitchRange::SwitchRange (double initial)
  {
    init_switch_range (initial);
  }

  void
  SwitchRange::init_switch_range (double initial)
  {
    _initial = initial;
    /*  added to symTable but not in _children */
    _branch_range = new DoubleProperty (initial);
    add_symbol ("state", _branch_range);
    _action = new SwitchRangeAction (this, get_name ());
    _state_dependency = _action;
    _c_branch = new Coupling (_branch_range, ACTIVATION, _action, ACTIVATION, true);
    add_state_dependency (_parent, _action);

    _cur_branch = nullptr;
  }

  SwitchRange::~SwitchRange ()
  {
    /* note:
     * We have to delete all content BEFORE deleting _action and _branch_name
     * especially all the state_dependency
     */
    Container::clean_up_content ();

    remove_state_dependency (_parent, _action);
   
    delete _c_branch;
    /* note :
     * Here, we can delete _action because is has not been add into _children
     * avoid to add the action in Container::_children list
     * otherwise there is a side effect on ~switch which 
     * occured after ~container which already deleted _children */
    delete _action;
    delete _branch_range;
  }

  void
  SwitchRange::impl_activate ()
  {
    _c_branch->enable ();
    change_branch ();
    //set_activated ();
    set_activation_state (ACTIVATED);
  }

  void
  SwitchRange::impl_deactivate ()
  {
    _c_branch->disable ();
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
    double v = _branch_range->get_value ();
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
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("initial", _initial);

    for (auto c : _children)
      c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}
