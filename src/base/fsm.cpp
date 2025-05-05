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
#include "core/tree/structure_observer.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"

namespace djnn {

FSMState::FSMState (CoreProcess* parent, const string& name)
    : Container (parent, name)
{
    FSM* fsm = djnn_dynamic_cast<FSM*> (parent);
    if (fsm == nullptr) {
        warning (this, "only a FSM can be the parent of a FSM State\n");
        return;
    }
    fsm->set_initial (name);
    _parent_fsm = fsm;
    _parent_fsm->FSM::add_state (this);

    // connect fsm_state to fsm by structure holder
    for (auto s : structure_observer_list) {
        s->add_child_to_container (_parent_fsm, this, _children.size () - 1);
    }

    finalize_construction (parent, name);
}

FSMState::~FSMState ()
{
    clean_up_content ();
    _transitions.clear ();

    // disconnect fsm_state from fsm by structure holder
    for (auto s : structure_observer_list) {
        s->remove_child_from_container (_parent_fsm, this);
    }
}

bool
FSMState::is_highest_priority (FSMTransition* t)
{
    int priority = t->priority ();
    for (auto t2 : _transitions) {
        if (t2 != t && t2->transition_action ()->get_activation_flag () == ACTIVATION && t2->priority () < priority)
            return false;
    }
    return true;
}

void
FSMState::disable_transitions (FSMTransition* t)
{
    for (auto t2 : _transitions) {
        if (t2 != t && t2->transition_action ()->get_activation_flag () == ACTIVATION) {
            t2->transition_action ()->set_activation_flag (NONE_ACTIVATION);
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
FSMState::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:fsmstate");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

FSMTransition::Init::Init (FSMTransition* t, CoreProcess* parent,
                           const string& tspec, const string& aspec)
{
    FSM* fsm = djnn_dynamic_cast<FSM*> (parent);
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

FSMTransition::FSMTransition (CoreProcess* parent, const string& name, CoreProcess* from, CoreProcess* to,
                              CoreProcess* trigger, const string& tspec, CoreProcess* action,
                              const string& aspec)
    : FatProcess (name),
      _priority (0),
      _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
      _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
      _trigger (trigger ? trigger->find_child_impl (tspec) : nullptr),
      _init (this, parent, tspec, aspec),
      _transition_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, action ? action->find_child_impl (aspec) : nullptr),
      _c_src (), _c_trigger_to_action ()
{
    CoreProcess* _action = action ? action->find_child_impl (aspec) : nullptr;
    _c_src.init (_trigger, ACTIVATION, &_transition_action, ACTIVATION);
    _c_src.disable ();
    if (_action) {
        _c_trigger_to_action.init (_trigger, ACTIVATION, _action, ACTIVATION);
        _c_trigger_to_action.disable ();
        graph_add_edge (_action, &_transition_action);
    }

    graph_add_edge (&_transition_action, parent->find_child ("state"));

    finalize_construction (parent, name);
    FSM* fsm = djnn_dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition (this);
}

FSMTransition::FSMTransition (CoreProcess* parent, const string& name, CoreProcess* from, CoreProcess* to,
                              CoreProcess* trigger, CoreProcess* action)
    : FatProcess (name),
      _priority (0),
      _from_state (from ? dynamic_cast<FSMState*> (from) : nullptr),
      _to_state (to ? dynamic_cast<FSMState*> (to) : nullptr),
      _trigger (trigger),
      _init (this, parent, "NO spec for trigger", "NO spec for action"),
      _transition_action (this, "transition_action_" + _from_state->get_name () + "_" + _to_state->get_name (), _from_state, _to_state, action),
      _c_src (), _c_trigger_to_action ()
{
    _c_src.init (_trigger, ACTIVATION, &_transition_action, ACTIVATION);
    _c_src.disable ();
    if (action) {
        _c_trigger_to_action.init (_trigger, ACTIVATION, action, ACTIVATION);
        _c_trigger_to_action.disable ();
        graph_add_edge (action, &_transition_action);
    }

    graph_add_edge (&_transition_action, from);
    graph_add_edge (&_transition_action, to);

    finalize_construction (parent, name);
    FSM* fsm = djnn_dynamic_cast<FSM*> (parent);
    fsm->FSM::add_transition (this);
}

FSMTransition::~FSMTransition ()
{
    graph_remove_edge (&_transition_action, get_parent ()->find_child ("state"));
    if (_c_trigger_to_action.is_effective ()) {
        graph_remove_edge (_c_trigger_to_action.get_dst (), &_transition_action);
    }
}

void
FSMTransition::impl_activate ()
{
    _c_src.enable ();
    if (_c_trigger_to_action.is_effective ())
        _c_trigger_to_action.enable ();
}

void
FSMTransition::impl_deactivate ()
{
    _c_src.disable ();
    if (_c_trigger_to_action.is_effective ())
        _c_trigger_to_action.disable ();
}

void
FSMTransition::FSMTransitionAction::impl_activate ()
{
    if (((FSM*)_t->get_parent ())->is_already_triggered ())
        return;
    _src->disable_transitions (_t);
    notify_activation ();
    if (_src != _dst) {
        _src->deactivate ();
        _dst->activate ();
    }
    ((FSM*)_t->get_parent ())->set_triggered (1);
}

#ifndef DJNN_NO_SERIALIZE
void
FSMTransition::serialize (const string& type)
{

    string buf;

    AbstractSerializer::pre_serialize (this, type);

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
    CoreProcess* _action = _c_trigger_to_action.is_effective () ? _c_trigger_to_action.get_dst () : nullptr;
    AbstractSerializer::compute_path (get_parent (), _action, buf);
    AbstractSerializer::serializer->text_attribute ("action", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

FSM::FSM (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _priority (0),
      _already_triggered (0),
      _cur_state (nullptr),
      _fsm_state (this, "state", ""),
      _initial (this, "initial", ""),
      _post_trigger (this, "post_trigger"),
      _has_been_initialized_with_parent (false)
{
    // to separe cases between djnn (often with parent) and smala (no parent at init)
    // no parent at init = no state_dependency with parent initialized
    if (parent)
        _has_been_initialized_with_parent = true;

    set_state_dependency (&_fsm_state);
    /* with state_dependency */
    finalize_construction (parent, name, &_fsm_state);

    // add FSM has potentiel GUIStrucureHolder or GUI container
    // TODO (2025.05): Commented out code because everything is now handled in GUIStructureObserver::add_child_to_container through ensure_container_registered
    // for (auto s : structure_observer_list) {
    //     s->add_container (this);
    // }
}

FSM::~FSM ()
{
    if (get_parent ()) {
        // remove current state_dependency
        remove_state_dependency (get_parent (), get_state_dependency ());
        // remove original state_dependency that may has changed
        // from _fsm_state if the fsm is a child of another fsm.state, switch.state ...
        // also do not exist when the fsm is initialized with no parent.
        if (_has_been_initialized_with_parent)
            remove_state_dependency (get_parent (), &_fsm_state);
    }

    int sz = _transitions.size ();
    /*remove graph edges*/
    if (sz > 0) {
        for (int i = 0; i < sz - 1; i++) {
            graph_remove_edge (_transitions[i]->transition_action (), _transitions[i + 1]->transition_action ());
        }
        graph_remove_edge (_transitions[sz - 1]->transition_action (), &_post_trigger);
    }
    /* inverse delete of _transition */
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
FSM::add_transition (FSMTransition* tr)
{
    _transitions.push_back (tr);
    int p = tr->priority ();
    if (p > 0) {
        graph_add_edge (_transitions.at (p - 1)->transition_action (), tr->transition_action ());
        graph_remove_edge (_transitions.at (p - 1)->transition_action (), &_post_trigger);
    }
    graph_add_edge (tr->transition_action (), &_post_trigger);
};

void
FSM::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), get_state_dependency ());
    }

    add_state_dependency (parent, get_state_dependency ());
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
    warning (this, string ("DEPRECATED - FSM::draw - Should not be using - use GUIStructureHolder::draw instead"));

    // if (somehow_deactivating ())
    //   return;

    // if (_cur_state != nullptr)
    //   _cur_state->draw ();
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
FSM::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:fsm");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto st : _states)
        st->serialize (type);
    for (auto tr : _transitions)
        tr->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
