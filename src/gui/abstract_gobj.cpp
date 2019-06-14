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

#include "../display/window.h"
#include "abstract_gobj.h"

#include <iostream>

namespace djnn
{
  Process*
  AbstractGObj::create_GObj_prop (BoolPropertyProxy **prop, Coupling **cprop, bool *rawp, const string& name, int notify_mask)
  {
    *prop = new BoolPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ())
      (*cprop)->enable(_frame);
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (IntPropertyProxy **prop, Coupling **cprop, int *rawp, const string& name, int notify_mask)
  {
    *prop = new IntPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ())
      (*cprop)->enable(_frame);
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (DoublePropertyProxy **prop, Coupling **cprop, double *rawp, const string& name, int notify_mask)
  {
    *prop = new DoublePropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ())
      (*cprop)->enable(_frame);
    else
      (*cprop)->disable ();
    return *prop;
  }

  Process*
  AbstractGObj::create_GObj_prop (TextPropertyProxy **prop, Coupling **cprop, string *rawp, const string& name, int notify_mask)
  {
    *prop = new TextPropertyProxy (this, name, *rawp, notify_mask);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    *cprop = new Coupling (*prop, ACTIVATION, update, ACTIVATION);
    if (this->somehow_activating ())
      (*cprop)->enable(_frame);
    else
      (*cprop)->disable ();
    return *prop;
  }

  void 
  UpdateDrawing::RedrawAction::activate () 
  {
    for (auto& w : _ud->get_win_list ()) {
      if (w != nullptr) {
        w->update ();
        w->set_refresh (false);
      }
    }
    _ud->clear_list ();
  }

  void 
  UpdateDrawing::UndelayedSpike::coupling_activation_hook () 
  {
    Window *frame = dynamic_cast<Window*> (get_data ());
    if (frame && !frame->refresh ()) {
      _ud->add_window_for_refresh (frame);
    }
    notify_activation (); 
  }

  bool gui_initialized = false;
  UpdateDrawing *UpdateDrawing::_instance;
  std::once_flag UpdateDrawing::onceFlag;

  UpdateDrawing::UpdateDrawing ()
  {
    _auto_refresh = new BoolProperty (this, "auto_refresh", true);
    _draw_sync = new Spike (this, "draw_sync");
    _damaged = new UndelayedSpike (this, "damaged");
    _redraw_action = new RedrawAction (this, "redraw_action");
    _update_auto_refresh_action = new AutoRefreshAction (this, "auto_refresh_action");
    Graph::instance ().add_output_node (_redraw_action);
    _redraw_when_damaged = new Coupling (_damaged, ACTIVATION, _draw_sync, ACTIVATION);
    Graph::instance ().add_edge (_damaged, _draw_sync);
    _redraw_when_draw_sync = new Coupling (_draw_sync, ACTIVATION, _redraw_action, ACTIVATION);
    _c_update_auto_refresh = new Coupling (_auto_refresh, ACTIVATION, _update_auto_refresh_action, ACTIVATION);
    Graph::instance ().add_edge (_auto_refresh, _update_auto_refresh_action);
    set_activated ();
  }

  UpdateDrawing::~UpdateDrawing ()
  {
    Graph::instance ().remove_edge (_damaged, _draw_sync);
    Graph::instance ().remove_edge (_auto_refresh, _update_auto_refresh_action);
    Graph::instance ().remove_output_node (_redraw_action);
    
    delete _redraw_when_draw_sync;
    delete _redraw_when_damaged;
    delete _redraw_action;
    delete _c_update_auto_refresh;

    delete _damaged;
    delete _draw_sync;
    delete _auto_refresh;
    delete _update_auto_refresh_action;
  }

  UpdateDrawing*
  UpdateDrawing::instance ()
  {
    std::call_once (UpdateDrawing::onceFlag, [] () {
      _instance = new UpdateDrawing ();
    });

    return _instance;
  }

  void
  UpdateDrawing::activate ()
  {
    if (_auto_refresh->get_value ())
      _redraw_when_damaged->enable ();
    else
      _redraw_when_damaged->disable ();
  }

  void
  UpdateDrawing::coupling_activation_hook ()
  {
    Window *frame = dynamic_cast<Window*> (get_data ());
    if (frame == nullptr)
      return;
    if (!frame->refresh ()) {
      _win_list.push_back (frame);
      frame->set_refresh (true);
    }
  }

  void 
  UpdateDrawing::add_window_for_refresh (Window* w) 
  {
    _win_list.push_back (w); w->set_refresh (true); 
  }

  void
  UpdateDrawing::init ()
  {
    //Graph::instance ().add_output_node (instance ());
    gui_initialized = true;
  }

  void
  UpdateDrawing::clear ()
  {
    if (gui_initialized) {
      _instance->set_data (nullptr);
      _instance->_win_list.clear ();
      gui_initialized = false;
    }
  }

  void
  AbstractGObj::activate ()
  {
    if (_frame == nullptr || _frame->somehow_activating ()) {
      /*  this algorithm is a little bit tricky. We want to find the closest running frame
       *  on the left side of the current object (cur_child). For this, we take its parent (cur_parent) and go through its
       *  children in order to find a frame. If no frame is found when the list iteration process arrived to (cur_child),
       *  then we set (cur_child) to its parent (cur_parent), and (cur_parent) is set to (cur_parent->parent).
       *  May be there is a place for simplification */
      bool found = false;
      Process *cur_parent = _parent;
      Process *cur_child = this;
      while (!found && cur_parent != nullptr) {
        if (cur_parent->get_cpnt_type () == COMPONENT_T) {
          Container *cont = dynamic_cast<Container*> (cur_parent);
          for (auto c : cont->children ()) {
            if (c == cur_child)
              break;
            else if (c->get_cpnt_type () == WINDOW_T && c->somehow_activating ()) {
              _frame = dynamic_cast<Window*> (c);
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
        std::cout << "Warning no running frame found\n";
        return;
      }
    }
    UpdateDrawing::instance ()->add_window_for_refresh (_frame);
    UpdateDrawing::instance ()->get_damaged ()->notify_activation ();
  }

  void
  AbstractGObj::deactivate ()
  {
    if (_frame != nullptr) {
      UpdateDrawing::instance ()->add_window_for_refresh (_frame);
      UpdateDrawing::instance ()->get_damaged ()->notify_activation ();
    }
  }
}
