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

#include "fsm.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"


namespace djnn
{
  using std::string;

  FSMState::FSMState (ParentProcess* parent, const std::string& name)
  : Container (parent, name)
  {
    FSM *fsm = djnn_dynamic_cast<FSM*> (parent);
    if (fsm == nullptr) {
      warning (this, "only a FSM can be the parent of a FSM State\n");
      return;
    }
    fsm->set_initial (name);
    _parent_fsm = fsm;
    finalize_construction (parent, name);
    _parent_fsm->FSM::add_state(this);
  }

  bool
  FSMState::is_highest_priority (FSMTransition *t)
  {
    int priority = t->priority ();
    for (auto t2 : _transitions) {
      if (t2 != t && t2->fsm_action ()->get_activation_flag () == ACTIVATION && t2->priority () < priority)
        return false;
    }
    return true;
  }

  void
  FSMState::disable_transitions (FSMTransition *t)
  {
    for (auto t2 : _transitions) {
      if (t2 != t && t2->fsm_action ()->get_activation_flag () == ACTIVATION) {
        t2->fsm_action ()->set_activation_flag (NONE_ACTIVATION);
      }
    }
  }

  void
  FSMState::impl_activate ()
  {
    for (auto t : _transitions) {
      t->activate ();
    }
    Container::impl_activate ();
    _parent_fsm->update_state (this, get_name ());
  }

  void
  FSMState::impl_deactivate ()
  {
    for (auto t : _transitions) {
      t->deactivate ();
    }
    Container::impl_deactivate ();
  }

#ifndef DJNN_NO_SERIALIZE
  void
  FSMState::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsmstate");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

  FSMTransition::Init::Init (FSMTransition* t, ParentProcess* parent, 
                            const std::string& tspec, const std::string& aspec) 
  {
    FSM *fsm = djnn_dynamic_cast<FSM*> (parent);
    if (fsm == nullptr) {
      warning (t, "only a FSM can be the parent of a FSM Transition\n");
      return;
    }
    t->_priority = fsm->priority ();
    fsm->increase_priority ();

    if (t->_from_state == nullptr || t->_to_state == nullptr) {
      warning (t, "only FSM states can be connected in a FSM transition\n");
      return;
    }

    t->_from_state->add_transition (t);
    if (t->_trigger == nullptr) {
      warning (t, "invalid TRIGGER in transition ( name: " + t->get_name () + ", trigger spec: " + tspec + ", action spec:" + aspec + ")\n");
    }
  }


