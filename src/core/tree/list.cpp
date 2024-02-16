/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include <stdexcept>

#include "list.h"

#include "core/control/spike.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/ontology/coupling.h"
#include "core/serializer/serializer.h"
#include "core/tree/structure_observer.h"
#include "core/utils/algorithm.h"
#include "core/utils/error.h"
#include "core/utils/to_string.h"
#include "core/utils/utils-dev.h"

namespace djnn {

AbstractList::AbstractList (CoreProcess* parent, const string& name)
    : Container (parent, name),
      _added (nullptr, "_added", nullptr),
      _removed (nullptr, "_removed", nullptr),
      _size (nullptr, "_size", 0)
{
    // we have to remove them manually because those symbols are creating with parent=nullptr
    // but their destruction is directly managed in ~AbstractList
    remove_from_parentless_name (&_added);
    remove_from_parentless_name (&_removed);
    remove_from_parentless_name (&_size);
}

#ifndef DJNN_NO_DEBUG
void
AbstractList::dump (int level)
{
    // std::cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ())  << " [ index=" << _children.size () << " ]" << endl ;
    loginfonofl ((get_parent () ? get_parent ()->find_child_name (this) : get_name ()) + " [ index=" + __to_string (_children.size ()) + " ]");
    // FIXME: indent problem
    // for (auto c : _children)
    //   c->dump(level);
}
#endif

void
AbstractList::add_child (CoreProcess* c, const string& name)
{
    if (c == nullptr)
        return;

    Container::push_back_child (c);

    /* We don't want multiple parenthood */
    if (c->get_parent () != nullptr && c->get_parent () != this) {
        c->get_parent ()->remove_child (c);
    }
    for (auto s : structure_observer_list) {
        s->add_child_to_container (this, c, _children.size () - 1);
    }
    finalize_child_insertion (c);
}

void
AbstractList::insert_new_child (Container::ordered_children_t::iterator it, CoreProcess* c)
{
    _children.insert (it, c);
    if (is_altered ())
        _unaltered_children->push_back (c);
    finalize_child_insertion (c);
}

void
AbstractList::insert (CoreProcess* c, const string& spec)
{
    // becareful : insert add a NEW child

    int index = _children.size () - 1;
    if (spec.compare (">") == 0) {
        add_child (c, "");
        for (auto s : structure_observer_list) {
            s->add_child_to_container (this, c, index);
        }
        return;
    }
    if (spec.compare ("<") == 0) {
        auto it = _children.begin ();
        insert_new_child (it, c);
        index = 0;
        for (auto s : structure_observer_list) {
            s->add_child_to_container (this, c, index);
        }
        return;
    }
    try {
        index = stoi (spec.substr (1, string::npos)) - 1;
        if (spec.at (0) == '<') {
            auto it = _children.begin () + index;
            insert_new_child (it, c);
            for (auto s : structure_observer_list) {
                s->add_child_to_container (this, c, index);
            }
            return;
        } else if (spec.at (0) == '>') {
            auto it = _children.begin () + index + 1;
            insert_new_child (it, c);
            for (auto s : structure_observer_list) {
                s->add_child_to_container (this, c, index + 1);
            }
            return;
        } else {
            goto label_error;
        }
    } catch (std::invalid_argument& arg) {
        goto label_error;
    }
label_error:
    warning (this, "invalid specification '" + spec + "' for insertion in list '" + get_name () + "'");
}

void
AbstractList::clear ()
{
    /* empty _children without calling delete on each element IF they are pointers */
    _children.clear ();
    _size.set_value (0, true);
}

void
AbstractList::clean_up_content ()
{
    /* empty _children CALLING delete on each element */
    Container::clean_up_content ();
    _size.set_value (0, true);
}

void
AbstractList::remove_child (CoreProcess* c)
{
    /* check for existance */
    auto it = djnnstl::find (_children.begin (), _children.end (), c);

    /* then remove it and set _removed */
    if (it != _children.end ()) {
        Container::remove_child (c);
        _removed.set_value (c, true);
        _size.set_value (_size.get_value () - 1, true);
    }
}

void
AbstractList::remove_child (const string& name)
{
    size_t index;
    try {
        /* from user index to internal index : -1 */
        index = stoi (name, nullptr) - 1;
        if (index < _children.size ()) {
            auto* c = _children.at (index);
            remove_child (c);
        } else {
            /* we have to dispay index as the API user index */
            warning (this, "index " + __to_string (index + 1) + " is out of bound for list '" + get_name () + "'");
        }
    } catch (std::invalid_argument& arg) {
        warning (this, "invalid child name '" + name + "' for list '" + get_name () + "'");
    }
}

CoreProcess*
AbstractList::find_child_impl (const string& path)
{

    /* case of .. and ../foobar */
    if (path[0] == '.' && path[1] == '.') {
        if (get_parent () && path[2] == '/')
            return get_parent ()->find_child_impl (path.substr (3));
        else
            return get_parent ();
    }
    /* case specific list's children */
    if (path.compare ("$added") == 0)
        return &_added;
    else if (path.compare ("$removed") == 0)
        return &_removed;
    else if (path.compare ("size") == 0)
        return &_size;
    else {
        try {
            string::size_type sz;
            size_t            index = stoi (path, &sz) - 1;
            if (index < _children.size ()) {
                auto* c = _children.at (index);
                if (path.length () > sz) {
                    return c->find_child_impl (path.substr (sz + 1));
                } else
                    return c;
            } else {
                /* we have to dispay index as the API user index */
                warning (this, "index " + __to_string (index + 1) + " is out of bound for list \'" + get_name () + "\'");
            }
        } catch (std::invalid_argument& arg) {
            // FIXME: comment the warning for now because it polluted the find_child ("//*")
            // warning (this, "invalid child path '" + path + "' for list '" + get_name () + "'");
        }
    }
    return nullptr;
}

CoreProcess*
AbstractList::find_child_impl (int index)
{
    if ((index - 1) < (int)_children.size ()) {
        return _children.at (index - 1);

    } else {
        /* we have to dispay index as the API user index */
        warning (this, "index " + __to_string (index) + " is out of bound for list \'" + get_name () + "\'");
    }
    return nullptr;
}

List::List (CoreProcess* parent, const string& name)
    : AbstractList (parent, name)
{
    finalize_construction (parent, name);
}

List::~List ()
{
}

void
List::finalize_child_insertion (CoreProcess* c)
{
    remove_from_parentless_name (c);

    c->set_parent (this);

    if (get_activation_state () == ACTIVATED && c->get_activation_state () == DEACTIVATED) {
        c->activate ();
    } else if (get_activation_state () == DEACTIVATED && c->get_activation_state () == ACTIVATED) {
        c->deactivate ();
    }
    _added.set_value (c, true); // fixme
    _size.set_value (_size.get_value () + 1, true);
}

List*
List::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    /*List* clone = new List (nullptr, get_name ());
    for (auto c: _children) {
      clone->add_child (c->clone (), "");
    }
    return clone;*/

