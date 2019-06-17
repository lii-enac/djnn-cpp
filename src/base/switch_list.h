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

#include "../core/tree/list.h"

//#include "../core/tree/int_property.h"
#include "../core/tree/spike.h"

#include <list>
#include <iostream>


namespace djnn {
  using namespace std;

  class IntProperty;
  class BoolProperty;

  class SwitchList : public AbstractList
  {
  private:
    class Next : public Action {
    public:
      Next (SwitchList *p) : Action (), _sw (p) { Process::finalize_construction (); }
      virtual ~Next () {}
      void activate () override;
      void deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class Previous : public Action {
    public:
      Previous (SwitchList *p) : Action (), _sw (p) { Process::finalize_construction (); }
      virtual ~Previous () {}
      void activate () override;
      void deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class ChangeIndex : public Action {
    public:
      ChangeIndex (SwitchList *p) : Action (), _sw (p) { Process::finalize_construction (); }
      virtual ~ChangeIndex () {}
      void activate () override;
      void deactivate () override {}
    private:
      SwitchList* _sw;
    };
  public:
    SwitchList (bool loop=false);
    SwitchList (Process *parent, const string& name, bool loop=false);
    virtual ~SwitchList ();
    virtual int get_cpnt_type () override { return SWITCH_T; }
    Process* find_component (const string &path) override;
    void activate () override;
    void deactivate () override;
    Process* clone () override;
    void serialize (const string& type) override;
    Process* item () { return _cur_item; }
    void set_item (Process *item) { _cur_item = item; }
    IntProperty* index () { return _index; }
    BoolProperty* loop () { return _loop; }
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
