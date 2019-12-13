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

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/tree/component.h"
#include "core/tree/text_property.h"

#include <list>

namespace djnn {
  using namespace std;
  class FSM;
  class FSMState;
  class FSMTransition;

  class FSMState : public Container
  {
  public:
    FSMState (Process *parent, const string &name);
    ~FSMState () { clean_up_content () ; _transitions.clear (); };
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
      FSMTransitionAction (Process *parent, const string &name, FSMState* src, FSMState* dst, Process* action) :
	       Action (parent, name), _src (src), _dst (dst), _action (action) { _t = dynamic_cast<FSMTransition*> (parent); }
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
    FSMTransition (Process *parent, const string &name, Process* from, Process* to,
		   Process *trigger, const string &tspec, Process *action = 0, const string &aspec = "");
    FSMTransition (Process *parent, const string &name, Process* from, Process* to,
       Process *trigger, Process *action = 0);
    ~FSMTransition ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    int priority () { return _priority; }
    Process* fsm_action () { return &_fsm_action; }
  protected:
    struct Init { Init (FSMTransition* t, Process* p, 
                        const string &tspec, const string &aspec); };
    friend struct Init;
    
  private:
    void init_FSMTransition ();
    FSMState* _from_state, *_to_state;
    Process *_trigger, *_action;
    Init _init;
    FSMTransitionAction _fsm_action;
    Coupling _c_src;
    int _priority;
  };

  class FSM : public Process
  {
  public:
    FSM (Process *parent, const string &name);
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_cpnt_type () const override { return FSM_T; }
    void update_state (FSMState *s, const string &name) { _cur_state = s; _fsm_state.set_value (name, true); };
    void set_initial (const string &n) { if (_str_initial.length() == 0) _str_initial = n; };
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    void add_state (FSMState* st) { _states.push_back(st); };
    void add_transition (FSMTransition* tr) { _transitions.push_back(tr); };
    virtual ~FSM ();
    int priority () { return _priority; }
    void increase_priority () { _priority++; }
    void set_parent (Process* p) override;
    void serialize (const string& type) override;
  private:
    int _priority;
    string _str_initial;
    FSMState *_cur_state;
    TextProperty _fsm_state, _initial;
    list<FSMState*> _states;
    list<FSMTransition*> _transitions;
  };

}
