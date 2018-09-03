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
    SwitchList ();
    SwitchList (Process *parent, const string& name);
    virtual ~SwitchList ();
    Process* find_component (const string &path) override;
    void activate () override;
    void deactivate () override;
    Process* clone () override;
    void serialize (const string& type) override;
    Process* item () { return _cur_item; }
    void set_item (Process *item) { _cur_item = item; }
    IntProperty* index () { return _index.get (); }
  private:
    void init ();
    void finalize_child_insertion (Process *child) override;
    shared_ptr<Spike> _next, _previous;
    shared_ptr <IntProperty> _index;
    unique_ptr<Coupling> _c_next, _c_previous, _c_index;
    shared_ptr<Process> _next_action, _previous_action, _change_index_action;
    Process* _cur_item;
  };
}
