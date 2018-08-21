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
#include "../core/error.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  FSMState::FSMState (Process *p, const string &n) :
      Container (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning ("only a FSM can be the parent of a FSM State\n");
      return;
    }
    fsm->set_initial (n);
    _parent_fsm = fsm;
    Process::finalize ();
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
        t2->fsm_action ()->set_activation_flag (NONE);
      }
    }
  }

  void
  FSMState::activate ()
  {
    for (auto t : _transitions) {
      t->activation ();
    }
    Container::activate ();
    _parent_fsm->update_state (this, _name);
  }

  void
  FSMState::deactivate ()
  {
    for (auto t : _transitions) {
      t->deactivation ();
    }
    Container::deactivate ();
  }

  void
  FSMState::draw ()
  {
    Container::draw ();
  }

  FSMTransition::FSMTransition (Process *p, const string &n, Process* from, Process* to,
                                Process *src, const string &spec, Process *action,
                                const string &aspec) :
      Process (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning ("only a FSM can be the parent of a FSM Transition\n");
      return;
    }
    _priority = fsm->priority ();
    fsm->increase_priority ();

    _src = dynamic_cast<FSMState*> (from);
    _dst = dynamic_cast<FSMState*> (to);
    if (_src == nullptr || _dst == nullptr) {
      warning ("only FSM states can be connected in a FSM transition\n");
      return;
    }

    _src->add_transition (this);
    _trigger = src->find_component (spec);
    if (_trigger == nullptr) {
      warning ("invalid source in transition " + n + "\n");
      _c_src = nullptr;
      return;
    }

    if (action != 0)
      _action = action->find_component (aspec);
    else
      _action = 0;

    _fsm_action = make_shared<FSMTransitionAction> (this,
                                                    "transition_action_" + _src->get_name () + "_" + _dst->get_name (),
                                                    _src, _dst, _action);
    _c_src = make_unique<Coupling> (_trigger, ACTIVATION, _fsm_action.get (), ACTIVATION);
    Graph::instance ().add_edge (_trigger, _fsm_action.get ());
    Graph::instance ().add_edge (_fsm_action.get (), p->find_component ("state"));
    Graph::instance ().add_edge (_fsm_action.get (), _dst);
    Graph::instance ().add_edge (_fsm_action.get (), _src);
    if (_action) {
      Graph::instance ().add_edge (_fsm_action.get (), _action);
    }
    _c_src.get ()->disable ();
    Process::finalize ();
  }

  FSMTransition::FSMTransition (Process *p, const string &n, Process* from, Process* to,
                                Process *trigger, Process *action) :
      Process (p, n)
  {
    FSM *fsm = dynamic_cast<FSM*> (p);
    if (fsm == nullptr) {
      warning ("only a FSM can be the parent of a FSM Transition\n");
      return;
    }
    _priority = fsm->priority ();
    fsm->increase_priority ();

    _src = dynamic_cast<FSMState*> (from);
    _dst = dynamic_cast<FSMState*> (to);
    if (_src == nullptr || _dst == nullptr) {
      warning ("only FSM states can be connected in a FSM transition\n");
      return;
    }

    _src->add_transition (this);
    _trigger = trigger;
    _action = action;
    if (_trigger == nullptr) {
      warning ("invalid source in transition " + n + "\n");
      _c_src = nullptr;
      return;
    }
  
    _fsm_action = make_shared<FSMTransitionAction> (this,
                                                    "transition_action_" + _src->get_name () + "_" + _dst->get_name (),
                                                    _src, _dst, _action);
    _c_src = make_unique<Coupling> (_trigger, ACTIVATION, _fsm_action.get (), ACTIVATION);
    Graph::instance ().add_edge (_trigger, _fsm_action.get ());
    Graph::instance ().add_edge (_fsm_action.get (), p->find_component ("state"));
    Graph::instance ().add_edge (_fsm_action.get (), _dst);
    Graph::instance ().add_edge (_fsm_action.get (), _src);
    if (_action) {
      Graph::instance ().add_edge (_fsm_action.get (), _action);
    }
    _c_src.get ()->disable ();
    Process::finalize ();
  }

  FSMTransition::~FSMTransition ()
  {
    Graph::instance ().remove_edge (_trigger, _fsm_action.get ());
    Graph::instance ().remove_edge (_fsm_action.get (), get_parent ()->find_component ("state"));
    Graph::instance ().remove_edge (_fsm_action.get (), _dst);
    Graph::instance ().remove_edge (_fsm_action.get (), _src);
    if (_action) {
      Graph::instance ().remove_edge (_fsm_action.get (), _action);
    }
  }

  void
  FSMTransition::activate ()
  {
    _c_src->enable ();
  }

  void
  FSMTransition::deactivate ()
  {
    _c_src->disable ();
  }

  void
  FSMTransition::FSMTransitionAction::activate ()
  {
    if (!_src->is_highest_priority (_t))
      return;
    _src->disable_transitions (_t);
    if (_action != 0)
      _action->activation ();
    notify_activation ();
    if (_src != _dst) {
      _src->deactivation ();
      _dst->activation ();
    }
  }

  void
  FSM::init_FSM ()
  {
    _fsm_state = make_shared<TextProperty> (this, "state", "");
    _initial = make_shared<TextProperty> (this, "initial", "");
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _fsm_state.get ());
    _state_dependency = _fsm_state.get ();
  }

  FSM::~FSM ()
  {
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _fsm_state.get ());
  }
  void
  FSM::activate ()
  {
    _fsm_state->activation ();
    _initial->activation ();
    _initial->set_value (_str_initial, true);

    if (_str_initial.length () != 0) {
      FSMState* init_state = dynamic_cast<FSMState*> (find_component (_str_initial));
      if (init_state)
        init_state->activation ();
    }
  }

  void
  FSM::deactivate ()
  {
    if (_cur_state != 0)
      _cur_state->deactivation ();
  }

  void
  FSM::draw ()
  {
    if (_activation_flag > activated)
      return;
    if (_cur_state != nullptr)
      _cur_state->draw ();
  }
}
