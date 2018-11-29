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

#include <list>
#include <iostream>
#include "../core/tree/int_property.h"
#include "../core/tree/list.h"
#include "../core/tree/spike.h"

namespace djnn {
  using namespace std;

  class SwitchList : public AbstractList
  {
  private:
    class Next : public Process {
    public:
      Next (SwitchList *p) : Process (), _sw (p) { Process::finalize(); }
      virtual ~Next () {}
      void activate () override;
      void deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class Previous : public Process {
    public:
      Previous (SwitchList *p) : Process (), _sw (p) { Process::finalize(); }
      virtual ~Previous () {}
      void activate () override;
      void deactivate () override {}
    private:
      SwitchList* _sw;
    };
    class ChangeIndex : public Process {
    public:
      ChangeIndex (SwitchList *p) : Process (), _sw (p) { Process::finalize(); }
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
