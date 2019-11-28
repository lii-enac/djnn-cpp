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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"
#include "core/control/action.h"

#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/text_property.h"

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
    virtual void set_cursor (const string &path, int hotX, int hotY) = 0;
    Picking* picking_view () { return _picking_view;};
    void set_picking_view (Picking* p) { _picking_view = p;};
     
  private:
    Picking *_picking_view;
  };

  // template<typename T>
  // class valid_ptr
  // {
  // public:
  //     valid_ptr() = delete;
  //     valid_ptr(valid_ptr const&) = delete;
  //     valid_ptr(T data) : ptr_(new T{data}) {}
  //     void operator=(valid_ptr const&) = delete;
  //     ~valid_ptr() {delete ptr_;}

  //     T* operator->() {return ptr_;};
  //     void reset(T data) {this->~valid_ptr(); ptr_ = new T{data};}
  //     void set(T value) {*ptr_ = value;}
  //     T get() {return *ptr_;}
  //     bool is_valid () const { return ptr_ != nullptr; }
  //     void invalidate () { ptr_ = nullptr; }

  // private:
  //     T* ptr_;
  // };

  class Window : public Process
  {
  public:
    Window (const std::string& title, double x, double y, double w, double h);
    Window (Process *parent, const std::string& name, const std::string& title, double x, double y, double w, double h);
    virtual ~Window ();
    virtual process_type_e get_cpnt_type () const override { return WINDOW_T; }

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
    Process* close () { return _close; }
    WinImpl* win_impl () { return _win_impl; }
    void set_refresh (bool r) { _refresh = r; }
    bool refresh () { return _refresh; }
    void update () { _win_impl->update (); };
    void impl_activate () override { _win_impl->impl_activate (); }
    void impl_deactivate () override { _win_impl->impl_deactivate (); }
    Picking* picking_view () { return _win_impl->picking_view ();}
    void set_picking_view (Picking* p) { _win_impl->set_picking_view(p);}
    Process* press () { return _press; }
    Process* move () { return _move; }
    Process* release () { return _release; }
    Process* wheel () { return _wheel; }
    Process* touches () { return _touches; }
    DoubleProperty* press_x () { return _press_x; }
    DoubleProperty* press_y () { return _press_y; }
    DoubleProperty* move_x () { return _move_x; }
    DoubleProperty* move_y () { return _move_y; }
    void set_frame ();
    Process* holder () { return _holder; }
    void set_holder (Process *p) { _holder = p; }
    void set_cursor (const string &path, int hotX, int hotY) { _win_impl->set_cursor (path, hotX, hotY); }

    //std::weak_ptr<Window> get_weak_ptr();
    //std::shared_ptr<Window> _self_shared_ptr;

  private:
    void init_ui (const std::string &title, double x, double y, double w, double h);
    void set_frame_to_component (Process* c);
    DoubleProperty* _pos_x;
    DoubleProperty* _pos_y;
    DoubleProperty* _w_dx, *_w_dy;
    DoubleProperty* _width;
    DoubleProperty* _height;
    TextProperty* _title;
    DoubleProperty* _hidpi_scale;
    Process *_close;
    Process* _press;
    Process* _move;
    Process* _release;
    Process* _wheel;
    Process* _left_button;
    Process* _right_button;
    Process* _middle_button;
    Process* _touches;
    DoubleProperty *_press_x;
    DoubleProperty *_press_y;
    DoubleProperty *_move_x;
    DoubleProperty *_move_y;
    TextProperty *_key_pressed_text;
    IntProperty *_key_pressed;
    TextProperty *_key_released_text;
    IntProperty *_key_released;
    WinImpl *_win_impl;
    bool _refresh;
    Process* _holder;
  };

   class Cursor : public Process {
    class UpdateCursorAction : public Action {
    public:
      UpdateCursorAction (Process *parent, const string &name) : Action (parent, name) {}
      ~UpdateCursorAction () {}
      void impl_activate () override;
      void impl_deactivate () override {};
    };
   public:
    Cursor (Process *parent, const string &name, const string &path, int hotX, int hotY);
    virtual ~Cursor ();
    Window* get_win ();
    void impl_activate () override;
    void impl_deactivate () override;
    void update_cursor ();
    Process* find_component (const string &n) override;
   private:
    struct raw_props_t { int hot_x; int hot_y; string path; };
    raw_props_t raw_props;
    Coupling *_c_x, *_c_y, *_c_path;
    UpdateCursorAction *_action;
    Window* _win;
  };

} /* namespace djnn */
