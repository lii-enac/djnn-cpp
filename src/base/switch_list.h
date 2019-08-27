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

#include "../core/control/action.h"
#include "../core/tree/list.h"
#include "../core/tree/spike.h"

#include <list>


namespace djnn {
  using namespace std;

  class IntProperty;
  class BoolProperty;

  class SwitchList : public AbstractList
  {
  private:
    class Next : public Action {
    public:
      Next (SwitchList *p) : Action (p, "next_action"), _sw (p) { Process::finalize_construction (p); }
      virtual ~Next () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class Previous : public Action {
    public:
      Previous (SwitchList *p) : Action (p, "previous_action"), _sw (p) { Process::finalize_construction (p); }
      virtual ~Previous () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class ChangeIndex : public Action {
    public:
      ChangeIndex (SwitchList *p) : Action (p, "change_index"), _sw (p) { Process::finalize_construction (p); }
      virtual ~ChangeIndex () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
  public:
    SwitchList (bool loop=false);
    SwitchList (Process *parent, const string& name, bool loop=false);
    virtual ~SwitchList ();
    virtual process_type_e get_cpnt_type () const override { return SWITCH_T; }
    Process* find_component (const string &path) override;
    void impl_activate () override;
    void impl_deactivate () override;
    Process* clone () override;
    void serialize (const string& type) override;
    Process* item () { return _cur_item; }
    void set_item (Process *item) { _cur_item = item; }
    IntProperty* index () { return _index; }
    BoolProperty* loop () { return _loop; }
    void set_parent (Process* p) override { add_state_dependency (p, _state_dependency); _parent = p; }
    
  private:
    void init (bool loop);
    void finalize_child_insertion (Process *child) override;
    Spike *_next, *_previous;
    IntProperty *_index;
    BoolProperty *_loop;
    Coupling *_c_next, *_c_previous, *_c_index;
    Process *_next_action, *_previous_action, *_change_index_action;
    Process* _cur_item;
  };
}
