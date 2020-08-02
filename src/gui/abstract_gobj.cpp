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
#include "backend.h"
#include "abstract_backend.h"

#include "display/window.h"
#include "display/update_drawing.h"

#include "core/execution/graph.h"

#include "core/utils/error.h"

#include <iostream>

namespace djnn
{

  void
  AbstractGObj::create_Gobj_update_coupling (CoreProcess **prop, CouplingWithData **cprop)
  {
    FatProcess *update = get_frame ();
    if (update) update = get_frame ()->damaged ();
    *cprop = new CouplingWithData (*prop, ACTIVATION, update, ACTIVATION);
    if (somehow_activating ()) {
      (*cprop)->enable ();
      Graph::instance ().add_edge (*prop, update);
    }
    else
      (*cprop)->disable ();
  }

  FatProcess*
  AbstractGObj::create_GObj_prop (BoolPropertyProxy **prop, CouplingWithData **cprop, bool *rawp, const std::string& name, int notify_mask)
  {
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
  }

  FatProcess*
  AbstractGObj::create_GObj_prop (IntPropertyProxy **prop, CouplingWithData **cprop, int *rawp, const std::string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
  }

  FatProcess*
  AbstractGObj::create_GObj_prop (DoublePropertyProxy **prop, CouplingWithData **cprop, double *rawp, const std::string& name, int notify_mask)
  {
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
  }

  FatProcess*
  AbstractGObj::create_GObj_prop (TextPropertyProxy **prop, CouplingWithData **cprop, std::string *rawp, const std::string& name, int notify_mask)
  {
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    create_Gobj_update_coupling(reinterpret_cast<CoreProcess**>(prop), cprop);
    return *prop;
  }

  void
  enable (Coupling* c, CoreProcess* dst)
  {
    if (c) {
      if (c->get_dst () == nullptr) {
        c->set_dst (dst);
        Graph::instance ().add_edge (c->get_src (), c->get_dst ());
      }
      c->enable();
    }
  }

  void
  remove_edge (Coupling *c)
  {
    if (c) {
      if (c->get_src () && c->get_dst ()) Graph::instance ().remove_edge (c->get_src (), c->get_dst ());
    }
  }

  void
  disable (Coupling *c)
  {
    if (c) {
      c->disable ();
    }
  }


  AbstractGObjImpl::~AbstractGObjImpl () {}

  void
  AbstractGObj::update_frame_if_necessary ()
  {
    auto _frame = get_frame ();
    if (_frame == nullptr || _frame->somehow_activating ()) {
      /*  this algorithm is a little bit tricky. We want to find the closest running frame
       *  on the left side of the current object (cur_child). For this, we take its parent (curget_parent ()) and go through its
       *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
       *  then we set (cur_child) to its parent (curget_parent ()), and (curget_parent ()) is set to (curget_parent ()->parent).
       *  May be there is a place for simplification */
      bool found = false;
      FatProcess *cur_parent = get_parent ();
      FatProcess *cur_child = this;

      Window * frame = nullptr;

      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_process_type () == CONTAINER_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
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
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_process_type () != CONTAINER_T);
      }

      if (!found) {
        warning ( nullptr, " no running frame found for graphic_obj " + get_name () + "\n" );
        return;
      }

      AbstractGObj::set_frame (frame);
    }
  }

  void
  AbstractGObj::update_drawing ()
  {
    if (_frame)
      _frame->damaged ()->notify_activation ();
  }

  void
  AbstractGObj::impl_activate ()
  {
    update_frame_if_necessary ();
    auto _frame = get_frame ();
    _frame->damaged ()->activate ();
    Backend::instance ()->activate_gobj (this);
  }

  void
  AbstractGObj::impl_deactivate ()
  {
    auto _frame = get_frame ();
    if (_frame != nullptr) {
      _frame->damaged ()->activate ();
      Backend::instance ()->deactivate_gobj (this);
      _frame = nullptr;
    }
  }
}