  FSMTransition::FSMTransition (ParentProcess* parent, const std::string& name, CoreProcess* from, CoreProcess* to,
                                CoreProcess *trigger, const std::string& tspec, CoreProcess *action,
                                const std::string& aspec) 
  : FatProcess (name),
  _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
  _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
  _trigger(trigger ? trigger->find_child_impl (tspec) : nullptr),
  _init(this, parent, tspec, aspec),
  _fsm_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, action ? action->find_child_impl (aspec) : nullptr),
  _c_src (), _c_trigger_to_action (), _c_action_to_fsm_action (),
  _priority (0)
  {
    CoreProcess* _action = action ? action->find_child_impl (aspec) : nullptr;
    if (_action) {
      _c_trigger_to_action.init (_trigger, ACTIVATION, _action, ACTIVATION);
      _c_action_to_fsm_action.init (_action, ACTIVATION, &_fsm_action, ACTIVATION);
      _c_trigger_to_action.disable ();
      _c_action_to_fsm_action.disable ();
      graph_add_edge (_trigger, _action);
      graph_add_edge(_action, &_fsm_action);
    } else {
      _c_src.init (_trigger, ACTIVATION, &_fsm_action, ACTIVATION);
      _c_src.disable ();
      graph_add_edge(_trigger, &_fsm_action);
    }
    graph_add_edge (&_fsm_action, parent->find_child ("state"));
    finalize_construction (parent, name);
    FSM *fsm = djnn_dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::FSMTransition (ParentProcess* parent, const std::string& name, CoreProcess* from, CoreProcess* to,
                                CoreProcess *trigger, CoreProcess *action) 
  : FatProcess (name),
  _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
  _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
  _trigger (trigger),
  _init(this, parent, "NO spec for trigger", "NO spec for action"),
  _fsm_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, action),
  _c_src (), _c_trigger_to_action (), _c_action_to_fsm_action (),
  _priority (0)
  {
    if (action) {
      _c_trigger_to_action.init (_trigger, ACTIVATION, action, ACTIVATION);
      _c_action_to_fsm_action.init (action, ACTIVATION, &_fsm_action, ACTIVATION);
      _c_trigger_to_action.disable ();
      _c_action_to_fsm_action.disable ();
      graph_add_edge (_trigger, action);
      graph_add_edge(action, &_fsm_action);
    } else {
      _c_src.init(_trigger, ACTIVATION, &_fsm_action, ACTIVATION);
      _c_src.disable ();
      graph_add_edge(_trigger, &_fsm_action);
    }
    graph_add_edge (&_fsm_action, parent->find_child ("state"));
    finalize_construction (parent, name);
    FSM *fsm = djnn_dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::~FSMTransition ()
  {
    if (_c_src.is_effective()) {
      graph_remove_edge(_trigger, &_fsm_action);
    }
	if (_c_trigger_to_action.is_effective()) {
      graph_remove_edge (_trigger, _c_trigger_to_action.get_dst());
    }
    if (_c_action_to_fsm_action.is_effective()) {
      graph_remove_edge(_c_action_to_fsm_action.get_src (), &_fsm_action);
    }
    graph_remove_edge (&_fsm_action, get_parent ()->find_child ("state"));
  }

  void
  FSMTransition::impl_activate ()
  {
	if (_c_src.is_effective())
      _c_src.enable ();
    if (_c_trigger_to_action.is_effective())
      _c_trigger_to_action.enable ();
    if (_c_action_to_fsm_action.is_effective())
      _c_action_to_fsm_action.enable ();
  }

  void
  FSMTransition::impl_deactivate ()
  {
    if (_c_src.is_effective ())
      _c_src.disable ();
    if (_c_trigger_to_action.is_effective ())
      _c_trigger_to_action.disable ();
    if (_c_action_to_fsm_action.is_effective())
      _c_action_to_fsm_action.disable ();
  }

  void
  FSMTransition::FSMTransitionAction::impl_activate ()
  {
    if (!_src->is_highest_priority (_t))
      return;
    _src->disable_transitions (_t);
    notify_activation ();
    if (_src != _dst) {
      _src->deactivate ();
      _dst->activate ();
    }
  }

#ifndef DJNN_NO_SERIALIZE
  void
  FSMTransition::serialize (const std::string& type) {
   
    string buf;

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsmtransition");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    AbstractSerializer::compute_path (get_parent (), _from_state, buf);
    AbstractSerializer::serializer->text_attribute ("from", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _to_state, buf);
    AbstractSerializer::serializer->text_attribute ("to", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _trigger, buf);
    AbstractSerializer::serializer->text_attribute ("trigger", buf);
    buf.clear ();
    CoreProcess* _action = _c_trigger_to_action.is_effective() ? _c_trigger_to_action.get_dst() : nullptr;
    AbstractSerializer::compute_path (get_parent (), _action, buf);
    AbstractSerializer::serializer->text_attribute ("action", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

  FSM::FSM (ParentProcess* parent, const std::string& name) 
  : FatProcess (name), 
  _priority (0),
  _cur_state (nullptr),
  _fsm_state (this, "state", ""),
  _initial (this, "initial", "")
  { 
    /* with state_dependency */
    finalize_construction (parent, name, &_fsm_state);
  }

  FSM::~FSM ()
  {
    remove_state_dependency (get_parent (), state_dependency ());

    /* inverse delete of _transition */
    int sz = _transitions.size ();
    for (int i = sz - 1; i >= 0; i--) {
        delete _transitions[i];
    }

    /* inverse delete of _states */
    sz = _states.size ();
    for (int i = sz - 1; i >= 0; i--) {
        graph_remove_edge (_states[i], &_fsm_state);
        delete _states[i];
    }
  }

  void
  FSM::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), state_dependency ());
    }

    add_state_dependency (parent, state_dependency ());
    FatProcess::set_parent (parent); 
  }

  void
  FSM::impl_activate ()
  {
    _fsm_state.activate ();

    FSMState* init_state = dynamic_cast<FSMState*> (find_child_impl (_initial.get_value ()));
    if (init_state)
      init_state->activate ();
  }

  void
  FSM::impl_deactivate ()
  {
    if (_cur_state != 0)
      _cur_state->deactivate ();
  }

  void
  FSM::draw ()
  {
    if (somehow_deactivating ())
      return;

    if (_cur_state != nullptr)
      _cur_state->draw ();
  }

  void
  FSM::pick ()
  {
    if (somehow_deactivating ())
      return;

    if (_cur_state != nullptr)
      _cur_state->pick ();
  }

  AbstractGShape*
  FSM::pick_analytical (PickAnalyticalContext& pac)
  {
    if (somehow_deactivating ())
      return nullptr;

    if (_cur_state != nullptr)
      return _cur_state->pick_analytical (pac);
    return nullptr;
  }

#ifndef DJNN_NO_SERIALIZE
  void
  FSM::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsm");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto st : _states)
        st->serialize (type);
    for (auto tr : _transitions)
        tr->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}
