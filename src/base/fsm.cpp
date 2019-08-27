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

#include "../core/core.h"
#include "../core/execution/graph.h"
#include "../core/serializer/serializer.h"
#include "../core/utils/error.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  FSMState::FSMState (Process *p, const string &n) :
      Container (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning (this, "only a FSM can be the parent of a FSM State\n");
      return;
    }
    fsm->set_initial (n);
    _parent_fsm = fsm;
    Process::finalize_construction (p);
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
    _parent_fsm->update_state (this, _name);
  }

  void
  FSMState::impl_deactivate ()
  {
    for (auto t : _transitions) {
      t->deactivate ();
    }
    Container::impl_deactivate ();
  }

  void
  FSMState::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsmstate");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  void
  FSMTransition::init_FSMTransition () 
  {
    _fsm_action = new FSMTransitionAction (this, "transition_action_" + _src->get_name () + "_" + _dst->get_name (), _src, _dst, _action);
    _c_src = new Coupling (_trigger, ACTIVATION, _fsm_action, ACTIVATION, true);
    _c_src->disable ();
    /*
    Graph::instance ().add_edge (_trigger, _fsm_action);
    Graph::instance ().add_edge (_fsm_action, _parent->find_component ("state"));
    Graph::instance ().add_edge (_fsm_action, _dst);
    Graph::instance ().add_edge (_fsm_action, _src);
    if (_action)
      Graph::instance ().add_edge (_fsm_action, _action);
    */
  }

  FSMTransition::FSMTransition (Process *p, const string &n, Process* from, Process* to,
                                Process *src, const string &spec, Process *action,
                                const string &aspec) :
      Process (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning (this, "only a FSM can be the parent of a FSM Transition\n");
      return;
    }
    _priority = fsm->priority ();
    fsm->increase_priority ();

    _src = dynamic_cast<FSMState*> (from);
    _dst = dynamic_cast<FSMState*> (to);
    if (_src == nullptr || _dst == nullptr) {
      warning (this, "only FSM states can be connected in a FSM transition\n");
      return;
    }

    _src->add_transition (this);
    _trigger = src->find_component (spec);
    if (_trigger == nullptr) {
      warning (this, "invalid SOURCE in transition ( name: " + n + ", src spec: " + spec + ", action spec:" + aspec + ")\n");
      _c_src = nullptr;
      return;
    }

    if (action != 0)
      _action = action->find_component (aspec);
    else
      _action = 0;

    init_FSMTransition ();
    
    Process::finalize_construction (p);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::FSMTransition (Process *p, const string &n, Process* from, Process* to,
                                Process *trigger, Process *action) :
      Process (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning (this, "only a FSM can be the parent of a FSM Transition\n");
      return;
    }
    _priority = fsm->priority ();
    fsm->increase_priority ();

    _src = dynamic_cast<FSMState*> (from);
    _dst = dynamic_cast<FSMState*> (to);
    if (_src == nullptr || _dst == nullptr) {
      warning (this, "only FSM states can be connected in a FSM transition\n");
      return;
    }

    _src->add_transition (this);
    _trigger = trigger;
    _action = action;
    if (_trigger == nullptr) {
      warning (this, "invalid source in transition " + n + "\n");
      _c_src = nullptr;
      return;
    }
  
    init_FSMTransition ();
    
    Process::finalize_construction (p);
    fsm->FSM::add_transition(this);
  }

  FSMTransition::~FSMTransition ()
  {
    /*
    if (_action)
      Graph::instance ().remove_edge (_fsm_action, _action);
    
    Graph::instance ().remove_edge (_trigger, _fsm_action);
    Graph::instance ().remove_edge (_fsm_action, get_parent ()->find_component ("state"));
    Graph::instance ().remove_edge (_fsm_action, _dst);
    Graph::instance ().remove_edge (_fsm_action, _src);
    */
    delete _fsm_action;
    delete _c_src;
    
  }

  void
  FSMTransition::impl_activate ()
  {
    _c_src->enable ();
  }

  void
  FSMTransition::impl_deactivate ()
  {
    _c_src->disable ();
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

  void
  FSMTransition::serialize (const string& type) {
   
    string buf;

    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsmtransition");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    AbstractSerializer::compute_path (get_parent (), _src, buf);
    AbstractSerializer::serializer->text_attribute ("from", buf);
    buf.clear ();
    AbstractSerializer::compute_path (get_parent (), _dst, buf);
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

  void
  FSM::init_FSM ()
  {
    _fsm_state = new TextProperty (this, "state", "");
    _initial = new TextProperty (this, "initial", "");
    
  }

  FSM::FSM () : Process ()
  {
    init_FSM ();
    _state_dependency = _fsm_state;  
  }

  FSM::FSM (Process *p, const string &n) : 
    Process (p, n), _cur_state (nullptr), _priority (0) 
  { 
    init_FSM ();

    /* with state_dependency */
    Process::finalize_construction (p, _fsm_state);
  }

  FSM::~FSM ()
  {
    remove_state_dependency (_parent, _state_dependency);

    for (FSMTransition* t : _transitions) {
        delete t;
    }
    for (FSMState* s : _states) {
        delete s;
    }

    delete _initial;
    delete _fsm_state;
  }

  void
  FSM::set_parent (Process* p)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (_parent){
       remove_state_dependency (_parent, _state_dependency);
    }

    add_state_dependency (p, _state_dependency);
    _parent = p; 
  }

  void
  FSM::impl_activate ()
  {
    _fsm_state->activate ();
    _initial->activate ();
    _initial->set_value (_str_initial, true);

    if (_str_initial.length () != 0) {
      FSMState* init_state = dynamic_cast<FSMState*> (find_component (_str_initial));
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

  void
  FSM::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:fsm");
    AbstractSerializer::serializer->text_attribute ("id", _name);

    for (auto st : _states)
        st->serialize (type);
    for (auto tr : _transitions)
        tr->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}
