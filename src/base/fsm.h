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

#pragma once

#include "../core/tree/component.h"
#include "../core/control/coupling.h"
#include "../core/tree/text_property.h"

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
    void activate () override;
    void deactivate () override;
    bool is_highest_priority (FSMTransition *t);
    void disable_transitions (FSMTransition *t);
    void draw () override;
    void add_transition (FSMTransition *t) { FSMTransition* pt = t; _transitions.push_front (pt); };
  private:
    list<FSMTransition*> _transitions;
    FSM* _parent_fsm;
  };

  class FSMTransition : public Process
  {
  private:
    class FSMTransitionAction : public Process
    {
    public:
      FSMTransitionAction (Process *p, const string &n, FSMState* src, FSMState* dst, Process* action) :
	Process (p, n), _src (src), _dst (dst), _action (action) { _t = dynamic_cast<FSMTransition*> (p); }
      virtual ~FSMTransitionAction () {};
      void activate ();
      void deactivate () {};
    private:
      FSMTransition *_t;
      FSMState* _src;
      FSMState* _dst;
      Process* _action;
    };
  public:
    FSMTransition (Process *p, const string &n, Process* from, Process* to,
		   Process *src, const string &spec, Process *action, const string &aspec);
    ~FSMTransition ();
    void activate ();
    void deactivate ();
    int priority () { return _priority; }
    Process* fsm_action () { return _fsm_action.get (); }
  private:
    FSMState* _src, *_dst;
    Process *_action, *_trigger;
    shared_ptr<Process> _fsm_action;
    unique_ptr<Coupling>_c_src;
    int _priority;
  };

  class FSM : public Process
  {
  public:
    FSM (Process *p, const string &n) : Process (p, n), _cur_state (nullptr), _priority (0) { init_FSM (); Process::finalize (); };
    FSM () : Process () { init_FSM (); };
    void activate () override;
    void deactivate () override;
    void update_state (FSMState *s, const string &name) { _cur_state = s; _fsm_state.get()->set_value (name, true); };
    void set_initial (const string &n) { if (_str_initial.length() == 0) _str_initial = n; };
    void draw () override;
    virtual ~FSM ();
    int priority () { return _priority; }
    void increase_priority () { _priority++; }
  private:
    void init_FSM ();
    string _str_initial;
    shared_ptr<TextProperty> _fsm_state, _initial;
    FSMState* _cur_state;
    int _priority;
  };

}
