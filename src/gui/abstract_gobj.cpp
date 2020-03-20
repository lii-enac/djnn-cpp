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

#include <iostream>

namespace djnn
{
  Process*
  AbstractGObj::create_GObj_prop (BoolPropertyProxy **prop, Coupling **cprop, bool *rawp, const std::string& name, int notify_mask)
  {
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      auto _frame = frame ();
      (*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (IntPropertyProxy **prop, Coupling **cprop, int *rawp, const std::string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      auto _frame = frame ();
      (*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (DoublePropertyProxy **prop, Coupling **cprop, double *rawp, const std::string& name, int notify_mask)
  {
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      auto _frame = frame ();
      (*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (TextPropertyProxy **prop, Coupling **cprop, std::string *rawp, const std::string& name, int notify_mask)
  {
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ()) {
      auto _frame = frame ();
      (*cprop)->enable(_frame);
    }
    else
      (*cprop)->disable ();
    return *prop;
  }

  

  AbstractGObjImpl::~AbstractGObjImpl() {}

  void
  AbstractGObj::update_frame_if_necessary ()
  {
    auto _frame = frame ();
    if (_frame == nullptr || _frame->somehow_activating ()) {
      /*  this algorithm is a little bit tricky. We want to find the closest running frame
       *  on the left side of the current object (cur_child). For this, we take its parent (curget_parent ()) and go through its
       *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
       *  then we set (cur_child) to its parent (curget_parent ()), and (curget_parent ()) is set to (curget_parent ()->parent).
       *  May be there is a place for simplification */
      bool found = false;
      Process *cur_parent = get_parent ();
      Process *cur_child = this;

      Window * frame = nullptr;

      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_cpnt_type () == COMPONENT_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_cpnt_type () == WINDOW_T && c->somehow_activating ()) {
              frame = dynamic_cast<Window*> (c);
              found = true;
            }
          }
        }
        do {
          cur_child = cur_parent;
          cur_parent = cur_parent->get_parent ();
        } while (cur_parent != nullptr && cur_parent->get_cpnt_type () != COMPONENT_T);
      }

      if (!found) {
        warning ( nullptr, " no running frame found for graphic_obj " + get_name () + "\n" );
        return;
      }

      //_frame = frame;
      //AbstractGObj::_frame = frame->get_weak_ptr ();
      AbstractGObj::frame () = frame;
    }
  }

  void
  AbstractGObj::update_drawing ()
  {
    if (_frame)
      UpdateDrawing::instance()->add_window_for_refresh (_frame);
  }

  void
  AbstractGObj::impl_activate ()
  {
    //std::cerr << __FILE__ << __LINE__ << std::endl;
    update_frame_if_necessary ();
    auto _frame = frame ();
    UpdateDrawing::instance ()->add_window_for_refresh (_frame);
    UpdateDrawing::instance ()->get_damaged ()->notify_activation ();
    Backend::instance()->activate_gobj(this);
  }

  void
  AbstractGObj::impl_deactivate ()
  {
    auto _frame = frame ();
    if (_frame != nullptr) {
      UpdateDrawing::instance ()->add_window_for_refresh (_frame);
      UpdateDrawing::instance ()->get_damaged ()->notify_activation ();
      UpdateDrawing::instance ()->set_activation_flag (ACTIVATION);
      //UpdateDrawing::instance ()->request_activation ();
      Backend::instance()->deactivate_gobj(this);
      _frame = nullptr;
    }
  }
}
