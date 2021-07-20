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

  class AbstractList : public Container
  {
  public:
    AbstractList (ParentProcess* parent, const string& name);
    void add_child (FatChildProcess* c, const string& name) override;
    void insert (FatChildProcess* c, const string& spec);
    void remove_child (FatChildProcess* c) override;
    void remove_child (const string& name) override;
    void clear (); /* empty _children without calling delete on each element IF they are pointers */
    FatChildProcess* find_child_impl (const string& path) override;
    FatChildProcess* find_child_impl (int index)  override;
    virtual ~AbstractList () {};
    int size () { return _size.get_value (); }
  protected:
    virtual void finalize_child_insertion (FatChildProcess *child) = 0;
    RefProperty _added, _removed;
    IntProperty _size;

  public:
#ifndef DJNN_NO_DEBUG
    void dump (int level=0) override;
#endif
  };

  class List : public AbstractList
  {
  public:
    List (ParentProcess* parent, const string& name);
    virtual ~List ();
    List* impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones) override;
  private:
    void finalize_child_insertion (FatChildProcess *child) override;
  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class ListIterator : public FatProcess
  {
  public:
    ListIterator (ParentProcess* parent, const string& name, CoreProcess *list, CoreProcess *action, bool model = true);
    virtual ~ListIterator () {}
    void impl_activate () override;
    void impl_deactivate () override {};
    void post_activate () override;
  private:
    CoreProcess *_action;
    List *_list;
  };

  class BidirectionalListIterator : public FatProcess
  {
  private:
    class IterAction : public Action
    {
    public:
      IterAction (ParentProcess* parent, const string& name, CoreProcess *list, RefProperty *iter, IntProperty *index, bool forward);
      virtual ~IterAction () {}
      void impl_activate () override;
    private:
      List* _list;
      RefProperty *_iter;
      IntProperty *_index;
      bool _forward;
    };
    class ResetAction : public Action
    {
    public:
      ResetAction (ParentProcess* parent, const string& name, IntProperty *index);
      virtual ~ResetAction () {}
      void impl_activate () override;
    private:
      IntProperty *_index;
    };
  public:
    BidirectionalListIterator (ParentProcess* parent, const string& name, List* list);
    virtual ~BidirectionalListIterator ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (ParentProcess* parent) override;
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

  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };
}
