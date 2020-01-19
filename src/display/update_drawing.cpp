#include "update_drawing.h"

#include "core/control/action.h"
#include "core/execution/graph.h"
#include "core/tree/bool_property.h"

#include <algorithm>

//#include <iostream>
//#include "utils/debug.h"

namespace djnn {

  void 
  UpdateDrawing::RedrawAction::impl_activate () 
  { //DBG;
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
  { //DBG;
    Window *frame = dynamic_cast<Window*> (get_data ());
    if (frame && !frame->refresh ()) {
      _ud->add_window_for_refresh (frame);
    }
    notify_activation (); 
  }

  bool gui_initialized = false;
  UpdateDrawing *UpdateDrawing::_instance;
  std::once_flag UpdateDrawing::onceFlag;

  UpdateDrawing::UpdateDrawing () :
    Process ("UpdateDrawing")
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
    set_activation_state (ACTIVATED);
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
  UpdateDrawing::impl_activate ()
  { //DBG;
    if (_auto_refresh->get_value ())
      _redraw_when_damaged->enable ();
    else
      _redraw_when_damaged->disable ();
  }

  void
  UpdateDrawing::coupling_activation_hook ()
  { //DBG;
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
    if (w == nullptr)
      return;
    
    _win_list.push_back (w);
    w->set_refresh (true); 
  }

   void 
  UpdateDrawing::remove_window_for_refresh (Window* w) 
  {
    _win_list.erase (std::remove (_win_list.begin (), _win_list.end (), w), _win_list.end ());
    w->set_refresh (false);
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
}