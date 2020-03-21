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

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "component.h"
#include "ref_property.h"
#include "int_property.h"
#include "spike.h"
#include "core/control/action.h"

namespace djnn {
  //using namespace std;

  class AbstractList : public Container
  {
  public:
    AbstractList (Process *parent, const std::string& name);
    void add_child (Process* c, const std::string& name) override;
    void insert (Process* c, const std::string& spec);
    void remove_child (Process* c) override;
    void remove_child (const std::string& name) override;
    void dump(int level=0) override;
    void clear (); /* empty _children without calling delete on each element IF they are pointers */
    Process* find_component (const std::string& path) override;
    Process* find_component (int index)  override;
    virtual ~AbstractList () {};
    int size () { return _size.get_value (); }
  protected:
    virtual void finalize_child_insertion (Process *child) = 0;
    RefProperty _added, _removed;
    IntProperty _size;
  };

  class List : public AbstractList
  {
  public:
    List (Process *parent, const std::string& name);
    virtual ~List ();
    void serialize (const std::string& type) override;
    Process* clone () override;
  private:
    void finalize_child_insertion (Process *child) override;
  };

  class ListIterator : public Process
  {
  public:
    ListIterator (Process *parent, const std::string& name, Process *list, Process *action, bool model = true);
    virtual ~ListIterator () {}
    void impl_activate () override;
    void impl_deactivate () override {};
    void post_activate () override;
  private:
    Process *_action;
    Container *_list;
  };

  class BidirectionalListIterator : public Process
  {
  private:
    class IterAction : public Action
    {
    public:
      IterAction (Process *parent, const std::string& name, List *list, RefProperty *iter, IntProperty *index, bool forward);
      virtual ~IterAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      List* _list;
      RefProperty *_iter;
      IntProperty *_index;
      bool _forward;
    };
    class ResetAction : public Action
    {
    public:
      ResetAction (Process *parent, const std::string& name, IntProperty *index);
      virtual ~ResetAction () {}
      void impl_activate () override;
      void impl_deactivate () override {}
    private:
      IntProperty *_index;
    };
  public:
    BidirectionalListIterator (Process *parent, const std::string& name, Process* list);
    virtual ~BidirectionalListIterator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const std::string& type) override;
  private:
    void set_parent (Process* p) override;
    List* _list;
    Spike _next, _previous, _reset;
    RefProperty _iter;
    IntProperty _index;

    IterAction  _next_action;
    Coupling _c_next;
    IterAction _previous_action;
    Coupling _c_previous;
    ResetAction _reset_action;
    Coupling _c_reset; 
  };
}
