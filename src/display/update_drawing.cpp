#include "update_drawing.h"

#include "core/control/action.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/execution/graph.h"
#include "core/property/bool_property.h"
#include "core/utils/algorithm.h"
#include "core/utils/iostream.h"
#include "utils/debug.h"

namespace djnn {

bool           update_display_initialized = false;
UpdateDrawing* UpdateDrawing::_instance;

UpdateDrawing*
UpdateDrawing::instance ()
{
    if (update_display_initialized == false) {
        _instance = new UpdateDrawing ();
        //_instance->update_auto_refresh ();
        _instance->impl_activate ();
    }

    return _instance;
}

void
UpdateDrawing::init ()
{
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

    _instance->_damaged                    = new UndelayedSpike (_instance, "damaged");                // UndelayedSpike _damaged
    _instance->_auto_refresh               = new BoolProperty (_instance, "auto_refresh", true);       // Bool _auto_refresh (true)
    _instance->_update_auto_refresh_action = new AutoRefreshAction (_instance, "auto_refresh_action"); // Action _update_auto_refresh_action (_c_redraw_when_damaged.en-dis-able)
    _instance->_draw_sync                  = new Spike (_instance, "draw_sync");                       // Spike _draw_sync
    _instance->_redraw_action              = new RedrawAction (_instance, "redraw_action");            // Action _redraw_action
    Graph::instance ().add_output_node (_instance->_redraw_action);

    _instance->_c_redraw_when_damaged   = new Coupling (_instance->_damaged, ACTIVATION, _instance->_draw_sync, ACTIVATION);       // _damaged -> _draw_sync
    _instance->_c_redraw_when_draw_sync = new Coupling (_instance->_draw_sync, ACTIVATION, _instance->_redraw_action, ACTIVATION); // _draw_sync -> _redraw_action
    graph_remove_edge (_instance->_draw_sync, _instance->_redraw_action);                                                          // remove the edge added by the coupling
    // no need to add_edge from _draw_sync to _redraw_action since _redraw_action is an output

    _instance->_c_update_auto_refresh = new Coupling (_instance->_auto_refresh, ACTIVATION, _instance->_update_auto_refresh_action, ACTIVATION); // _auto_refresh -> _update_auto_refresh_action

    _instance->set_activation_state (ACTIVATED);

    update_display_initialized = true;
}

void
UpdateDrawing::clear ()
{
    if (update_display_initialized) {
        _instance->set_activation_state (DEACTIVATED);
        _instance->set_data (nullptr);
        _instance->_win_list.clear ();

        delete _instance->_c_update_auto_refresh;

        graph_add_edge (_instance->_draw_sync, _instance->_redraw_action); // this edge will be removed by the coupling's destructor but it doesn't exist anymore so add it now
        delete _instance->_c_redraw_when_draw_sync;
        delete _instance->_c_redraw_when_damaged;
        Graph::instance ().remove_output_node (_instance->_redraw_action);
        delete _instance->_redraw_action;

        delete _instance->_draw_sync;
        delete _instance->_update_auto_refresh_action;
        delete _instance->_auto_refresh;
        delete _instance->_damaged;

        update_display_initialized = false;
    }
}

UpdateDrawing::UpdateDrawing ()
    : FatProcess ("UpdateDrawing")
{
    // we have to "finalise" it with nullptr to correctly added into Parentless_names
    finalize_construction (nullptr, "UpdateDrawing");
}

UpdateDrawing::~UpdateDrawing ()
{
    // NOTE: 07.2020: moved everything to clear ()
    UpdateDrawing::clear ();
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
    // dynamic_cast<UpdateDrawing*>(get_parent())->get_damaged()->set_data(nullptr);
}

void
UpdateDrawing::UndelayedSpike::impl_activate ()
{ // DBG;
    Window* frame = dynamic_cast<Window*> (get_data ());
    // std::cerr << frame << __FL__;
    if (frame && !frame->refresh ()) {
        _ud->add_window_for_refresh (frame);
        set_data (nullptr);
    }
    notify_activation ();
}

void
UpdateDrawing::update_auto_refresh ()
{
    if (_auto_refresh->get_value ()) {
        _c_redraw_when_damaged->enable ();
    } else {
        _c_redraw_when_damaged->disable ();
    }
}

void
UpdateDrawing::impl_activate ()
{ // DBG;
  // update_auto_refresh ();
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
    _win_list.erase (djnnstl::remove (_win_list.begin (), _win_list.end (), w), _win_list.end ());
    w->set_refresh (false);
}
} // namespace djnn
