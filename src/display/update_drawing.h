#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/bool_property.h"
#include "core/tree/spike.h"
#include "window.h"

#include <mutex> // for once_flag

//#include <iostream>
//#include "utils/debug.h"

namespace djnn {

  class UpdateDrawing : public Process
  {
  private:
    class RedrawAction : public Action {
      public:
        RedrawAction (UpdateDrawing *parent, const string &name) : Action (parent, name), _ud (parent) {}
        virtual ~RedrawAction () {}
        void impl_activate () override ;
        void impl_deactivate () override {}
      private:
        UpdateDrawing* _ud;
    };
    class UndelayedSpike : public Process
    {
      public:
        UndelayedSpike (UpdateDrawing *parent, const string &name)  : Process (name), _ud (parent) { set_is_model (true); Process::finalize_construction (parent, name); }
        virtual ~UndelayedSpike () {}
        void post_activate () override { set_activation_state (DEACTIVATED); }
        void impl_activate () override {};
        void impl_deactivate () override {};
        void coupling_activation_hook () override;
      private:
        UpdateDrawing* _ud;
    };
    class AutoRefreshAction : public Action
    {
      public:
        AutoRefreshAction (UpdateDrawing *parent, const string &name) : Action (parent, name), _ud (parent) {}
        virtual ~AutoRefreshAction () {}
        void impl_activate () override { _ud->update_auto_refresh (); };
        void impl_deactivate () override {}
      private:
        UpdateDrawing *_ud;
      };
  public:
    static UpdateDrawing* instance ();
    ~UpdateDrawing ();
    void impl_activate () override;
    void impl_deactivate () override {}
    void coupling_activation_hook () override;
    void add_window_for_refresh (Window* w);
    void remove_window_for_refresh (Window* w);
    void update_auto_refresh ();
    std::vector<Window*>& get_win_list () { return _win_list; };
    Process* get_damaged () { return _damaged; }
    void clear_list () {_win_list.clear (); }
    static void init ();
    static void clear ();
  private:
    UpdateDrawing (const UpdateDrawing&) = delete;
    UpdateDrawing & operator=(const UpdateDrawing&) = delete;
    static UpdateDrawing* _instance;
    static std::once_flag onceFlag;
    std::vector<Window*> _win_list;
    BoolProperty *_auto_refresh;
    Spike *_draw_sync;
    UndelayedSpike *_damaged;
    RedrawAction *_redraw_action;
    AutoRefreshAction *_update_auto_refresh_action;
    Coupling *_c_redraw_when_damaged, *_c_redraw_when_draw_sync, *_c_update_auto_refresh;
    UpdateDrawing ();
  };
}
