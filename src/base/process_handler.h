/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
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
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/tree/spike.h"
#include "core/tree/ref_property.h"
#include "core/tree/text_property.h"
#include "core/tree/double_property.h"

namespace djnn
{

  class ProcessDeleter : public Process
  {
  private:
    class DeleteOneAction : public Action
    {
    public:
      DeleteOneAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~DeleteOneAction () {}
      void impl_activate () override { ((ProcessDeleter*)get_parent ())->delete_one (); };
      void impl_deactivate () override {};
    };
  public:
    ProcessDeleter (Process *parent, const std::string& name);
    virtual ~ProcessDeleter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void delete_one ();
  protected:
    void set_parent (Process* p) override;
    RefProperty _del;
    DeleteOneAction _del_one;
    Coupling _c_del;
  };

  /*class ProcessMover : public Process
  {
  //TODO
  };*/

  class CollectionDeleter : public Process
  {
  private:
    class DeleteAllAction : public Action
    {
    public:
      DeleteAllAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~DeleteAllAction () {}
      void impl_activate () override { ((CollectionDeleter*)get_parent ())->delete_all (); };
      void impl_deactivate () override {};
    };
  public:
    CollectionDeleter (Process *parent, const std::string& name);
    virtual ~CollectionDeleter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void delete_all ();
  private:
    void set_parent (Process *p) override;
    RefProperty _del;
    DeleteAllAction _del_action;
    Coupling _c_del_all;
  };

  class CollectionActivator : public Process
  {
  private:
    class ActivateAllAction : public Action
    {
    public:
      ActivateAllAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~ActivateAllAction () {}
      void impl_activate () override { ((CollectionActivator*)get_parent ())->activate_all (); };
      void impl_deactivate () override {};
    };
  public:
    CollectionActivator (Process *parent, const std::string& name, Process* collection = nullptr, const std::string& path = std::string(""));
    virtual ~CollectionActivator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void activate_all ();
  private:
    void set_parent (Process *p) override;
    Spike _activate;
    RefProperty _collection;
    TextProperty _path;
    ActivateAllAction _act_all;
    Coupling _c_act_all;
  };

  class AbstractCollectionSetValue : public Process
  {
  private:
    class SetValueAction : public Action
    {
    public:
      SetValueAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~SetValueAction () {}
      void impl_activate () override { ((AbstractCollectionSetValue*)get_parent ())->set_value (); };
      void impl_deactivate () override {};
    };
  public:
    AbstractCollectionSetValue (Process *parent, const std::string& name, Process* collection = nullptr, const std::string& path = std::string(""));
    virtual ~AbstractCollectionSetValue ();
    virtual void set_value () = 0;
  protected:
    void set_parent (Process *p) override;
    RefProperty _collection;
    TextProperty _path;
    SetValueAction _act_set_val;
  };

  class CollectionSetDoubleValue : public AbstractCollectionSetValue
  {
  public:
    CollectionSetDoubleValue (Process *parent, const std::string& name, Process* collection = nullptr, const std::string& path = std::string(""));
    virtual ~CollectionSetDoubleValue ();
    void impl_activate () override;
    void impl_deactivate () override;
    void set_value () override;
  private:
    DoubleProperty _value;
    Coupling _c_act_set_val;
  };

  class CollectionSetStringValue : public AbstractCollectionSetValue
  {
  public:
    CollectionSetStringValue (Process *parent, const std::string& name, Process* collection = nullptr, const std::string& path = std::string(""));
    virtual ~CollectionSetStringValue ();
    void impl_activate () override;
    void impl_deactivate () override;
    void set_value () override;
  private:
    TextProperty _value;
    Coupling _c_act_set_val;
  };

  class ProcessCollector : public Process
  {
  private:
   class AddOneAction : public Action
   {
   public:
     AddOneAction (Process *parent, const std::string& name) : Action (parent, name) {}
     virtual ~AddOneAction () {}
     void impl_activate () override { ((ProcessCollector*)get_parent ())->add_one (); };
     void impl_deactivate () override {};
   };
   class RemoveOneAction : public Action
   {
   public:
     RemoveOneAction (Process *parent, const std::string& name) : Action (parent, name) {}
     virtual ~RemoveOneAction () {}
     void impl_activate () override { ((ProcessCollector*)get_parent ())->remove_one (); };
     void impl_deactivate () override {};
    };
    class RemoveAllAction : public Action
    {
    public:
      RemoveAllAction (Process *parent, const std::string& name) : Action (parent, name) {}
      virtual ~RemoveAllAction () {}
      void impl_activate () override { ((ProcessCollector*)get_parent ())->remove_all (); };
      void impl_deactivate () override {};
    };
  public:
    ProcessCollector (Process *parent, const std::string& name);
    virtual ~ProcessCollector ();
    void impl_activate () override;
    void impl_deactivate () override;
    void add_one ();
    void remove_one ();
    void remove_all ();
    std::vector<Process*> get_list () { return _list; }
  protected:
    void set_parent (Process* p) override;
    Spike _s_rm_all;
    RefProperty _add;
    RefProperty _remove;
    std::vector<Process*> _list;
    AddOneAction _add_one;
    RemoveOneAction _rm_one;
    RemoveAllAction _rm_all;
    Coupling _c_add, _c_rm, _c_rm_all;
  };
}
