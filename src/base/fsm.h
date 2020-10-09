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
#include "core/execution/graph.h"

#include "core/utils/djnn_dynamic_cast.h"

#include <list>

namespace djnn {

  class FSM;
  class FSMState;
  class FSMTransition;

  class FSMState : public Container
  {
  public:
    FSMState (ParentProcess* parent, const std::string& name);
    ~FSMState () { clean_up_content () ; _transitions.clear (); };
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
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
      FSMTransitionAction (ParentProcess* parent, const std::string& name, FSMState* src, FSMState* dst, CoreProcess* action) :
	       Action (parent, name), _src (src), _dst (dst), _action (action) { _t = dynamic_cast<FSMTransition*> (parent); }
      virtual ~FSMTransitionAction () {};
      void impl_activate ();
    private:
      FSMTransition *_t;
      FSMState* _src;
      FSMState* _dst;
      CoreProcess* _action;
    };
  public:
    FSMTransition (ParentProcess* parent, const std::string& name, CoreProcess* from, CoreProcess* to,
		   CoreProcess *trigger, const std::string& tspec, CoreProcess *action = 0, const std::string& aspec = "");
    FSMTransition (ParentProcess* parent, const std::string& name, CoreProcess* from, CoreProcess* to,
       CoreProcess *trigger, CoreProcess *action = 0);
    ~FSMTransition ();
    void impl_activate () override;
    void impl_deactivate () override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
    int priority () { return _priority; }
    Action* fsm_action () { return &_fsm_action; }
  protected:
    struct Init { Init (FSMTransition* t, ParentProcess* parent, 
                        const std::string& tspec, const std::string& aspec); };
    friend struct Init;
    
  private:
    void init_FSMTransition ();
    FSMState* _from_state, *_to_state;
    CoreProcess *_trigger;
    CoreProcess *_action;
    Init _init;
    FSMTransitionAction _fsm_action;
    Coupling _c_src;
    int _priority;
  };

  class FSM : public FatProcess
  {
  public:
    FSM (ParentProcess* parent, const std::string& name);
    void impl_activate () override;
    void impl_deactivate () override;
    virtual process_type_e get_process_type () const override { return FSM_T; }
    void update_state (FSMState *s, const std::string& name) { _cur_state = s; _fsm_state.set_value (name, true); };
    void set_initial (const std::string& n) { if (_initial.get_value().length() == 0) _initial.set_value (n, false); };
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    void add_state (FSMState* st) { Graph::instance().add_edge (st, &_fsm_state); _states.push_back(st); };
    void add_transition (FSMTransition* tr) { _transitions.push_back(tr); };
    virtual ~FSM ();
    int priority () { return _priority; }
    void increase_priority () { _priority++; }
    void set_parent (ParentProcess* parent) override;
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    int _priority;
    FSMState *_cur_state;
    TextProperty _fsm_state, _initial;
    std::vector<FSMState*> _states;
    std::vector<FSMTransition*> _transitions;
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
