
/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/property/bool_property.h"
#include "core/control/spike.h"
#include "window.h"

#include <mutex> // std::call_once

//#include "core/utils/iostream.h"
//#include "utils/debug.h"

namespace djnn {

  class UpdateDrawing : public FatProcess
  {
  private:
    class RedrawAction : public Action {
      public:
        RedrawAction (UpdateDrawing * parent, const string& name) : Action (parent, name), _ud (parent) { finalize_construction (parent, name); }
        virtual ~RedrawAction () {}
        void impl_activate () override ;
      private:
        UpdateDrawing* _ud;
    };
    class UndelayedSpike : public FatProcess
    {
      public:
        UndelayedSpike (UpdateDrawing * parent, const string& name)  : FatProcess (name), _ud (parent) { set_is_model (true); finalize_construction (parent, name); }
        virtual ~UndelayedSpike () {}
        void post_activate () override { post_activate_auto_deactivate (); }
        void impl_activate () override;
        void impl_deactivate () override {};
      private:
        UpdateDrawing* _ud;
    };
    class AutoRefreshAction : public Action
    {
      public:
        AutoRefreshAction (UpdateDrawing * parent, const string& name) : Action (parent, name), _ud (parent) {}
        virtual ~AutoRefreshAction () {}
        void impl_activate () override { _ud->update_auto_refresh (); };
      private:
        UpdateDrawing *_ud;
      };
  public:
    static UpdateDrawing* instance ();
    static void init ();
    static void clear ();
    
    ~UpdateDrawing ();
    void impl_activate () override;
    void impl_deactivate () override {}
    //void coupling_activation_hook () override;
    void update_auto_refresh ();
    void add_window_for_refresh (Window* w);
    void remove_window_for_refresh (Window* w);
    vector<Window*>& get_windows_for_refresh () { return _win_list; };
    void clear_windows_for_refresh () {_win_list.clear (); }
    FatProcess* get_damaged () { return _damaged; }
    FatProcess* get_redraw () { return _redraw_action; }
  private:
    UpdateDrawing (const UpdateDrawing&) = delete;
    UpdateDrawing & operator=(const UpdateDrawing&) = delete;
    static UpdateDrawing* _instance;
    vector<Window*> _win_list;
    BoolProperty *_auto_refresh;
    Spike *_draw_sync;
    UndelayedSpike *_damaged;
    RedrawAction *_redraw_action;
    AutoRefreshAction *_update_auto_refresh_action;
    Coupling *_c_redraw_when_damaged, *_c_redraw_when_draw_sync, *_c_update_auto_refresh;
    UpdateDrawing ();
  };
}
