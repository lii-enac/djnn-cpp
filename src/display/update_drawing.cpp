#include "update_drawing.h"

#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/execution/graph.h"
#include "core/tree/bool_property.h"

#include <algorithm>

#include <iostream>
#include "utils/debug.h"

namespace djnn {

  bool update_display_initialized = false;
  UpdateDrawing *UpdateDrawing::_instance;
  std::once_flag UpdateDrawing::onceFlag;

  UpdateDrawing*
  UpdateDrawing::instance ()
  {
    std::call_once (UpdateDrawing::onceFlag, [] () {
      _instance = new UpdateDrawing ();
      //_instance->update_auto_refresh ();
      _instance->impl_activate ();
      //update_display_initialized = true;
    });

    return _instance;
  }

  void
  UpdateDrawing::init ()
  {
    //Graph::instance ()->add_output_node (instance ());
    //update_display_initialized = true;
    instance ();

    // in smala that would be:

    // UndelayedSpike _damaged
    // Bool _auto_refresh (true) // true by default, when set will enable or disable the coupling between damaged and draw_sync to offer client-provided, specific redraw policy
    // Action _update_auto_refresh_action (_c_redraw_when_damaged.en/disable) // 
    // Spike _draw_sync      // external API to allow clients to directly trigger draw
    // Action _redraw_action // actual redraw action, output at the end of the activation vector

    // here '->' denotes a coupling, not a binding...
    // _damaged -> _draw_sync
    // _draw_sync -> _redraw_action
    // _auto_refresh -> _update_auto_refresh_action

    _instance->_damaged = new UndelayedSpike (_instance, "damaged"); // UndelayedSpike _damaged
    _instance->_auto_refresh = new BoolProperty (_instance, "auto_refresh", true); // Bool _auto_refresh (true)
    _instance->_update_auto_refresh_action = new AutoRefreshAction (_instance, "auto_refresh_action"); // Action _update_auto_refresh_action (_c_redraw_when_damaged.en-dis-able)
    _instance->_draw_sync = new Spike (_instance, "draw_sync"); // Spike _draw_sync
    _instance->_redraw_action = new RedrawAction (_instance, "redraw_action"); // Action _redraw_action
    Graph::instance ().add_output_node (_instance->_redraw_action);
  
    _instance->_c_redraw_when_damaged = new Coupling (_instance->_damaged, ACTIVATION, _instance->_draw_sync, ACTIVATION); // _damaged -> _draw_sync
    graph_add_edge (_instance->_damaged, _instance->_draw_sync);
    _instance->_c_redraw_when_draw_sync = new Coupling (_instance->_draw_sync, ACTIVATION, _instance->_redraw_action, ACTIVATION); // _draw_sync -> _redraw_action
    // no need to add_edge from _draw_sync to _redraw_action since _redraw_action is an output
  
    _instance->_c_update_auto_refresh = new Coupling (_instance->_auto_refresh, ACTIVATION, _instance->_update_auto_refresh_action, ACTIVATION); // _auto_refresh -> _update_auto_refresh_action
    graph_add_edge (_instance->_auto_refresh, _instance->_update_auto_refresh_action);

    _instance->set_activation_state (ACTIVATED);

    update_display_initialized = true;
  }

  void
  UpdateDrawing::clear ()
  {
    if (update_display_initialized) {
      _instance->set_data (nullptr);
      _instance->_win_list.clear ();
      graph_remove_edge (_instance->_auto_refresh, _instance->_update_auto_refresh_action);
      graph_remove_edge (_instance->_damaged, _instance->_draw_sync);
      Graph::instance ().remove_output_node (_instance->_redraw_action);
      
      delete _instance->_c_redraw_when_draw_sync;
      delete _instance->_c_redraw_when_damaged;
      delete _instance->_redraw_action;
      delete _instance->_c_update_auto_refresh;

      delete _instance->_damaged;
      delete _instance->_draw_sync;
      delete _instance->_auto_refresh;
      delete _instance->_update_auto_refresh_action;
      update_display_initialized = false;
    }
  }

