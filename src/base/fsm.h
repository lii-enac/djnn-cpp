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

#pragma once

#include "../core/tree/component.h"
#include "../core/ontology/coupling.h"
#include "../core/tree/text_property.h"

#include <list>

namespace djnn {
  using namespace std;
  class FSM;
  class FSMState;
  class FSMTransition;

  class FSMState : public Container
  {
  public:
    FSMState (Process *p, const string &n);
    ~FSMState () { _transitions.clear (); };
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    bool is_highest_priority (FSMTransition *t);
    void disable_transitions (FSMTransition *t);
    void add_transition (FSMTransition *t) { FSMTransition* pt = t; _transitions.push_front (pt); };
  private:
    list<FSMTransition*> _transitions;
    FSM* _parent_fsm;
  };

  class FSMTransition : public Process
  {
  private:
    class FSMTransitionAction : public Action
    {
    public:
      FSMTransitionAction (Process *p, const string &n, FSMState* src, FSMState* dst, Process* action) :
	Action (p, n), _src (src), _dst (dst), _action (action) { _t = dynamic_cast<FSMTransition*> (p); }
      virtual ~FSMTransitionAction () {};
      void impl_activate ();
      void impl_deactivate () {};
    private:
      FSMTransition *_t;
      FSMState* _src;
      FSMState* _dst;
      Process* _action;
    };
  public:
    FSMTransition (Process *p, const string &n, Process* from, Process* to,
		   Process *src, const string &spec, Process *action = 0, const string &aspec = "");
    FSMTransition (Process *p, const string &n, Process* from, Process* to,
       Process *trigger, Process *action = 0);
    ~FSMTransition ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    int priority () { return _priority; }
    Process* fsm_action () { return _fsm_action; }
  private:
    void init_FSMTransition ();
    FSMState* _src, *_dst;
    Process *_action, *_trigger;
    Process *_fsm_action;
    Coupling *_c_src;
    int _priority;
  };

  class FSM : public Process
  {
  public:
    FSM ();
    FSM (Process *p, const string &n);
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_cpnt_type () const override { return FSM_T; }
    void update_state (FSMState *s, const string &name) { _cur_state = s; _fsm_state->set_value (name, true); };
    void set_initial (const string &n) { if (_str_initial.length() == 0) _str_initial = n; };
    void draw () override;
    void pick () override;
    void add_state (FSMState* st) { _states.push_back(st); };
    void add_transition (FSMTransition* tr) { _transitions.push_back(tr); };
    virtual ~FSM ();
    int priority () { return _priority; }
    void increase_priority () { _priority++; }
    void serialize (const string& type) override;
  private:
    void init_FSM (); 
    string _str_initial;
    TextProperty *_fsm_state, *_initial;
    FSMState* _cur_state;
    list<FSMState*> _states;
    list<FSMTransition*> _transitions;
    int _priority;
  };

}
