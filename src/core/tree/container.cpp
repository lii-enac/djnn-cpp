/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2023)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "container.h"

#include "core/serializer/serializer.h"
#include "core/tree/component_observer.h"
#include "core/tree/structure_observer.h"
#include "core/utils/algorithm.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"
#include "core/utils/remotery.h"
#include "core/utils/utils-dev.h"
#include "gui/picking/analytical_picking_context.h"

#if !defined(DJNN_NO_DEBUG)
#include <iostream>

// #define DEBUG_DEACTIVATE
#endif

namespace djnn {

Container::Container (CoreProcess* parent, const string& name, bool is_model)
    : FatProcess (name, is_model), _unaltered_children (nullptr)
{
    for (auto s : structure_observer_list) {
        s->add_container (this);
    }
    if (parent) {
        set_state_dependency (parent->get_state_dependency ());
    }
}

// use only for _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL > 1
// but can't use precompiled condition anymore to add/remove it
#ifndef DJNN_NO_DEBUG
static int nb_space = 0;
#endif

void
Container::clean_up_content ()
{
    if (is_altered () && (_children.size () != _unaltered_children->size ())) {
        warning (this, "this container \"" + this->get_debug_name () + "\" has been altered and _chilldren size != that _unaltered_children !!!!\n");
    }

#ifndef DJNN_NO_DEBUG
    if (_DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL >= 2) {
        for (int space = 0; space < nb_space; space++)
            std::cerr << "\t";
        ++nb_space;
        std::cerr << "--- " << get_debug_name ().c_str () << " :" << std::endl;
    }
#endif

    /* choose correct vector */
    ordered_children_t* pvector_to_delete = &_children;
    if (is_altered ()) {
        pvector_to_delete = _unaltered_children;
    }
    auto& vector_to_delete = *pvector_to_delete;

    /* delete and remove children from this container */
    int sz = vector_to_delete.size ();
    for (int i = sz - 1; i >= 0; i--) {

#ifndef DJNN_NO_DEBUG
        if (_DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL >= 2) {
            for (int space = 0; space < nb_space; space++)
                std::cerr << "\t";
            std::cerr << i << ". " << vector_to_delete[i]->get_debug_name ().c_str () << std::endl;
        }
#endif

        if (is_altered ())
            remove_child_from_children_only (vector_to_delete[i]);
        else {
            for (auto s : structure_observer_list)
                s->remove_child_from_container (this, vector_to_delete[i]);
        }
        delete vector_to_delete[i];
        vector_to_delete.pop_back ();
    }

#ifndef DJNN_NO_DEBUG
    if (_DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL >= 1) {
        --nb_space;
    }
#endif
}

Container::~Container ()
{
    if (!_children.empty ())
        clean_up_content ();

    /* remove container from structure_observer_list */
    for (auto s : structure_observer_list)
        s->remove_container (this);

    delete _unaltered_children;
}

void
Container::push_back_child (CoreProcess* child)
{
    _children.push_back (child);
    if (is_altered ())
        _unaltered_children->push_back (child);
}

void
Container::add_child (CoreProcess* child, const string& name)
{
    if (child == nullptr) {
        error (this, " add_child: trying to add '" + name + "' to the parent '" + this->get_name () + "'  but could NOT find it\n");
        return;
    }

    push_back_child (child);

    /* WARNING should we authorize multiple parenthood? */
    if (child->get_parent () != nullptr && child->get_parent () != this) {
        child->get_parent ()->remove_child (child);
    }
    remove_from_parentless_name (child);

    child->set_parent (this);
    add_symbol (name, child);

    if (get_activation_state () == ACTIVATED && !child->is_model ()) {
        child->activate ();
    } else if (child->get_activation_state () == ACTIVATED) {
        child->deactivate ();
    }
    for (auto s : structure_observer_list) {
        s->add_child_to_container (this, child, _children.size () - 1);
    }
}

void
Container::move_child (CoreProcess* child_to_move, child_position_e spec, CoreProcess* child2)
{
    /* create a copy of _children if necessary*/
    if (is_altered () == false) {
        _unaltered_children = new ordered_children_t (_children);
    }

    if (child2 == 0) {
        if (spec == FIRST) {
            Container::remove_child_from_children_only (child_to_move);
            _children.insert (_children.begin (), child_to_move);
            for (auto s : structure_observer_list) {
                s->move_child_to (this, child_to_move, 0, spec, 0);
            }
            return;
        } else if (spec == LAST) {
            Container::remove_child_from_children_only (child_to_move);
            _children.push_back (child_to_move); // it is the same children so do not add it in _not_alterated_children
            for (auto s : structure_observer_list) {
                s->move_child_to (this, child_to_move, 0, spec, _children.size () - 1);
            }
            return;
        } else
            return;
    }
    // check for existance
    auto it = djnnstl::find (_children.begin (), _children.end (), child2);
    if (it == _children.end ()) {
        return;
    } else {
        if (spec == BEFORE) {
            Container::remove_child_from_children_only (child_to_move);
            // update it and index
            it         = djnnstl::find (_children.begin (), _children.end (), child2);
            auto index = std::distance (_children.begin (), it);
            _children.insert (_children.begin () + index, child_to_move); // it is the same children so do not add it in _not_alterated_children
            for (auto s : structure_observer_list) {
                s->move_child_to (this, child_to_move, index, spec, index); // BUG ? : using the same index for _children and GUIstructureHolder ?
            }
            return;
        } else if (spec == AFTER) {
            Container::remove_child_from_children_only (child_to_move);
            // update it and index
            it         = djnnstl::find (_children.begin (), _children.end (), child2);
            auto index = std::distance (_children.begin (), it);
            _children.insert (_children.begin () + index + 1, child_to_move); // it is the same children so do not add it in _not_alterated_children
            for (auto s : structure_observer_list) {
                s->move_child_to (this, child_to_move, index + 1, spec, index); // BUG ? : using the same index for _children and GUIstructureHolder ?
            }
            return;
        } else {
#ifndef DJNN_NO_DEBUG
            loginfo (string ("spec = ") + __to_string (std::underlying_type<child_position_e>::type (spec)));
#endif
            warning (this, (string ("Undefined spec to move child ") + child_to_move->get_name (child_to_move->get_parent ())).c_str ());
        }
    }
}

void
Container::remove_child_from_children_only (CoreProcess* c)
{
    _children.erase (djnnstl::remove (_children.begin (), _children.end (), c), _children.end ());
    for (auto s : structure_observer_list) {
        s->remove_child_from_container (this, c);
    }
}

/* note:
  remove child but do not delete child
*/
void
Container::remove_child (CoreProcess* c)
{
    FatProcess::remove_child (c);
    if (is_altered ())
        _unaltered_children->erase (djnnstl::remove (_unaltered_children->begin (), _unaltered_children->end (), c), _unaltered_children->end ());
    remove_child_from_children_only (c);
}

/* note:
  remove child but do not delete child
*/
void
Container::remove_child (const string& name)
{
    symtable_t::iterator it = find_child_iterator (name);
    if (it != children_end ()) {
        auto* c = it->second;
        Container::remove_child (c);
    } else
        warning (nullptr, " symbol " + name + " not found in Component " + get_name () + "\n");
}

void
Container::swap_children (int i, int j)
{
    auto* buff   = _children[j];
    _children[j] = _children[i];
    _children[i] = buff;
    for (auto s : structure_observer_list) {
        s->swap_children (this, i, j);
    }
}

void
Container::set_child (CoreProcess* child, int i)
{
    _children[i] = child;
    for (auto s : structure_observer_list) {
        s->set_child (this, child, i);
    }
}

void
Container::impl_activate ()
{
    Container* c = djnn_dynamic_cast<Container*> (get_parent ());
    if (c)
        init_context (c->get_context ());
    ComponentObserver::instance ().start_component ();
    /* WARNING Here we don't use C++ iterator as we want to allow
     * the dynamic modification of children list */
    unsigned int i = 0;
    while (i < _children.size ()) {
        if (!_children[i]->is_model ()) {
            _children[i]->activate ();
        }
        i++;
    }
    ComponentObserver::instance ().end_component ();
}

void
Container::impl_deactivate ()
{
    for (auto it = _children.rbegin (); it != _children.rend (); ++it) {
        auto& c = *it;
#ifdef DEBUG_DEACTIVATE
        std::cerr << __FUNCTION__ << " name: " << c->get_debug_name () << std::endl;
#endif
        c->deactivate ();
    }
}

void
Container::update_drawing ()
{
    if (somehow_deactivating ())
        return;

    for (auto c : _children) {
        c->update_drawing ();
    }
}

void
Container::draw ()
{
    warning (this, string ("DEPRECATED - Container::draw - Should not be using - use GUIStructureHolder::draw instead"));

    // // //std::cerr << this << " " << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ <<  std::endl;

    // if (somehow_deactivating ())
    //   return;

    // //rmt_BeginCPUSample(container_draw, RMTSF_Aggregate);
    // ComponentObserver::instance ().start_draw ();
    // for (auto c : _children) {
    //   c->draw ();
    // }
    // ComponentObserver::instance ().end_draw ();
    // //rmt_EndCPUSample();
}

void
Container::pick ()
{
    // std::cerr << this << " " << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ <<  std::endl;
    if (somehow_deactivating ())
        return;

    // rmt_BeginCPUSample(container_pick, RMTSF_Recursive);
    // ComponentObserver::instance ().start_pick ();
    for (auto c : _children) {
        c->pick ();
    }
    // ComponentObserver::instance ().end_pick ();
    // rmt_EndCPUSample();
}

AbstractGShape*
Container::pick_analytical (PickAnalyticalContext& pac)
{
    if (somehow_deactivating ())
        return nullptr;

    PickAnalyticalContext pac_stacked = pac;

    AbstractGShape* picked = nullptr;
    for (auto p : _children) {
        AbstractGShape* picked_ = p->pick_analytical (pac_stacked);
        if (pac_stacked.clipped) {
            return picked;
        }
        if (picked_)
            picked = picked_;
    }
    return picked;
}

#ifndef DJNN_NO_DEBUG
extern int indent;

void
Container::dump (int level)
{
    // same as FatProcess, except in an ordered manner
    loginfonocr ((get_parent () ? get_parent ()->find_child_name (this) : get_name ()) + ": ");

    /* check if the component is empty - should be ?*/
    if (children_empty ()) {
        loginfonofl ("<EMPTY>");
        return;
    }

    /* check if the level is reached */
    if ((level != 0) && (indent == level - 1)) {
        return;
    }

    loginfonofl ("");
    indent++;

    int i = 0;
    // for (symtable_t::iterator it = symtable ().begin (); it != symtable ().end (); ++it) {
    for (auto& it : children ()) {
        auto* child = &*it;
        for (int j = 0; j < indent; j++)
            loginfonocr ("|\t");
        loginfonocr (" +" + __to_string (i++) + " ");
        child->dump (level);
        if (child->get_process_type () != CONTAINER_T || indent == level - 1)
            loginfonofl ("");
    }

    indent--;
}
#endif

void
Container::add_to_context (const string& k, CoreProcess* v)
{
    context_t::iterator it = _cur_context.find (k);
    if (it != _cur_context.end ())
        it->second = v;
    else
        _cur_context.insert (djnnstl::make_pair (k, v));
}

CoreProcess*
Container::get_from_context (const string& k)
{
    context_t::iterator it = _cur_context.find (k);
    if (it != _cur_context.end ())
        return it->second;
    else
        return nullptr;
}

void
Container::init_context (context_t& context)
{
    if (!context.empty ()) {
        _cur_context.insert (context.begin (), context.end ());
    }
}

void
Container::print_children ()
{
#ifndef DJNN_NO_DEBUG
    loginfonofl (get_name () + "'s children:");
    for (auto c : _children) {
        // loginfonofl (c->get_name (c->get_parent()));
        loginfonofl (c->get_debug_name ());
    }
    loginfonocr ("\n");
#endif
}

} // namespace djnn
