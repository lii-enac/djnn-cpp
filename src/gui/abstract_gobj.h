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

#pragma once

#include "../core/ontology/process.h"
#include "../core/ontology/coupling.h"
#include "../core/control/action.h"
#include "../core/tree/bool_property.h"
#include "../core/tree/int_property.h"
#include "../core/tree/double_property.h"
#include "../core/tree/text_property.h"
#include "../core/tree/component.h"
#include "../core/execution/graph.h"
#include "../core/utils/error.h"

namespace djnn
{

  class Window;
  extern bool gui_initialized;

  class AbstractGObjImpl {
  public:
    virtual ~AbstractGObjImpl();
  };

  class AbstractGObj : public Process
  {
  public:
    AbstractGObj () : Process(), _frame (nullptr), _damaged(notify_none), _impl(nullptr) {
      if (!gui_initialized) warning (this, "Module GUI not initialized");
    }
    AbstractGObj (Process *p, const std::string& n) : Process (p, n), _frame (nullptr), _damaged(notify_none), _impl(nullptr) {
      if (!gui_initialized) warning (this, "Module GUI not initialized");
    }
    virtual ~AbstractGObj () {};
    virtual process_type_e get_cpnt_type () const override { return GOBJ_T; }
    Window*& frame () { return _frame; }
    void impl_activate () override;
    void impl_deactivate () override;
    void notify_change (unsigned int nm) override { _damaged |= nm; }
    void reset_damaged (unsigned int nm) { _damaged &= ~nm; }
    void reset_damaged () { _damaged = notify_none; }
    int get_damaged () { return _damaged; }

    AbstractGObjImpl* impl() { return _impl; }
    void set_impl(AbstractGObjImpl* i) { _impl=i; }
    
  protected:
    virtual Process* create_GObj_prop (BoolPropertyProxy **prop, Coupling  **cprop, bool *rawp, const string& name, int notify_mask);
    virtual Process* create_GObj_prop (IntPropertyProxy **prop, Coupling  **cprop, int *rawp, const string& name, int notify_mask);
    virtual Process* create_GObj_prop (DoublePropertyProxy **prop, Coupling  **cprop, double *rawp, const string& name, int notify_mask);
    virtual Process* create_GObj_prop (TextPropertyProxy **prop, Coupling  **cprop, string *rawp, const string& name, int notify_mask);

  protected:
    Window *_frame;
    unsigned int _damaged;
    AbstractGObjImpl *_impl;
  };


  class UpdateDrawing : public Process
  {
  private:
    class RedrawAction : public Action {
      public:
        RedrawAction (UpdateDrawing *p, const string &n) : Action (p, n), _ud (p) { Process::finalize_construction (); }
        virtual ~RedrawAction () {}
        void impl_activate () override ;
        void impl_deactivate () override {}
      private:
        UpdateDrawing* _ud;
    };
    class UndelayedSpike : public Process
    {
    public:
      UndelayedSpike (UpdateDrawing *p, const string &n)  : Process (p, n), _ud (p) { set_is_model (true); Process::finalize_construction (); }
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
      AutoRefreshAction (UpdateDrawing *p, const string &n) : Action (p, n), _ud (p) { Process::finalize_construction (); }
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
    void update_auto_refresh () {
      if (_auto_refresh->get_value()) _redraw_when_damaged->enable ();
      else _redraw_when_damaged->disable ();
    }
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
    Coupling *_redraw_when_damaged, *_redraw_when_draw_sync, *_c_update_auto_refresh;
    UpdateDrawing ();
  };
}
