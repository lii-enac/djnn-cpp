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
#include <stdexcept>

#include "process_handler.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "core/utils/algorithm.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h"

namespace djnn {
ProcessDeleter::ProcessDeleter (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _del (this, "del", nullptr),
      _del_one (this, "delete_one_action"),
      _c_del (&_del, ACTIVATION, &_del_one, ACTIVATION, true)
{
    finalize_construction (parent, name);
}

ProcessDeleter::~ProcessDeleter ()
{
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
    auto* to_del = _del.get_value ();
    if (to_del) {
        to_del->deactivate ();
        auto* p = to_del->get_parent ();
        if (p)
            p->remove_child (to_del);
        to_del->schedule_deletion ();
    }
}

ProcessCollector::ProcessCollector (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _s_rm_all (this, "rm_all"),
      _add (this, "add", nullptr),
      _remove (this, "rm", nullptr),
      _size (this, "size", 0),
      _add_one (this, "add_one_action"),
      _rm_one (this, "remove_one_action"),
      _rm_all (this, "remove_all_action"),
      _c_add (&_add, ACTIVATION, &_add_one, ACTIVATION),
      _c_rm (&_remove, ACTIVATION, &_rm_one, ACTIVATION),
      _c_rm_all (&_s_rm_all, ACTIVATION, &_rm_all, ACTIVATION)
{
    graph_add_edge (&_add_one, &_size);
    graph_add_edge (&_rm_one, &_size);
    graph_add_edge (&_rm_all, &_size);
    finalize_construction (parent, name);
}

ProcessCollector::~ProcessCollector ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_rm_all);
        remove_state_dependency (get_parent (), &_rm_one);
        remove_state_dependency (get_parent (), &_add_one);
    }
    graph_remove_edge (&_add_one, &_size);
    graph_remove_edge (&_rm_one, &_size);
    graph_remove_edge (&_rm_all, &_size);
}

void
ProcessCollector::set_parent (CoreProcess* p)
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
    FatProcess::set_parent (p);
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
    auto* to_add = _add.get_value ();
    if (to_add) {
        add_one (to_add);
    }
}

void
ProcessCollector::add_one (CoreProcess* p)
{
    if (p) {
        _list.push_back (p);
        _size.set_value ((int)_list.size (), true);
    }
}

void
ProcessCollector::remove_one ()
{
    auto* to_remove = _remove.get_value ();
    if (to_remove)
        remove_one (to_remove);
}

void
ProcessCollector::remove_one (CoreProcess* p)
{
    if (p) {
        _list.erase (djnnstl::remove (_list.begin (), _list.end (), p), _list.end ());
        _size.set_value ((int)_list.size (), true);
    }
}

void
ProcessCollector::remove_all ()
{
    _list.clear ();
    _size.set_value (0, true);
}

CoreProcess*
ProcessCollector::find_child_impl (const string& path)
{

    /* case of .. and ../foobar */
    if (path[0] == '.' && path[1] == '.') {
        if (get_parent () && path[2] == '/')
            return get_parent ()->find_child_impl (path.substr (3));
        else
            return get_parent ();
    }

    if (path.compare ("rm_all") == 0)
        return &_s_rm_all;
    else if (path.compare ("add") == 0)
        return &_add;
    else if (path.compare ("rm") == 0)
        return &_remove;
    else if (path.compare ("size") == 0)
        return &_size;
    if (path.compare ("add_one_action") == 0)
        return &_add_one;
    else if (path.compare ("remove_one_action") == 0)
        return &_rm_one;
    else if (path.compare ("remove_all_action") == 0)
        return &_rm_all;
    else {
        try {
            string::size_type sz;
            size_t            index = stoi (path, &sz) - 1;
            if (index < _list.size ()) {
                auto* c = _list.at (index);
                if (path.length () > sz) {
                    return c->find_child_impl (path.substr (sz + 1));
                } else
                    return c;
            } else {
                /* we have to dispay index as the API user index */
                warning (this, "index " + __to_string (index + 1) + " is out of bound for ProcessCollector - should be between 1 and size \'" + get_name () + "\'");
            }
        } catch (std::invalid_argument& arg) {
            // FIXME: comment the warning for now because it polluted the find_child ("//*")
            // warning (this, "invalid child path '" + path + "' for ProcessCollector - should be between 1 and size '" + get_name () + "'");
        }
    }
    return nullptr;
}

CoreProcess*
ProcessCollector::find_child_impl (int index)
{
    if ((index - 1) < (int)_list.size ()) {
        return _list.at (index - 1);

    } else {
        /* we have to dispay index as the API user index */
        warning (this, "index " + __to_string (index) + " is out of bound for ProcessCollector - should be between 1 and size \'" + get_name () + "\'");
    }
    return nullptr;
}

CollectionDeleter::CollectionDeleter (CoreProcess* parent, const string& name)
    : FatProcess (name),
      _del (this, "del", nullptr),
      _del_action (this, "del_action"),
      _c_del_all (&_del, ACTIVATION, &_del_action, ACTIVATION)
{
    finalize_construction (parent, name);
}