    auto* clone = new List (nullptr, get_name ());
    for (auto c : _children) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->add_child (cclone, this->find_child_name (c));
    }
    origs_clones[this] = clone;
    return clone;
}

#if !defined(DJNN_NO_SERIALIZE)
void
List::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:list");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (type);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

ListIterator::ListIterator (CoreProcess* parent, const string& name, CoreProcess* list, CoreProcess* action, bool model)
    : FatProcess (name, model),
      _action (action)
{
    List* l = dynamic_cast<List*> (list);
    if (l == nullptr)
        error (this, "The list argument must be a List component in list iterator " + name);
    _list = l;
    finalize_construction (parent, name);
}

void
ListIterator::impl_activate ()
{
    for (auto p : _list->children ()) {
        _action->set_data (p);
        _action->activate ();
    }
    notify_activation ();
}

void
ListIterator::post_activate ()
{
    post_activate_auto_deactivate ();
}

BidirectionalListIterator::IterAction::IterAction (CoreProcess* parent, const string& name, CoreProcess* list,
                                                   RefProperty* iter, IntProperty* index,
                                                   bool forward)
    : Action (parent, name), _list (dynamic_cast<List*> (list)), _iter (iter), _index (index), _forward (forward)
{
    if (_list == nullptr)
        error (this, "The set argument must be a List component in set iterator " + name);
}

void
BidirectionalListIterator::IterAction::impl_activate ()
{
    if (get_parent ()->somehow_deactivating ())
        return;
    int index = _index->get_value ();
    if (_forward) {
        if (_list->size () > index) {
            _iter->set_value (_list->children ().at (index - 1), true);
            _index->set_value (index + 1, true);
        }
    } else {
        if (index >= 1) {
            _iter->set_value (_list->children ().at (index - 1), true);
            _index->set_value (index - 1, true);
        }
    }
}

BidirectionalListIterator::ResetAction::ResetAction (CoreProcess* parent, const string& name,
                                                     IntProperty* index)
    : Action (parent, name), _index (index)
{
}

void
BidirectionalListIterator::ResetAction::impl_activate ()
{
    if (get_parent ()->somehow_deactivating ())
        return;
    _index->set_value (1, true);
}

BidirectionalListIterator::BidirectionalListIterator (CoreProcess* parent, const string& name,
                                                      List* list)
    : FatProcess (name),
      _list (dynamic_cast<List*> (list)),
      _next (this, "next"),
      _previous (this, "previous"),
      _reset (this, "reset"),
      _iter (this, "iter", nullptr),
      _index (this, "index", 1),
      _next_action (this, name + "_next_action", _list, &_iter, &_index, true),
      _c_next (&_next, ACTIVATION, &_next_action, ACTIVATION),
      _previous_action (this, name + "_previous_action", _list, &_iter, &_index, false),
      _c_previous (&_previous, ACTIVATION, &_previous_action, ACTIVATION),
      _reset_action (this, name + "_reset_action", &_index),

      _c_reset (&_reset, ACTIVATION, &_reset_action, ACTIVATION)
{

    if (_list == nullptr) {
        warning (this, "list iterator must take a List as its third argument\n");
        return;
    }

    _c_next.disable ();
    _c_previous.disable ();
    _c_reset.disable ();
    finalize_construction (parent, name);
}

BidirectionalListIterator::~BidirectionalListIterator ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_next_action);
        remove_state_dependency (get_parent (), &_previous_action);
        remove_state_dependency (get_parent (), &_reset_action);
    }
}

void
BidirectionalListIterator::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_next_action);
        remove_state_dependency (get_parent (), &_previous_action);
        remove_state_dependency (get_parent (), &_reset_action);
    }

    add_state_dependency (parent, &_next_action);
    add_state_dependency (parent, &_previous_action);
    add_state_dependency (parent, &_reset_action);

    FatProcess::set_parent (parent);
}

void
BidirectionalListIterator::impl_activate ()
{
    _c_next.enable ();
    _c_previous.enable ();
    _c_reset.enable ();
}

void
BidirectionalListIterator::impl_deactivate ()
{
    _c_next.disable ();
    _c_previous.disable ();
    _c_reset.disable ();
}

#if !defined(DJNN_NO_SERIALIZE)
void
BidirectionalListIterator::serialize (const string& type)
{

    string buf;

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("core:bidirectionallistiterator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _list, buf);
    AbstractSerializer::serializer->text_attribute ("list", buf);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
