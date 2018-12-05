/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "../core/tree/bool_property.h"
#include "../core/tree/double_property.h"
#include "../core/tree/process.h"
#include "../core/tree/component.h"
#include "../core/execution/component_observer.h"
#include "../core/control/coupling.h"
#include "window.h"
#include "../core/error.h"
#include <iostream>

namespace djnn
{

  extern bool gui_initialized;

  class AbstractGObj : public Process
  {
  public:
    AbstractGObj () : Process(), _frame (nullptr) {
      if (!gui_initialized) warning (this, "Module GUI not initialized");
      _cpnt_type = GOBJ;
    }
    AbstractGObj (Process *p, const std::string& n) : Process (p, n), _frame (nullptr) {
      if (!gui_initialized)  warning (this, "Module GUI not initialized");
      _cpnt_type = GOBJ;
    }
    virtual ~AbstractGObj () {};
    Window*& frame () { return _frame; }
    void activate () override;
    void deactivate () override;
  protected:
    Window *_frame;
  };


  class UpdateDrawing : public Process
  {
  private:
    class RedrawAction : public Process {
      public:
        RedrawAction (UpdateDrawing *p, const string &n) : Process (p, n), _ud (p) { Process::finalize (); }
        virtual ~RedrawAction () {}
        void activate () {
          for (auto w : _ud->get_win_list ()) {
            if (w != nullptr) {
              w->update ();
              w->set_refresh (false);
            }
          }
          _ud->clear_list ();
        }
        void deactivate () {}
      private:
        UpdateDrawing* _ud;
    };
    class UndelayedSpike : public Process
    {
    public:
      UndelayedSpike (UpdateDrawing *p, const string &n)  : Process (p, n), _ud (p) { _model = true; Process::finalize (); }
      virtual ~UndelayedSpike () {}
      void post_activate () override {_activation_state = deactivated;}
      void activate () override {};
      void deactivate () override {};
      void coupling_activation_hook () override {
        Window *frame = dynamic_cast<Window*> (get_data ());
        if (frame && !frame->refresh ()) {
          _ud->add_window_for_refresh (frame);
         }
        notify_activation (); };
    private:
      UpdateDrawing* _ud;
    };
    class AutoRefreshAction : public Process
    {
    public:
      AutoRefreshAction (UpdateDrawing *p, const string &n) : Process (p, n), _ud (p) { Process::finalize (); }
      virtual ~AutoRefreshAction () {}
      void activate () override { _ud->update_auto_refresh (); };
      void deactivate () override {}
    private:
      UpdateDrawing *_ud;
    };
  public:
    static UpdateDrawing* instance ();
    ~UpdateDrawing ();
    void activate () override;
    void deactivate () override {}
    void coupling_activation_hook () override;
    void add_window_for_refresh (Window* w) { _win_list.push_back (w); w->set_refresh (true); }
    void update_auto_refresh () {
      if (_auto_refresh->get_value()) _redraw_when_damaged->enable ();
      else _redraw_when_damaged->disable ();
    }
    std::vector<Window*> get_win_list () { return _win_list; };
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
    Coupling *_redraw_when_damaged, *_redraw_when_draw_sync, *_c_update_auto_refresh;
    UpdateDrawing ();
  };
}
