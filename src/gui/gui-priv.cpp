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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "gui-priv.h"

#include "core/tree/component.h"
#include "core/tree/component_observer.h"
#include "core/utils/algorithm.h"
#include "display/abstract_display.h"
#include "display/display-dev.h"
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/layer.h"

// #include <boost/range/adaptor/reversed.hpp>

#if _DEBUG_SEE_GUI_INFO_PREF
#include "core/utils/iostream.h"

/* define in gui.cpp */
extern int __nb_Drawing_object;
extern int __nb_Drawing_object_picking;
#endif

namespace djnn {
FatProcess*           GenericMouse;
GUIStructureObserver* gui_structure_observer;

void
GUIStructureHolder::add_gui_child (CoreProcess* c, size_t index)
{
    _children.push_back (children_t::value_type (c, index));
}

void
GUIStructureHolder::remove_gui_child (CoreProcess* c)
{

    // TODO MP : becareful ... how much time does this take ?
    _children.erase (
        djnnstl::remove_if (_children.begin (), _children.end (), [c] (children_t::value_type p) { return p.first == c; }),
        _children.end ());
}

void
GUIStructureHolder::swap_children (size_t i, size_t j)
{
    if (i < _children.size () && j < _children.size ()) {
        auto buff    = _children[j];
        _children[j] = _children[i];
        _children[i] = buff;
    }
}

void
GUIStructureHolder::set_child (CoreProcess* child, size_t i)
{
    if (i < _children.size ()) {
        for (auto p : _children) {
            if (p.second == i) {
                p.first = child;
                break;
            }
        }
    }
}

void
GUIStructureHolder::draw ()
{

#if _DEBUG_SEE_GUI_INFO_PREF
    /* reset  */
    __nb_Drawing_object         = 0;
    __nb_Drawing_object_picking = 0;
#endif

    // if the content_process activated else return
    if (content_process->somehow_deactivating ())
        return;

    // if Layer and the correct frame
    if (content_process->get_process_type () == LAYER_T) {
        Layer* l = dynamic_cast<Layer*> (content_process);
        if (DisplayBackend::instance ()->window () == l->get_frame ()) {
            ComponentObserver::instance ().start_draw ();
            Backend::instance ()->draw_layer (l, _children);
            ComponentObserver::instance ().end_draw ();
        }
    } else if (content_process->get_process_type () == Z_ORDERED_GROUP_T) {
        content_process->draw ();
    }
    // if other container
    else {
        ComponentObserver::instance ().start_draw ();
        for (auto p : _children) {
            p.first->draw ();
        }
        ComponentObserver::instance ().end_draw ();
    }

#if _DEBUG_SEE_GUI_INFO_PREF
    cerr << "\033[1;36m" << endl;
    cerr << "NB DRAWING OBJS: " << __nb_Drawing_object << endl;
    cerr << "NB DRAWING OBJS PICKING VIEW: " << __nb_Drawing_object_picking << endl;
    cerr << "\033[0m";
#endif
}

AbstractGShape*
GUIStructureHolder::pick_analytical (PickAnalyticalContext& pac)
{
    // ComponentObserver::instance ().start_draw ();
    // for (auto p : boost::adaptors::reverse(_children)) {
    AbstractGShape* picked = nullptr;
    for (auto p : _children) {
        AbstractGShape* picked_ = p.first->pick_analytical (pac);
        if (picked_)
            picked = picked_;
    }
    // ComponentObserver::instance ().end_draw ();
    return picked;
}

void
GUIStructureHolder::add_gui_child_at (CoreProcess* c, size_t neighbour_index, child_position_e spec, size_t new_index)
{
    size_t sz = _children.size ();
    switch (spec) {
    case LAST:
        _children.push_back (children_t::value_type (c, new_index));
        break;
    case FIRST:
        _children.insert (_children.begin (), children_t::value_type (c, new_index));
        break;
    case AFTER:
        for (size_t i = 0; i < sz - 1; i++) {
            if (_children.at (i).second == neighbour_index) {
                _children.insert (_children.begin () + i + 1, children_t::value_type (c, new_index));
                break;
            }
        }
        _children.push_back (children_t::value_type (c, new_index));
        break;
    case BEFORE:
        for (size_t i = 0; i < sz; i++) {
            if (_children.at (i).second == neighbour_index) {
                _children.insert (_children.begin () + i, children_t::value_type (c, new_index));
                break;
            }
        }
    }
}

void
GUIStructureHolder::move_child_to (CoreProcess* c, size_t neighbour_index, child_position_e spec, size_t new_index)
{
    _children.erase (
        djnnstl::remove_if (_children.begin (), _children.end (), [c] (children_t::value_type p) { return p.first == c; }),
        _children.end ());
    add_gui_child_at (c, neighbour_index, spec, new_index);
}

GUIStructureObserver::~GUIStructureObserver ()
{
    structures_t::iterator it = _structure_map.begin ();
    while (it != _structure_map.end ()) {
        delete it->second;
        _structure_map.erase (it);
        it = _structure_map.begin ();
    }
}

void
GUIStructureObserver::add_container (FatProcess* cont)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    if (it_cont == _structure_map.end ()) {
        GUIStructureHolder* holder = new GUIStructureHolder (cont);
        _structure_map.insert (pair<CoreProcess*, GUIStructureHolder*> (cont, holder));
    }
}

void
GUIStructureObserver::remove_container (FatProcess* cont)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ()) {
        delete it_cont->second;
        _structure_map.erase (it_cont);
    }
}

