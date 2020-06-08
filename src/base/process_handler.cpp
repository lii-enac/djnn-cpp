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

#include "process_handler.h"

#include "core/serializer/serializer.h"
#include "core/execution/graph.h"
#include "core/utils/error.h"

namespace djnn
{
  ProcessDeleter::ProcessDeleter (Process *parent, const std::string& name)
  : Process (name),
  _del (this, "del", nullptr),
  _del_one (this, "delete_one_action"),
  _c_del (&_del, ACTIVATION, &_del_one, ACTIVATION)
  {

    Graph::instance ().add_edge (&_del, &_del_one);
    Process::finalize_construction (parent, name);
  }

  ProcessDeleter::~ProcessDeleter ()
  {
    Graph::instance ().remove_edge (&_del, &_del_one);
  }

  void
  ProcessDeleter::set_parent (Process *p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
     remove_state_dependency (get_parent (), &_del_one);
    }

    add_state_dependency (p, &_del_one);
    Process::set_parent (p);
  }
  void
  ProcessDeleter::impl_activate ()
  {
    _c_del.enable ();
  }

  void
  ProcessDeleter::impl_deactivate ()
  {
    _c_del.disable ();
  }

  void
  ProcessDeleter::delete_one ()
  {
    Process *to_del = _del.get_value ();
    if (to_del) {
      to_del->deactivate();
      Process *p = to_del->get_parent ();
      if (p)
        p->remove_child (to_del);
      to_del->schedule_delete();
    }
  }

  ProcessCollector::ProcessCollector (Process *parent, const std::string& name) :
      Process (name),
      _s_rm_all (this, "rm_all"),
      _add (this, "add", nullptr),
      _remove (this, "rm", nullptr),
      _add_one (this, "add_one_action"),
      _rm_one (this, "remove_one_action"),
      _rm_all (this, "remove_all_action"),
      _c_add (&_add, ACTIVATION, &_add_one, ACTIVATION),
      _c_rm (&_remove, ACTIVATION, &_rm_one, ACTIVATION),
      _c_rm_all (&_s_rm_all, ACTIVATION, &_rm_all, ACTIVATION)
  {

    Graph::instance ().add_edge (&_add, &_add_one);
    Graph::instance ().add_edge (&_remove, &_rm_one);
    Graph::instance ().add_edge (&_s_rm_all, &_rm_all);
    Process::finalize_construction (parent, name);
  }

  ProcessCollector::~ProcessCollector ()
  {
    Graph::instance ().remove_edge (&_add, &_add_one);
    Graph::instance ().remove_edge (&_remove, &_rm_one);
    Graph::instance ().remove_edge (&_s_rm_all, &_rm_all);
  }

  void
  ProcessCollector::set_parent (Process *p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_add_one);
      remove_state_dependency (get_parent (), &_rm_one);
      remove_state_dependency (get_parent (), &_rm_all);
    }

    add_state_dependency (p, &_add_one);
    add_state_dependency (p, &_rm_one);
    add_state_dependency (p, &_rm_all);
    Process::set_parent (p);
  }

  void
  ProcessCollector::impl_activate ()
  {
    _c_add.enable ();
    _c_rm.enable ();
    _c_rm_all.enable ();
  }

  void
  ProcessCollector::impl_deactivate ()
  {
    _c_add.disable ();
    _c_rm.disable ();
    _c_rm_all.disable ();
  }

  void
  ProcessCollector::add_one ()
  {
    Process *to_add = _add.get_value ();
    if (to_add) {
      _list.push_back (to_add);
    }
  }

  void
  ProcessCollector::remove_one ()
  {
    Process *to_remove = _remove.get_value ();
    if (to_remove)
      std::remove (_list.begin (), _list.end (), to_remove);
  }

  void
  ProcessCollector::remove_all ()
  {
    _list.clear ();
  }

  CollectionDeleter::CollectionDeleter (Process *parent, const std::string& name) :
      Process (name),
      _del (this, "del", nullptr),
      _del_action (this, "del_action"),
      _c_del_all (&_del, ACTIVATION, &_del_action, ACTIVATION)
  {
    Graph::instance ().add_edge (&_del, &_del_action);
    Process::finalize_construction (parent, name);
  }

  CollectionDeleter::~CollectionDeleter ()
  {
    Graph::instance ().remove_edge (&_del, &_del_action);
  }

  void
  CollectionDeleter::set_parent (Process *p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_del_action);
    }

    add_state_dependency (p, &_del_action);
    Process::set_parent (p);
  }

  void
  CollectionDeleter::impl_activate ()
  {
    _c_del_all.enable ();
  }

  void
  CollectionDeleter::impl_deactivate ()
  {
    _c_del_all.disable ();
  }

  void
  CollectionDeleter::delete_all ()
  {
    ProcessCollector* coll = dynamic_cast<ProcessCollector*> (_del.get_value());
    if (coll) {
      for (auto to_del : coll->get_list ()) {
        to_del->deactivate ();
        Process *p = to_del->get_parent ();
        if (p)
          p->remove_child (to_del);
        to_del->schedule_delete ();
      }
    }
    coll->remove_all ();
  }

  CollectionActivator::CollectionActivator (Process *parent, const std::string& name, Process* collection, const std::string &path) :
      Process (name),
      _activate (this, "activate"),
      _collection (this, "collection", collection),
      _path (this, "path", path),
      _act_all (this, "act_all"),
      _c_act_all (&_collection, ACTIVATION, &_act_all, ACTIVATION)
  {
    Graph::instance ().add_edge (&_activate, &_act_all);
    Graph::instance ().add_edge (&_collection, &_act_all);
    Graph::instance ().add_edge (&_path, &_act_all);
    Process::finalize_construction (parent, name);
  }

  CollectionActivator::~CollectionActivator ()
  {
    Graph::instance ().remove_edge (&_activate, &_act_all);
    Graph::instance ().remove_edge (&_collection, &_act_all);
    Graph::instance ().remove_edge (&_path, &_act_all);
  }

  void
  CollectionActivator::set_parent (Process *p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_act_all);
    }

    add_state_dependency (p, &_act_all);
    Process::set_parent (p);
  }

  void
  CollectionActivator::impl_activate ()
  {
    _c_act_all.enable ();
  }

  void
  CollectionActivator::impl_deactivate ()
  {
    _c_act_all.disable ();
  }

  void
  CollectionActivator::activate_all ()
  {
    ProcessCollector* coll = dynamic_cast<ProcessCollector*> (_collection.get_value ());
    if (coll) {
      std::string path = _path.get_value ();
      for (auto to_act : coll->get_list ()) {
        Process *p = to_act->find_child (path);
        if (p)
          p->activate ();
      }
    }
  }

  AbstractCollectionSetValue::AbstractCollectionSetValue (Process *parent, const std::string& name, Process* collection, const std::string &path) :
    Process (name),
    _collection (this, "collection", collection),
    _path (this, "path", path),
    _act_set_val (this, "act_all")
  {
    Graph::instance ().add_edge (&_collection, &_act_set_val);
    Graph::instance ().add_edge (&_path, &_act_set_val);
  }

  AbstractCollectionSetValue::~AbstractCollectionSetValue ()
  {
    Graph::instance ().remove_edge (&_collection, &_act_set_val);
    Graph::instance ().remove_edge (&_path, &_act_set_val);
  }

  void
  AbstractCollectionSetValue::set_parent (Process *p)
  {
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
      remove_state_dependency (get_parent (), &_act_set_val);
    }

    add_state_dependency (p, &_act_set_val);
    Process::set_parent (p);
  }

  CollectionSetDoubleValue::CollectionSetDoubleValue (Process *parent, const std::string& name, Process* collection, const std::string &path) :
    AbstractCollectionSetValue (parent, name, collection, path),
    _value (this, "value", 0),
    _c_act_set_val (&_value, ACTIVATION, &_act_set_val, ACTIVATION)
    {
      Graph::instance ().add_edge (&_value, &_act_set_val);
      Process::finalize_construction (parent, name);
    }

  CollectionSetDoubleValue::~CollectionSetDoubleValue ()
  {
    Graph::instance ().remove_edge (&_value, &_act_set_val);
  }

  void
  CollectionSetDoubleValue::impl_activate ()
  {
    _c_act_set_val.enable ();
  }

  void
  CollectionSetDoubleValue::impl_deactivate ()
  {
    _c_act_set_val.disable ();
  }

  void
  CollectionSetDoubleValue::set_value ()
  {
    ProcessCollector* coll = dynamic_cast<ProcessCollector*> (_collection.get_value ());
    if (coll) {
      std::string path = _path.get_value ();
      for (auto to_act : coll->get_list ()) {
        AbstractProperty *p = dynamic_cast<AbstractProperty*>(to_act->find_child (path));
        if (p)
          p->set_value (_value.get_value(), true);
      }
    }
  }

  CollectionSetStringValue::CollectionSetStringValue (Process *parent, const std::string& name, Process* collection, const std::string &path) :
    AbstractCollectionSetValue (parent, name, collection, path),
    _value (this, "value", ""),
    _c_act_set_val (&_value, ACTIVATION, &_act_set_val, ACTIVATION)
  {
    Graph::instance ().add_edge (&_value, &_act_set_val);
    Process::finalize_construction (parent, name);
  }

  CollectionSetStringValue::~CollectionSetStringValue ()
  {
    Graph::instance ().remove_edge (&_value, &_act_set_val);
  }

  void
  CollectionSetStringValue::impl_activate ()
  {
    _c_act_set_val.enable ();
  }

  void
  CollectionSetStringValue::impl_deactivate ()
  {
    _c_act_set_val.disable ();
  }

  void
  CollectionSetStringValue::set_value ()
  {
    ProcessCollector* coll = dynamic_cast<ProcessCollector*> (_collection.get_value ());
    if (coll) {
      std::string path = _path.get_value ();
      for (auto to_act : coll->get_list ()) {
        AbstractProperty *p = dynamic_cast<AbstractProperty*> (to_act->find_child (path));
        if (p)
          p->set_value (_value.get_value (), true);
      }
    }
  }
}
