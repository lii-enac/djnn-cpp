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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "abstract_gobj.h"

#include "abstract_backend.h"
#include "backend.h"
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "display/update_drawing.h"
#include "display/window.h"
#include "layer.h"

namespace djnn {

void
AbstractGObj::create_Gobj_update_coupling (CoreProcess** prop, CouplingWithData** cprop)
{
    FatProcess* update = nullptr;
    if (find_layer ())
        update = find_layer ()->damaged ();
    else if (get_frame ())
        update = get_frame ()->damaged ();
    *cprop = new CouplingWithData (*prop, ACTIVATION, update, ACTIVATION);
    if (somehow_activating ()) {
        (*cprop)->enable ();
    } else
        (*cprop)->disable ();
}

FatProcess*
AbstractGObj::create_GObj_prop (BoolPropertyProxy** prop, CouplingWithData** cprop, bool* rawp, const string& name, int notify_mask)
{
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling (reinterpret_cast<CoreProcess**> (prop), cprop);
    return *prop;
}

FatProcess*
AbstractGObj::create_GObj_prop (IntPropertyProxy** prop, CouplingWithData** cprop, int* rawp, const string& name, int notify_mask)
{
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling (reinterpret_cast<CoreProcess**> (prop), cprop);
    return *prop;
}

FatProcess*
AbstractGObj::create_GObj_prop (DoublePropertyProxy** prop, CouplingWithData** cprop, double* rawp, const string& name, int notify_mask)
{
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling (reinterpret_cast<CoreProcess**> (prop), cprop);
    return *prop;
}

FatProcess*
AbstractGObj::create_GObj_prop (TextPropertyProxy** prop, CouplingWithData** cprop, string* rawp, const string& name, int notify_mask)
{
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling (reinterpret_cast<CoreProcess**> (prop), cprop);
    return *prop;
}

FatProcess*
AbstractGObj::create_GObj_prop (TextPropertyProxy** prop, CouplingWithData** cprop, string** rawp, const string& name, int notify_mask)
{
    *prop = new TextPropertyProxy (this, name, **rawp, notify_mask);
    create_Gobj_update_coupling (reinterpret_cast<CoreProcess**> (prop), cprop);
    return *prop;
}

void
AbstractGObj::impl_clone_properties (CoreProcess* clone, map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    for (auto& pname : get_properties_name ()) {
        auto* p = const_cast<AbstractGObj*> (this)->AbstractGObj::find_child_impl (pname);
        if (p) {
            origs_clones[p] = clone->find_child_impl (pname);
        }
    }
}

AbstractGObjImpl::~AbstractGObjImpl () {}

Window*
find_frame (CoreProcess* obj)
{
    /*  this algorithm is a little bit tricky. We want to find the closest running frame
     *  on the left side of the current object (cur_child). For this, we take its parent (cur_parent ()) and go through its
     *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
     *  then we set (cur_child) to its parent (cur_parent ()), and (cur_parent ()) is set to (cur_parent ()->parent).
     *  May be there is a place for simplification */
    bool         found      = false;
    FatProcess*  cur_parent = obj->get_parent ();
    CoreProcess* cur_child  = obj;

    Window* frame = nullptr;

    while (!found && cur_parent != nullptr) {
        if (cur_parent->get_process_type () == CONTAINER_T) {
            Container* cont = dynamic_cast<Container*> (cur_parent);
            for (auto c : cont->children ()) {
                if (c == cur_child)
                    break;
                else if (c->get_process_type () == WINDOW_T && c->somehow_activating ()) {
                    frame = dynamic_cast<Window*> (c);
                    found = true;
                }
            }
        }
        do {
            cur_child  = cur_parent;
            cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
    }

    if (!found) {
        return nullptr;
    }

    return frame;
}

Window*
find_frame_debug (CoreProcess* obj)
{
    /*  this algorithm is a little bit tricky. We want to find the closest running frame
     *  on the left side of the current object (cur_child). For this, we take its parent (cur_parent ()) and go through its
     *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
     *  then we set (cur_child) to its parent (cur_parent ()), and (cur_parent ()) is set to (cur_parent ()->parent).
     *  May be there is a place for simplification */
    bool         found      = false;
    FatProcess*  cur_parent = obj->get_parent ();
    CoreProcess* cur_child  = obj;

    Window* frame = nullptr;

    while (!found && cur_parent != nullptr) {
        std::cerr << "find_frame_debug: " << cur_parent->get_debug_name() << std::endl;
        if (cur_parent->get_process_type () == CONTAINER_T) {
            Container* cont = dynamic_cast<Container*> (cur_parent);
            for (auto c : cont->children ()) {
                if (c == cur_child)
                    break;
                else if (c->get_process_type () == WINDOW_T && c->somehow_activating ()) {
                    frame = dynamic_cast<Window*> (c);
                    found = true;
                }
            }
        }
        do {
            cur_child  = cur_parent;
            cur_parent = cur_parent->get_parent ();
            if (cur_parent)
                std::cerr << "find_frame_debug (do): " << cur_parent->get_debug_name() << std::endl;
            else
                std::cerr << "find_frame_debug (do): cur_parent is NULL !" << std::endl;
        } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
    }

    if (!found) {
        return nullptr;
    }

    return frame;
}

void
AbstractGObj::update_frame_if_necessary ()
{
    auto _frame = get_frame ();
    if (_frame == nullptr || _frame->somehow_activating ()) {
        Window* frame = find_frame (this);

        if (!frame) {
            find_frame_debug (this);
            warning (nullptr, " no running frame found for graphic_obj " + get_name () + "\n");
            return;
        }

        AbstractGObj::set_frame (frame);
    }
}

void
AbstractGObj::update_drawing ()
{
    if (find_layer ()) {
        find_layer ()->damaged ()->activate ();
    } else if (_frame)
        _frame->damaged ()->activate ();
}

void
AbstractGObj::impl_activate ()
{
    update_frame_if_necessary ();
    _frame = get_frame ();
    if (find_layer ()) {
        find_layer ()->set_invalid_cache (true);
    } else if (_frame) {
        UpdateDrawing::instance ()->add_window_for_refresh (_frame);
        UpdateDrawing::instance ()->get_redraw ()->activate ();
        //_frame->damaged ()->activate ();
    }
    Backend::instance ()->activate_gobj (this);
}

void
AbstractGObj::impl_deactivate ()
{
    auto _frame = get_frame ();
    if (find_layer ()) {
        find_layer ()->set_invalid_cache (true);
    } else if (_frame) {
        UpdateDrawing::instance ()->add_window_for_refresh (_frame);
        UpdateDrawing::instance ()->get_redraw ()->activate ();
        //_frame->damaged ()->activate ();
    }
    Backend::instance ()->deactivate_gobj (this);
    _frame = nullptr;
}

Layer*
AbstractGObj::find_layer ()
{
    FatProcess* p = get_parent ();
    while (p != nullptr) {
        if (p->get_process_type () == LAYER_T) {
            return dynamic_cast<Layer*> (p);
        } else {
            p = p->get_parent ();
        }
    }
    return nullptr;
}
} // namespace djnn
