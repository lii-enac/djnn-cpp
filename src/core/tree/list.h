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
#include "core/property/ref_property.h"
#include "core/property/int_property.h"
#include "core/control/spike.h"
#include "core/control/action.h"

namespace djnn {

  class AbstractList : public Container
  {
  public:
    AbstractList (CoreProcess* parent, const string& name);
    void add_child (CoreProcess* c, const string& name) override;
    void insert (CoreProcess* c, const string& spec);
    void remove_child (CoreProcess* c) override;
    void remove_child (const string& name) override;
    void insert_new_child (Container::ordered_children_t::iterator it, CoreProcess *c);
    void clear (); /* empty _children without calling delete on each element IF they are pointers */
    void clean_up_content () override; /* empty _children CALLING delete on each element */
    CoreProcess* find_child_impl (const string& path) override;
    CoreProcess* find_child_impl (int index)  override;
    virtual ~AbstractList () {};
    int size () { return _size.get_value (); }
  protected:
    virtual void finalize_child_insertion (CoreProcess *child) = 0;
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
    List (CoreProcess* parent, const string& name);
    virtual ~List ();
    List* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const override;
  private:
    void finalize_child_insertion (CoreProcess *child) override;
  public:
#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& format) override;
#endif
  };

  class ListIterator : public FatProcess
  {
  public:
    ListIterator (CoreProcess* parent, const string& name, CoreProcess *list, CoreProcess *action, bool model = true);
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
      IterAction (CoreProcess* parent, const string& name, CoreProcess *list, RefProperty *iter, IntProperty *index, bool forward);
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
      ResetAction (CoreProcess* parent, const string& name, IntProperty *index);
      virtual ~ResetAction () {}
      void impl_activate () override;
    private:
      IntProperty *_index;
    };
  public:
    BidirectionalListIterator (CoreProcess* parent, const string& name, List* list);
    virtual ~BidirectionalListIterator ();
    void impl_activate () override;
    void impl_deactivate () override;
  private:
    void set_parent (CoreProcess* parent) override;
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