// void
// GUIStructureObserver::print_structure_map () {
//   std::cerr << "------- _structure_map ------ " << std::endl;
//   for (auto p : _structure_map) {
//     std::cout << "stucture of - " << cpp_demangle(typeid(*p.first).name()) << " - " << p.first->get_debug_name () << std::endl;
//     int i = 0 ;
//     GUIStructureHolder* GH = dynamic_cast<GUIStructureHolder*>(p.second);
//     if (GH) {
//       //std::cout << "size: " << GH->children ().size () << std::endl;
//       for (auto c : GH->children ()) {
//         std::cout  << "--- c" << i++ << " - " << cpp_demangle(typeid(*c.first).name()) << " - " << c.first->get_debug_name () << std::endl;
//       }
//     }
//   }
//   std::cerr << "---------------------------- \n" << std::endl;
// }

void
GUIStructureObserver::add_child_to_container (FatProcess* cont, CoreProcess* c, int index)
{
    structures_t::iterator it_cont = _structure_map.find (cont);

    switch (c->get_process_type ()) {
    case GOBJ_T:
        if (it_cont != _structure_map.end ())
            it_cont->second->add_gui_child (c, index);
        break;
    case WINDOW_T: {
        Window* w = dynamic_cast<Window*> (c);
        w->set_holder (it_cont->second);
        break;
    }
    case CONTAINER_T:
    case FSM_T:
    case LAYER_T: {
        GUIStructureHolder* GH = _structure_map[c];
        if (it_cont != _structure_map.end ())
            it_cont->second->add_gui_child (GH, index);
        break;
    }

    default:
        break;
    }
    cont->update_drawing ();
}

void
GUIStructureObserver::add_child_at (FatProcess* cont, CoreProcess* c, int neighbour_index, child_position_e spec, int new_index)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    switch (c->get_process_type ()) {
    case GOBJ_T:
        if (it_cont != _structure_map.end ())
            it_cont->second->add_gui_child_at (c, neighbour_index, spec, new_index);
        break;
    case WINDOW_T: {
        Window* w = dynamic_cast<Window*> (c);
        w->set_holder (it_cont->second);
        break;
    }
    case CONTAINER_T:
    case FSM_T:
    case LAYER_T: {
        GUIStructureHolder* GH = _structure_map[c];
        if (it_cont != _structure_map.end ())
            it_cont->second->add_gui_child_at (GH, neighbour_index, spec, new_index);
        break;
    }

    default:
        break;
    }
    cont->update_drawing ();
}

void
GUIStructureObserver::move_child_to (FatProcess* cont, CoreProcess* c, int neighbour_index, child_position_e spec, int new_index)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    switch (c->get_process_type ()) {
    case GOBJ_T:
        if (it_cont != _structure_map.end ())
            it_cont->second->move_child_to (c, neighbour_index, spec, new_index);
        break;
    case WINDOW_T: {
        Window* w = dynamic_cast<Window*> (c);
        w->set_holder (it_cont->second);
        break;
    }
    case CONTAINER_T:
    case FSM_T:
    case LAYER_T: {
        GUIStructureHolder* GH = _structure_map[c];
        if (it_cont != _structure_map.end ())
            it_cont->second->move_child_to (GH, neighbour_index, spec, new_index);
        break;
    }

    default:
        break;
    }
    cont->update_drawing ();
}

void
GUIStructureObserver::remove_child_from_container (FatProcess* cont, CoreProcess* c)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ()) {

        switch (c->get_process_type ()) {
        case GOBJ_T:
            it_cont->second->remove_gui_child (c);
            break;
        case WINDOW_T:
            break;
        case CONTAINER_T:
        case FSM_T:
        case LAYER_T: {
            GUIStructureHolder* GH = _structure_map[c];
            it_cont->second->remove_gui_child (GH);
            break;
        }
        default:
            break;
        }
    }
    cont->update_drawing ();
}

void
GUIStructureObserver::swap_children (FatProcess* cont, int i, int j)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
        it_cont->second->swap_children (i, j);
    cont->update_drawing ();
}

void
GUIStructureObserver::set_child (FatProcess* cont, CoreProcess* child, int i)
{
    structures_t::iterator it_cont = _structure_map.find (cont);
    if (it_cont != _structure_map.end ())
        it_cont->second->set_child (child, i);
    cont->update_drawing ();
}

GUIStructureHolder*
GUIStructureObserver::find_holder (CoreProcess* cont)
{
    structures_t_it it = _structure_map.find (cont);
    if (it != _structure_map.end ())
        return it->second;
    return nullptr;
}

GUIMouseButton::GUIMouseButton (CoreProcess* parent, const string& name)
    : FatProcess (name)
{
    _press   = new Spike (this, "press");
    _release = new Spike (this, "release");
    finalize_construction (parent, name);
}

GUIMouse::GUIMouse (CoreProcess* parent, const string& name)
    : FatProcess (name)
{
    _left   = new GUIMouseButton (this, "left");
    _right  = new GUIMouseButton (this, "right");
    _middle = new GUIMouseButton (this, "middle");
    _move   = new Spike (this, "move");
    _wheel  = new Spike (this, "wheel");
    _pos_x  = new DoubleProperty (this, "x", 0);
    _pos_y  = new DoubleProperty (this, "y", 0);
    _dx     = new DoubleProperty (nullptr, "dx", 0); // TODO: not plug !?
    _dy     = new DoubleProperty (nullptr, "dy", 0); // TODO: not plug !?
    _wheel->add_symbol ("dx", _dx);
    _wheel->add_symbol ("dy", _dy);
}

GUIMouse::~GUIMouse ()
{
    delete _left;
    delete _right;
    delete _middle;
    delete _move;
    delete _pos_x;
    delete _pos_y;
    _wheel->remove_symbol ("dx");
    _wheel->remove_symbol ("dy");
    delete _wheel;
    delete _dx;
    delete _dy;
}
} // namespace djnn
