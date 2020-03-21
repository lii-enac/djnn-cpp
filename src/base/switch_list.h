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

#include "core/control/action.h"
#include "core/tree/list.h"
#include "core/tree/spike.h"
#include "core/tree/bool_property.h"

#include <list>


namespace djnn {

  class IntProperty;
  class BoolProperty;

  class SwitchList : public AbstractList
  {
  private:
    class Next : public Action {
    public:
      Next (SwitchList *p);
      virtual ~Next () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class Previous : public Action {
    public:
      Previous (SwitchList *p);
      virtual ~Previous () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class ChangeIndex : public Action {
    public:
      ChangeIndex (SwitchList *p);
      virtual ~ChangeIndex () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      SwitchList* _sw;
    };
  public:
    SwitchList (Process *parent, const std::string& name, bool loop=false);
    virtual ~SwitchList ();
    virtual process_type_e get_cpnt_type () const override { return SWITCH_T; }
    Process* find_child (const std::string& path) override;
    void impl_activate () override;
    void impl_deactivate () override;
    Process* clone () override;
    void serialize (const std::string& type) override;
    Process* item () { return _cur_item; }
    void set_item (Process *item) { _cur_item = item; }
    IntProperty* index () { return &_index; }
    BoolProperty* loop () { return &_loop; }
    void set_parent (Process* p) override;
    
  private:
    void finalize_child_insertion (Process *child) override;
    Process* _cur_item;
    BoolProperty _loop;
    IntProperty _index;
    Spike _next, _previous;
    Next _next_action;
    Previous _previous_action;
    ChangeIndex _change_index_action;
    Coupling _c_next, _c_previous, _c_index;
  };
}
