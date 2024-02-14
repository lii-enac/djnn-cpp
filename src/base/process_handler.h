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
#include "core/control/spike.h"
#include "core/property/ref_property.h"
#include "core/property/text_property.h"
#include "core/property/int_property.h"
#include "core/property/double_property.h"

namespace djnn
{

  class ProcessDeleter : public FatProcess
  {
  private:
    class DeleteOneAction : public Action
    {
    public:
      DeleteOneAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~DeleteOneAction () {}
      void impl_activate () override { ((ProcessDeleter*)get_parent ())->delete_one (); };
    };
  public:
    ProcessDeleter (CoreProcess *parent, const string& name);
    virtual ~ProcessDeleter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void delete_one ();
  protected:
    RefProperty _del;
    DeleteOneAction _del_one;
    Coupling _c_del;
  };

  /*class ProcessMover : public Process
  {
  //TODO
  };*/

  class CollectionDeleter : public FatProcess
  {
  private:
    class DeleteAllAction : public Action
    {
    public:
      DeleteAllAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~DeleteAllAction () {}
      void impl_activate () override { ((CollectionDeleter*)get_parent ())->delete_all (); };
    };
  public:
    CollectionDeleter (CoreProcess *parent, const string& name);
    virtual ~CollectionDeleter ();
    void impl_activate () override;
    void impl_deactivate () override;
    void delete_all ();
  private:
    void set_parent (CoreProcess *p) override;
    RefProperty _del;
    DeleteAllAction _del_action;
    Coupling _c_del_all;
  };

  class CollectionActivator : public FatProcess
  {
  private:
    class ActivateAllAction : public Action
    {
    public:
      ActivateAllAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~ActivateAllAction () {}
      void impl_activate () override { ((CollectionActivator*)get_parent ())->activate_all (); };
    };
  public:
    CollectionActivator (CoreProcess *parent, const string& name, CoreProcess* collection = nullptr, const string& path = string(""));
    virtual ~CollectionActivator ();
    void impl_activate () override;
    void impl_deactivate () override;
    void activate_all ();
  private:
    void set_parent (CoreProcess *p) override;
    Spike _activate;
    RefProperty _collection;
    TextProperty _path;
    ActivateAllAction _act_all;
    Coupling _c_act_all;
  };

  class AbstractCollectionSetValue : public FatProcess
  {
  private:
    class SetValueAction : public Action
    {
    public:
      SetValueAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~SetValueAction () {}
      void impl_activate () override { ((AbstractCollectionSetValue*)get_parent ())->set_value (); };
    };
  public:
    AbstractCollectionSetValue (CoreProcess *parent, const string& name, CoreProcess* collection = nullptr, const string& path = string(""));
    virtual ~AbstractCollectionSetValue ();
    virtual void set_value () = 0;
  protected:
    void set_parent (CoreProcess *p) override;
    RefProperty _collection;
    TextProperty _path;
    SetValueAction _act_set_val;
  };

  class CollectionSetDoubleValue : public AbstractCollectionSetValue
  {
  public:
    CollectionSetDoubleValue (CoreProcess *parent, const string& name, CoreProcess* collection = nullptr, const string& path = string(""));
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
    CollectionSetStringValue (CoreProcess *parent, const string& name, CoreProcess* collection = nullptr, const string& path = string(""));
    virtual ~CollectionSetStringValue ();
    void impl_activate () override;
    void impl_deactivate () override;
    void set_value () override;
  private:
    TextProperty _value;
    Coupling _c_act_set_val;
  };

  class ProcessCollector : public FatProcess
  {
  private:
   class AddOneAction : public Action
   {
   public:
     AddOneAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
     virtual ~AddOneAction () {}
     void impl_activate () override { ((ProcessCollector*)get_parent ())->add_one (); };
   };
   class RemoveOneAction : public Action
   {
   public:
     RemoveOneAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
     virtual ~RemoveOneAction () {}
     void impl_activate () override { ((ProcessCollector*)get_parent ())->remove_one (); };
    };
    class RemoveAllAction : public Action
    {
    public:
      RemoveAllAction (CoreProcess *parent, const string& name) : Action (parent, name) {}
      virtual ~RemoveAllAction () {}
      void impl_activate () override { ((ProcessCollector*)get_parent ())->remove_all (); };
    };
  public:
    ProcessCollector (CoreProcess *parent, const string& name);
    virtual ~ProcessCollector ();
    void impl_activate () override;
    void impl_deactivate () override;
    void add_one ();
    void add_one (CoreProcess* p);
    void remove_one ();
    void remove_one (CoreProcess* p);
    void remove_all ();
    const vector<CoreProcess*>& get_list () { return _list; }
    process_type_e get_process_type () const override { return PROCESS_COLLECTOR_T; }
  protected:
    void set_parent (CoreProcess* p) override;
    Spike _s_rm_all;
    RefProperty _add;
    RefProperty _remove;
    IntProperty _size;
    vector<CoreProcess*> _list;
    AddOneAction _add_one;
    RemoveOneAction _rm_one;
    RemoveAllAction _rm_all;
    Coupling _c_add, _c_rm, _c_rm_all;
  };

  typedef void (NativeCollectionCode) (CoreProcess*, djnnstl::vector<CoreProcess*>);

  class NativeCollectionAction : public Action
  {
  public:
    NativeCollectionAction (CoreProcess* parent, const string& name, NativeCollectionCode *action, CoreProcess* coll, void* data, bool isModel);
    virtual ~NativeCollectionAction ();
    virtual process_type_e get_process_type () const override { return NATIVE_COLLECTION_ACTION_T; }
    void impl_activate () override;
    void* data ();

    virtual void set_activation_source (CoreProcess* src) override { _activation_source = src; }
    virtual CoreProcess* get_activation_source () override { return _activation_source; }

  private:
    void *_data;
    NativeCollectionCode *_action;
    CoreProcess *_activation_source;
    ProcessCollector* _coll;
  };
  void* get_native_collection_user_data (CoreProcess* native_collection_action);
}
