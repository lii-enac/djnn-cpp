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
#include "core/core-dev.h" // graph add/remove edge

#include "core/utils/djnn_dynamic_cast.h"

#include <list>

namespace djnn {

  class FSM;
  class FSMState;
  class FSMTransition;

  class FSMState : public Container
  {
  public:
    FSMState (ParentProcess* parent, const string& name);
    ~FSMState () { clean_up_content () ; _transitions.clear (); };
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
    bool is_highest_priority (FSMTransition *t);
    void disable_transitions (FSMTransition *t);
    void add_transition (FSMTransition *t) { FSMTransition* pt = t; _transitions.push_front (pt); };
  private:
    std::list<FSMTransition*> _transitions;
    FSM* _parent_fsm;
  };

  class FSMTransition : public FatProcess
  {
  private:
    class FSMTransitionAction : public Action
    {
    public:
      FSMTransitionAction (ParentProcess* parent, const string& name, FSMState* src, FSMState* dst, CoreProcess* action) :
	       Action (parent, name), _src (src), _dst (dst) { _t = dynamic_cast<FSMTransition*> (parent); }
      virtual ~FSMTransitionAction () {};
      void impl_activate ();
    private:
      FSMTransition *_t;
      FSMState* _src;
      FSMState* _dst;
    };
  public:
    FSMTransition (ParentProcess* parent, const string& name, CoreProcess* from, CoreProcess* to,
		   CoreProcess *trigger, const string& tspec, CoreProcess *action = 0, const string& aspec = "");
    FSMTransition (ParentProcess* parent, const string& name, CoreProcess* from, CoreProcess* to,
       CoreProcess *trigger, CoreProcess *action = 0);
    ~FSMTransition ();
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
    int priority () { return _priority; }
    Action* transition_action () { return &_transition_action; }
  protected:
    struct Init { Init (FSMTransition* t, ParentProcess* parent, 
                        const string& tspec, const string& aspec); };
    friend struct Init;
    
  private:
    void init_FSMTransition ();
    int _priority;
    FSMState* _from_state, *_to_state;
    CoreProcess *_trigger;
    Init _init;
    FSMTransitionAction _transition_action;
    Coupling _c_src, _c_trigger_to_action;
  };

  class FSM : public FatProcess
  {
      class FSMPostTriggerAction : public Action
      {
         public:
          FSMPostTriggerAction (ParentProcess* parent, const string& name) :
               Action (parent, name) { }
          virtual ~FSMPostTriggerAction () {};
          void impl_activate () { ((FSM*)get_parent())->set_triggered (0); };
      };
  public:
    FSM (ParentProcess* parent, const string& name);
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_process_type () const override { return FSM_T; }
    void update_state (FSMState *s, const string& name) { _cur_state = s; _fsm_state.set_value (name, true); };
    void set_initial (const string& n) { if (_initial.get_value().length() == 0) _initial.set_value (n, false); };
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    void add_state (FSMState* st) { graph_add_edge (st, &_fsm_state); _states.push_back(st); };
    void add_transition (FSMTransition* tr);
    virtual ~FSM ();
    int priority () { return _priority; }
    void increase_priority () { _priority++; }
    void set_parent (ParentProcess* parent) override;
    void set_triggered (int v) { _already_triggered = v; if (v) _post_trigger.set_activation_flag(ACTIVATION); }
    int is_already_triggered () { return _already_triggered; }
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    int _priority, _already_triggered;
    FSMState *_cur_state;
    TextProperty _fsm_state, _initial;
    FSMPostTriggerAction _post_trigger;
    vector<FSMState*> _states;
    vector<FSMTransition*> _transitions;
  };

#ifdef DJNN_NO_DYNAMIC_CAST
  template <> inline FSM* djnn_dynamic_cast(CoreProcess* p)
  {
    if(p->get_process_type() == FSM_T) {
      return static_cast<FSM*>(p);
    }
    return nullptr;
  }

  /*
  template <> inline FSMState* djnn_dynamic_cast(CoreProcess* p)
  {
    if(p->get_process_type() == FSM_T) {
      return static_cast<FSM*>(p);
    }
    return nullptr;
  }
  */
#endif

}
