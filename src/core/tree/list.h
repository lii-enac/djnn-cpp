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

#include "component.h"
#include <list>
#include <iostream>
#include "ref_property.h"
#include "int_property.h"

namespace djnn {
  using namespace std;

  class AbstractList : public Container
  {
  public:
    AbstractList ();
    AbstractList (Process *parent, const string& name);
    void add_child (Process* c, const string& name) override;
    void insert (Process* c, const string& spec);
    void remove_child (Process* c) override;
    void remove_child (const string &name) override;
    void dump(int level=0) override;
    Process* find_component (const string &path) override;
    virtual ~AbstractList () {};
    int size () { return _size->get_value (); }
  protected:
    virtual void finalize_child_insertion (Process *child) = 0;
    shared_ptr<RefProperty> _added, _removed;
    shared_ptr <IntProperty> _size;
  };

  class List : public AbstractList
  {
  public:
    List ();
    List (Process *parent, const string& name);
    virtual ~List ();
    void serialize (const string& type) override;
    Process* clone () override;
  private:
    void finalize_child_insertion (Process *child) override;
  };

  class ListIterator : public Process
  {
  public:
    ListIterator (Process *parent, const string &name, Process *list, Process *action, bool model = true);
    virtual ~ListIterator () {}
    void activate () override;
    void deactivate () override {};
    void post_activate () override;
  private:
    Process *_action;
    Container *_list;
  };

  class BidirectionalListIterator : public Process
  {
  private:
    class IterAction : public Process
    {
    public:
      IterAction (Process *parent, const string& name, List *list, shared_ptr<RefProperty> iter, shared_ptr<IntProperty> index, bool forward);
      virtual ~IterAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      List* _list;
      shared_ptr<RefProperty> _iter;
      shared_ptr<IntProperty> _index;
      bool _forward;
    };
    class ResetAction : public Process
    {
    public:
      ResetAction (Process *parent, const string& name, shared_ptr<IntProperty> index);
      virtual ~ResetAction () {}
      void activate () override;
      void deactivate () override {}
    private:
      shared_ptr<IntProperty> _index;
    };
  public:
    BidirectionalListIterator (Process *parent, const string& name, Process* list);
    virtual ~BidirectionalListIterator ();
    void activate () override;
    void deactivate () override;
    void serialize (const string& type) override;
  private:
    List* _list;
    shared_ptr<Process> _next, _previous, _reset;
    shared_ptr<RefProperty> _iter;
    shared_ptr<IntProperty> _index;
    unique_ptr<Coupling> _c_next, _c_reset, _c_previous;
    unique_ptr<Process> _next_action, _previous_action, _reset_action;
  };
}