  UpdateDrawing::UpdateDrawing () :
    FatProcess ("UpdateDrawing")
  {
    // NOTE: 07.2020 : move everything to init ()

    // // in smala that would be:

    // // UndelayedSpike _damaged
    // // Bool _auto_refresh (true) // true by default, when set will enable or disable the coupling between damaged and draw_sync to offer client-provided, specific redraw policy
    // // Action _update_auto_refresh_action (_c_redraw_when_damaged.en/disable) // 
    // // Spike _draw_sync      // external API to allow clients to directly trigger draw
    // // Action _redraw_action // actual redraw action, output at the end of the activation vector

    // // here '->' denotes a coupling, not a binding...
    // // _damaged -> _draw_sync
    // // _draw_sync -> _redraw_action
    // // _auto_refresh -> _update_auto_refresh_action

    // _damaged = new UndelayedSpike (this, "damaged"); // UndelayedSpike _damaged
    // _auto_refresh = new BoolProperty (this, "auto_refresh", true); // Bool _auto_refresh (true)
    // _update_auto_refresh_action = new AutoRefreshAction (this, "auto_refresh_action"); // Action _update_auto_refresh_action (_c_redraw_when_damaged.en-dis-able)
    // _draw_sync = new Spike (this, "draw_sync"); // Spike _draw_sync
    // _redraw_action = new RedrawAction (this, "redraw_action"); // Action _redraw_action
    // Graph::instance ().add_output_node (_redraw_action);
  
    // _c_redraw_when_damaged = new Coupling (_damaged, ACTIVATION, _draw_sync, ACTIVATION); // _damaged -> _draw_sync
    // graph_add_edge (_damaged, _draw_sync);
    // _c_redraw_when_draw_sync = new Coupling (_draw_sync, ACTIVATION, _redraw_action, ACTIVATION); // _draw_sync -> _redraw_action
    // // no need to add_edge from _draw_sync to _redraw_action since _redraw_action is an output
  
    // _c_update_auto_refresh = new Coupling (_auto_refresh, ACTIVATION, _update_auto_refresh_action, ACTIVATION); // _auto_refresh -> _update_auto_refresh_action
    // graph_add_edge (_auto_refresh, _update_auto_refresh_action);

    // set_activation_state (ACTIVATED);

    // //finalize_construction (nullptr, "UpdateDrawing");
  }

  UpdateDrawing::~UpdateDrawing ()
  { 
   UpdateDrawing::clear ();

   //NOTE: 07.2020 : move everything to clear ()

   // graph_remove_edge (_damaged, _draw_sync);
   // graph_remove_edge (_auto_refresh, _update_auto_refresh_action);
   // Graph::instance ().remove_output_node (_redraw_action);

   // delete _c_redraw_when_draw_sync;
   // delete _c_redraw_when_damaged;
   // delete _redraw_action;
   // delete _c_update_auto_refresh;

   // delete _damaged;
   // delete _draw_sync;
   // delete _auto_refresh;
   // delete _update_auto_refresh_action;
  }

  

  void 
  UpdateDrawing::RedrawAction::impl_activate () 
  {
    for (auto& w : _ud->get_windows_for_refresh ()) {
      if (w != nullptr) {
        w->update ();
        w->set_refresh (false);
      }
    }
    _ud->clear_windows_for_refresh ();
    //dynamic_cast<UpdateDrawing*>(get_parent())->get_damaged()->set_data(nullptr);
  }

  void 
  UpdateDrawing::UndelayedSpike::impl_activate () 
  { //DBG;
    Window *frame = dynamic_cast<Window*> (get_data ());
    //std::cerr << frame << __FL__;
    if (frame && !frame->refresh ()) {
      _ud->add_window_for_refresh (frame);
      set_data (nullptr);
    }
    notify_activation ();
  }

  void
  UpdateDrawing::update_auto_refresh ()
  {
    if (_auto_refresh->get_value()) {
      _c_redraw_when_damaged->enable ();
    }
    else {
      _c_redraw_when_damaged->disable ();
    }
  }

  void
  UpdateDrawing::impl_activate ()
  { //DBG;
    //update_auto_refresh ();
  }

  // useless ?? there is no binding or coupling or whatsoever directly on UpdateDrawing ?!
  // void
  // UpdateDrawing::coupling_activation_hook ()
  // { DBG;
  //   Window *frame = dynamic_cast<Window*> (get_data ());
  //   if (frame == nullptr)
  //     return;
  //   if (!frame->refresh ()) {
  //     _win_list.push_back (frame);
  //     frame->set_refresh (true);
  //   }
  // }

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
}
