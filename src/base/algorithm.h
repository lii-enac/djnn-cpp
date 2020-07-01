/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/spike.h"
#include "core/tree/component.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/bool_property.h"
#include "core/control/action.h"

namespace djnn
{

  class Sorter : public FatProcess
  {
  private:
    class SortAction : public Action
    {
    public:
      SortAction (ParentProcess* parent, const std::string& name) : Action(parent, name) { finalize_construction (parent, name); }
      virtual ~SortAction () {}
      void impl_activate () { ((Sorter*) get_parent())->sort (); }
    };
  public:
    Sorter (ParentProcess* parent, const std::string& name, CoreProcess *container, const std::string& spec);
    virtual ~Sorter () {}
    void impl_activate () override;
    void impl_deactivate () override;
    void sort ();
  private:
    int partition (int i, int j);
    void quick_sort (int i_first, int i_last);
    void merge_sort (int p, int r);
    void merge (int p, int q, int r);
    bool compare (AbstractProperty* left, AbstractProperty* right);
    AbstractProperty* get_and_check (int i);
    AbstractProperty* get_and_check (CoreProcess* i);
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
    BoolProperty _ascending;
    TextProperty _spec;
    Container *_container;
    SortAction _action;
    Spike _sort;
    Coupling _c_sort_action, _c_spec_action;
  };

  class ListOperator : public FatProcess
  {
  private:
    class ListOperatorAction : public Action
    {
    public:
      ListOperatorAction (ParentProcess* parent, const std::string& name) : Action(parent, name) { finalize_construction (parent, name); }
      virtual ~ListOperatorAction () {}
      void impl_activate () { ((ListOperator*) get_parent())->do_action (); }
    };
    class UpdateListOperatorAction : public Action
    {
    public:
      UpdateListOperatorAction (ParentProcess* parent, const std::string& name) : Action(parent, name) { finalize_construction (parent, name); }
      virtual ~UpdateListOperatorAction () {}
      void impl_activate () { ((ListOperator*) get_parent())->update_list (); }
    };
  public:
    ListOperator (ParentProcess* parent, const std::string& name, FatProcess *container, const std::string& spec);
    virtual ~ListOperator ();
    void impl_activate () override;
    void impl_deactivate () override;
    virtual void do_action () = 0;
    void update_list ();
  private:

  protected:
    TextProperty _spec;
    Container *_container;
    UpdateListOperatorAction _update_list;
    ListOperatorAction _action;
    Coupling _c_spec_action;
    Coupling *_c_update_list_action;
    std::vector<Coupling*> _coupling_list;
  };

  class SumList : public ListOperator
  {
  public:
    SumList (ParentProcess* parent, const std::string& name, FatProcess *container, const std::string& spec) : ListOperator (parent, name, container, spec), _output (this, "output", 0) {}
    virtual ~SumList () {}
    void do_action () override;
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    DoubleProperty _output;
  };

  class ProductList : public ListOperator
  {
  public:
    ProductList (ParentProcess* parent, const std::string& name, FatProcess *container, const std::string& spec) : ListOperator (parent, name, container, spec), _output (this, "output", 0) {}
    virtual ~ProductList () {}
    void do_action () override;
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    DoubleProperty _output;
  };

  class MaxList : public ListOperator
  {
  public:
    MaxList (ParentProcess* parent, const std::string& name, FatProcess *container, const std::string& spec) : ListOperator (parent, name, container, spec), _output (this, "output", 0) {}
    virtual ~MaxList () {}
    void do_action () override;
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    DoubleProperty _output;
  };

  class MinList : public ListOperator
  {
  public:
    MinList (ParentProcess* parent, const std::string& name, FatProcess *container, const std::string& spec) : ListOperator (parent, name, container, spec), _output (this, "output", 0) {}
    virtual ~MinList () {}
    void do_action () override;
  protected:
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    DoubleProperty _output;
  };
}