CollectionDeleter::~CollectionDeleter ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_del_action);
    }
}

void
CollectionDeleter::set_parent (CoreProcess* p)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_del_action);
    }

    add_state_dependency (p, &_del_action);
    FatProcess::set_parent (p);
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
    ProcessCollector* coll = dynamic_cast<ProcessCollector*> (_del.get_value ());
    if (coll) {
        for (auto to_del : coll->get_list ()) {
            to_del->deactivate ();
            auto* p = to_del->get_parent ();
            if (p)
                p->remove_child (to_del);
            to_del->schedule_deletion ();
        }
    }
    coll->remove_all ();
}

CollectionActivator::CollectionActivator (CoreProcess* parent, const string& name, CoreProcess* collection, const string& path)
    : FatProcess (name),
      _activate (this, "activate"),
      _collection (this, "collection", collection),
      _path (this, "path", path),
      _act_all (this, "act_all"),
      _c_act_all (&_collection, ACTIVATION, &_act_all, ACTIVATION)
{
    graph_add_edge (&_activate, &_act_all);
    graph_add_edge (&_path, &_act_all);
    finalize_construction (parent, name);
}

CollectionActivator::~CollectionActivator ()
{
    graph_remove_edge (&_path, &_act_all);
    graph_remove_edge (&_activate, &_act_all);
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_act_all);
    }
}

void
CollectionActivator::set_parent (CoreProcess* p)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_act_all);
    }

    add_state_dependency (p, &_act_all);
    FatProcess::set_parent (p);
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
        string& path = _path.get_value ();
        for (auto to_act : coll->get_list ()) {
            auto* p = to_act->find_child_impl (path);
            if (p)
                p->activate ();
        }
    }
}

AbstractCollectionSetValue::AbstractCollectionSetValue (CoreProcess* parent, const string& name, CoreProcess* collection, const string& path)
    : FatProcess (name),
      _collection (this, "collection", collection),
      _path (this, "path", path),
      _act_set_val (this, "act_all")
{
    graph_add_edge (&_collection, &_act_set_val);
    graph_add_edge (&_path, &_act_set_val);
}

AbstractCollectionSetValue::~AbstractCollectionSetValue ()
{
    graph_remove_edge (&_collection, &_act_set_val);
    graph_remove_edge (&_path, &_act_set_val);
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_act_set_val);
    }
}

void
AbstractCollectionSetValue::set_parent (CoreProcess* p)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_act_set_val);
    }

    add_state_dependency (p, &_act_set_val);
    FatProcess::set_parent (p);
}

CollectionSetDoubleValue::CollectionSetDoubleValue (CoreProcess* parent, const string& name, CoreProcess* collection, const string& path)
    : AbstractCollectionSetValue (parent, name, collection, path),
      _value (this, "value", 0),
      _c_act_set_val (&_value, ACTIVATION, &_act_set_val, ACTIVATION)
{
    finalize_construction (parent, name);
}

CollectionSetDoubleValue::~CollectionSetDoubleValue ()
{
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
        string& path = _path.get_value ();
        for (auto to_act : coll->get_list ()) {
            auto* p = djnn_dynamic_cast<AbstractSimpleProperty*> (to_act->find_child_impl (path));
            if (p)
                p->set_value (_value.get_value (), true);
        }
    }
}

CollectionSetStringValue::CollectionSetStringValue (CoreProcess* parent, const string& name, CoreProcess* collection, const string& path)
    : AbstractCollectionSetValue (parent, name, collection, path),
      _value (this, "value", ""),
      _c_act_set_val (&_value, ACTIVATION, &_act_set_val, ACTIVATION)
{
    finalize_construction (parent, name);
}

CollectionSetStringValue::~CollectionSetStringValue ()
{
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
        string& path = _path.get_value ();
        for (auto to_act : coll->get_list ()) {
            auto* p = djnn_dynamic_cast<AbstractSimpleProperty*> (to_act->find_child_impl (path));
            if (p)
                p->set_value (_value.get_value (), true);
        }
    }
}

NativeCollectionAction::NativeCollectionAction (CoreProcess* parent, const string& name, NativeCollectionCode* action, CoreProcess* coll, void* data, bool isModel)
    : Action (parent, name), _data (data), _action (action), _activation_source (nullptr)
{
    _coll = dynamic_cast<ProcessCollector*> (coll);
    if (coll == nullptr)
        error (this, "Only a ProcessCollector can be used in a NativeCollectionAction");
    set_is_model (isModel);
    finalize_construction (parent, name);
}

NativeCollectionAction::~NativeCollectionAction ()
{
}

void
NativeCollectionAction::impl_activate ()
{
    (_action) (this, _coll->get_list ());
}

void*
NativeCollectionAction::data ()
{
    return _data;
}

void*
get_native_collection_user_data (CoreProcess* native_action)
{
    NativeCollectionAction* na = djnn_dynamic_cast<NativeCollectionAction*> (native_action);
    if (na == nullptr)
        return nullptr;
    return na->data ();
}
} // namespace djnn
