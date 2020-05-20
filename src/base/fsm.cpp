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

#include "core/execution/graph.h"
#include "core/serializer/serializer.h"
#include "core/utils/error.h"


namespace djnn
{
  using std::string;

  FSMState::FSMState (Process *parent, const std::string& name) :
      Container (parent, name)
  {
    FSM *fsm = dynamic_cast<FSM*> (parent);
    if (fsm == nullptr) {
      warning (this, "only a FSM can be the parent of a FSM State\n");
      return;
    }
    fsm->set_initial (name);
    _parent_fsm = fsm;
    Process::finalize_construction (parent, name);
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

  FSMTransition::Init::Init (FSMTransition* t, Process* p, 
                            const std::string& tspec, const std::string& aspec) 
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
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


  FSMTransition::FSMTransition (Process *parent, const std::string& name, Process* from, Process* to,
                                Process *trigger, const std::string& tspec, Process *action,
                                const std::string& aspec) 
  : Process (name),
  _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
  _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
  _trigger( trigger ? dynamic_cast<Process*>(trigger->find_child (tspec)) : nullptr),
  _action( action ? dynamic_cast<Process*>(action->find_child (aspec)) : nullptr),
  _init(this, parent, tspec, aspec),
  _fsm_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, _action),
  _c_src (_trigger, ACTIVATION, &_fsm_action, ACTIVATION, true)
  {
    
    _c_src.disable ();
    
    Graph::instance().add_edge(_trigger, _to_state);
    finalize_construction (parent, name);
    FSM *fsm = dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::FSMTransition (Process *parent, const std::string& name, Process* from, Process* to,
                                Process *trigger, Process *action) 
  : Process (name),
  _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
  _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
  _trigger (trigger),
  _action (action),
  _init(this, parent, "NO spec for trigger", "NO spec for action"),
  _fsm_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, _action),
  _c_src (_trigger, ACTIVATION, &_fsm_action, ACTIVATION, true)
  {
     _c_src.disable ();
    
    Graph::instance().add_edge(_trigger, _to_state);
    finalize_construction (parent, name);
    FSM *fsm = dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::~FSMTransition ()
  {
    Graph::instance().remove_edge (_trigger, _to_state);
  }

  void
  FSMTransition::impl_activate ()
  {
    _c_src.enable ();
  }

  void
  FSMTransition::impl_deactivate ()
  {
    _c_src.disable ();
  }

  void
  FSMTransition::FSMTransitionAction::impl_activate ()
  {
    if (!_src->is_highest_priority (_t))
      return;
    _src->disable_transitions (_t);
    if (_action != 0)
      _action->activate ();
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
    AbstractSerializer::compute_path (get_parent (), _action, buf);
    AbstractSerializer::serializer->text_attribute ("action", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

  FSM::FSM (Process *parent, const std::string& name) 
  : Process (name), 
  _priority (0),
  _cur_state (nullptr),
  _fsm_state (this, "state", ""),
  _initial (this, "initial", "")
  { 
    /* with state_dependency */
    Process::finalize_construction (parent, name, &_fsm_state);
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
        Graph::instance().remove_edge (_states[i], &_fsm_state);
        delete _states[i];
    }
  }

  void
  FSM::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), state_dependency ());
    }

    add_state_dependency (p, state_dependency ());
    Process::set_parent (p); 
  }

  void
  FSM::impl_activate ()
  {
    _fsm_state.activate ();
    _initial.activate ();
    _initial.set_value (_str_initial, true);

    if (_str_initial.length () != 0) {
      FSMState* init_state = dynamic_cast<FSMState*> (find_child (_str_initial));
      if (init_state)
        init_state->activate ();
    }
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
    if (get_activation_flag() == DEACTIVATION)
      return;
    if (_cur_state != nullptr)
      _cur_state->draw ();
  }

  void
  FSM::pick ()
  {
    if (get_activation_flag() == DEACTIVATION)
      return;
    if (_cur_state != nullptr)
      _cur_state->pick ();
  }

  AbstractGShape*
  FSM::pick_analytical (PickAnalyticalContext& pac)
  {
    if (get_activation_flag() == DEACTIVATION)
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
