/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/control/action.h"

#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"

#include <memory>

namespace djnn
{

  class Picking;
  class WinImpl {
  public:
    WinImpl () : _picking_view(nullptr) {}
    virtual ~WinImpl () {}
    virtual void impl_activate () = 0;
    virtual void impl_deactivate () = 0;
    virtual void update () = 0;
    virtual void set_cursor (const std::string& path, int hotX, int hotY) = 0;
    Picking* picking_view () { return _picking_view;};
    void set_picking_view (Picking* p) { _picking_view = p;};
     
  private:
    Picking *_picking_view;
  };

  class Window : public FatProcess
  {
    class UndelayedSpike : public FatProcess
    {
      public:
        UndelayedSpike (Window * parent, const std::string& name)  : FatProcess (name) { set_is_model (true); finalize_construction (parent, name); }
        virtual ~UndelayedSpike () {}
        void post_activate () override { post_activate_auto_deactivate (); }
        void impl_activate () override;
        void impl_deactivate () override {};
    };
  public:
    Window (ParentProcess* parent, const std::string& name, const std::string& title, double x, double y, double w, double h);
    virtual ~Window ();
    virtual process_type_e get_process_type () const override { return WINDOW_T; }

    DoubleProperty* pos_x () { return _pos_x; }
    DoubleProperty* pos_y () { return _pos_y; }
    DoubleProperty* wheel_dx () { return _w_dx; }
    DoubleProperty* wheel_dy () { return _w_dy; }
    DoubleProperty* width () { return _width; }
    DoubleProperty* height () { return _height; }
    TextProperty* title () { return _title; }
    IntProperty* key_pressed () { return _key_pressed; }
    IntProperty* key_released () { return _key_released; }
    TextProperty* key_pressed_text () { return _key_pressed_text; }
    TextProperty* key_released_text () { return _key_released_text; }
    DoubleProperty* hidpi_scale () { return _hidpi_scale; }
    DoubleProperty* mspf () { return _mspf; }
    FatProcess* close () { return _close; }
    WinImpl* win_impl () { return _win_impl; }
    void set_refresh (bool r) { _refresh = r; }
    bool refresh () { return _refresh; }
    void update () { _win_impl->update (); };
    void impl_activate () override;
    void impl_deactivate () override;
    Picking* picking_view () { return _win_impl->picking_view ();}
    void set_picking_view (Picking* p) { _win_impl->set_picking_view(p);}
    FatProcess* press () { return _press; }
    FatProcess* move () { return _move; }
    FatProcess* release () { return _release; }
    FatProcess* wheel () { return _wheel; }
    FatProcess* touches () { return _touches; }
    DoubleProperty* press_x () { return _press_x; }
    DoubleProperty* press_y () { return _press_y; }
    DoubleProperty* move_x () { return _move_x; }
    DoubleProperty* move_y () { return _move_y; }
    CoreProcess* get_display () { return _display->get_value (); }
    void init_display (FatProcess *conn) { _display->set_value (conn, false); }
    void set_frame ();
    FatProcess* damaged () { return _damaged; }
    FatProcess* holder () { return _holder; }
    void set_holder (FatProcess *p) { _holder = p; }
    void set_cursor (const std::string& path, int hotX, int hotY) { _win_impl->set_cursor (path, hotX, hotY); }

  private:
    void init_ui (const std::string& title, double x, double y, double w, double h);
    void set_frame_to_component (FatProcess* c);
    DoubleProperty* _pos_x;
    DoubleProperty* _pos_y;
    DoubleProperty* _w_dx, *_w_dy;
    DoubleProperty* _width;
    DoubleProperty* _height;
    TextProperty* _title;
    DoubleProperty* _hidpi_scale;
    DoubleProperty* _mspf;
    RefProperty *_display;
    FatProcess *_close;
    FatProcess* _press;
    FatProcess* _move;
    FatProcess* _release;
    FatProcess* _wheel;
    FatProcess* _left_button;
    FatProcess* _right_button;
    FatProcess* _middle_button;
    FatProcess* _touches;
    DoubleProperty *_press_x;
    DoubleProperty *_press_y;
    DoubleProperty *_move_x;
    DoubleProperty *_move_y;
    TextProperty *_key_pressed_text;
    IntProperty *_key_pressed;
    TextProperty *_key_released_text;
    IntProperty *_key_released;
    WinImpl *_win_impl;
    UndelayedSpike *_damaged;
    //CouplingWithData2 *_c_damaged_update_drawing_damaged;
    Coupling *_c_damaged_update_drawing_damaged;
    bool _refresh;
    FatProcess* _holder;
  };

   class Cursor : public FatProcess {
    class UpdateCursorAction : public Action {
    public:
      UpdateCursorAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
      ~UpdateCursorAction () {}
      void impl_activate () override;
    };
   public:
    Cursor (ParentProcess* parent, const std::string& name, const std::string& path, int hotX, int hotY);
    virtual ~Cursor ();
    Window* get_win ();
    void impl_activate () override;
    void impl_deactivate () override;
    void update_cursor ();
    FatChildProcess* find_child (const std::string& n) override;
   private:
    struct raw_props_t { int hot_x; int hot_y; std::string path; };
    raw_props_t raw_props;
    Coupling *_c_x, *_c_y, *_c_path;
    UpdateCursorAction *_action;
    Window* _win;
  };

} /* namespace djnn */
