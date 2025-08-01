/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2021)
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

#include "core/control/action.h"
#include "core/ontology/process.h"
#include "core/property/int_property_fwd.h"
#include "core/property/ref_property.h"
#include "core/tree/component.h"

namespace djnn {
// class IntProperty;
class DoubleProperty;
class TextProperty;
class BoolProperty;
class RefProperty;
class Spike;
class Blank;
class BackgroundColor;
class BackgroundRect;

class Picking;
enum cursor_t {
    DJNN_ARROW,
    DJNN_UP_ARROW,
    DJNN_CROSS,
    DJNN_WAIT,
    DJNN_IBEAM,
    DJNN_SIZE_VER,
    DJNN_SIZE_HOR,
    DJNN_SIZE_BDIAG,
    DJNN_SIZE_FDIAG,
    DJNN_SIZE_ALL,
    DJNN_BLANK,
    DJNN_SPLIT_V,
    DJNN_SPLIT_H,
    DJNN_POINTING_HAND,
    DJNN_FORBIDDEN,
    DJNN_OPEN_HAND,
    DJNN_CLOSED_HAND,
    DJNN_WHATS_THIS,
    DJNN_BUSY,
    DJNN_DRAG_MOVE,
    DJNN_DRAG_COPY,
    DJNN_DRAG_LINK
};

class WinImpl {
  public:
    WinImpl ()
        : _picking_view (nullptr) {}
    virtual ~WinImpl () {}
    virtual void impl_activate ()                                             = 0;
    virtual void impl_deactivate ()                                           = 0;
    virtual void update ()                                                    = 0;
    virtual void set_cursor (const djnnstl::string& path, int hotX, int hotY) = 0;
    virtual void set_cursor (int cursor_shape)                                = 0;
    Picking*     picking_view () { return _picking_view; };
    void         set_picking_view (Picking* p) { _picking_view = p; };
    virtual void perform_screenshot (const djnnstl::string& path) {}
    virtual void set_opacity (double opacity) {}
    virtual void set_background_opacity_and_color (double is_opaque, int r, int g, int b, bool isFrameless = false) {}
    virtual void set_minimum_size (int w, int h) {}
    virtual void update_geometry () {}

  private:
    Picking* _picking_view;
};

class Window : public FatProcess {
    class UndelayedSpike : public FatProcess {
      public:
        UndelayedSpike (Window* parent, const string& name)
            : FatProcess (name) {
            set_is_model (true);
            finalize_construction (parent, name);
        }
        virtual ~UndelayedSpike () {}
        void post_activate () override { post_activate_auto_deactivate (); }
        void impl_activate () override;
        void impl_deactivate () override {};
    };

    class ScreenshotAction : public Action {
      public:
        ScreenshotAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->perform_screenshot (); }
    };
    class OpacityAction : public Action {
      public:
        OpacityAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->set_opacity (); }
    };
    class BackgroundOpacityAction : public Action {
      public:
        BackgroundOpacityAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->set_background_opacity_and_color (); }
    };
    class BackgroundColorAction : public Action {
      public:
        BackgroundColorAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->set_background_opacity_and_color (); }
    };
    class GeometryAction : public Action {
      public:
        GeometryAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->update_geometry (); }
    };
    class MinimumSizeAction : public Action {
      public:
        MinimumSizeAction (Window* parent, const string& name)
            : Action (parent, name) {}
        void impl_activate () override { ((Window*)get_parent ())->set_minimum_size (); }
    };

  public:
    Window (CoreProcess* parent, const string& name, const string& title, double x, double y, double w, double h);
    Window (CoreProcess* parent, const string& name, const string& title);
    Window (CoreProcess* parent, const string& name);
    virtual ~Window ();
    virtual process_type_e get_process_type () const override { return WINDOW_T; }

    void     update () { _win_impl->update (); };
    void     impl_activate () override;
    void     impl_deactivate () override;
    Picking* picking_view () { return _win_impl->picking_view (); }
    void     set_picking_view (Picking* p) { _win_impl->set_picking_view (p); }

    DoubleProperty* pos_x () { return _pos_x; }
    DoubleProperty* pos_y () { return _pos_y; }
    DoubleProperty* width () { return _width; }
    DoubleProperty* height () { return _height; }
    IntProperty*    min_width () { return _min_width; }
    IntProperty*    min_height () { return _min_height; }
    TextProperty*   title () { return _title; }
    IntProperty*    key_pressed () { return _key_pressed; }
    IntProperty*    key_released () { return _key_released; }
    TextProperty*   key_pressed_text () { return _key_pressed_text; }
    TextProperty*   key_released_text () { return _key_released_text; }
    DoubleProperty* hidpi_scale () { return _hidpi_scale; }
    DoubleProperty* mspf () { return _mspf; }
    FatProcess*     close () { return _close; }
    WinImpl*        win_impl () { return _win_impl; }
    void            set_refresh (bool r) { _refresh = r; }
    bool            refresh () { return _refresh; }

    FatProcess*     press () { return _press; }
    FatProcess*     move () { return _move; }
    FatProcess*     release () { return _release; }
    FatProcess*     wheel () { return _wheel; }
    FatProcess*     touches () { return _touches; }
    DoubleProperty* press_x () { return _press_x; }
    DoubleProperty* press_y () { return _press_y; }
    DoubleProperty* move_x () { return _move_x; }
    DoubleProperty* move_y () { return _move_y; }
    DoubleProperty* release_x () { return _release_x; }
    DoubleProperty* release_y () { return _release_y; }
    DoubleProperty* wheel_dx () { return _w_dx; }
    DoubleProperty* wheel_dy () { return _w_dy; }
    DoubleProperty* wheel_x () { return _w_x; }
    DoubleProperty* wheel_y () { return _w_y; }
    FatProcess*     stylus_pen_press () { return _stylus_pen_press; };
    DoubleProperty* stylus_pen_press_x () { return _stylus_pen_press_x; };
    DoubleProperty* stylus_pen_press_y () { return _stylus_pen_press_y; };
    FatProcess*     stylus_pen_move () { return _stylus_pen_move; };
    DoubleProperty* stylus_pen_move_x () { return _stylus_pen_move_x; };
    DoubleProperty* stylus_pen_move_y () { return _stylus_pen_move_y; };
    FatProcess*     stylus_pen_release () { return _stylus_pen_release; };
    DoubleProperty* stylus_pen_release_x () { return _stylus_pen_release_x; };
    DoubleProperty* stylus_pen_release_y () { return _stylus_pen_release_y; };
    DoubleProperty* stylus_pen_pressure () { return _stylus_pen_pressure; };
    FatProcess*     stylus_eraser_press () { return _stylus_eraser_press; };
    DoubleProperty* stylus_eraser_press_x () { return _stylus_eraser_press_x; };
    DoubleProperty* stylus_eraser_press_y () { return _stylus_eraser_press_y; };
    FatProcess*     stylus_eraser_move () { return _stylus_eraser_move; };
    DoubleProperty* stylus_eraser_move_x () { return _stylus_eraser_move_x; };
    DoubleProperty* stylus_eraser_move_y () { return _stylus_eraser_move_y; };
    FatProcess*     stylus_eraser_release () { return _stylus_eraser_release; };
    DoubleProperty* stylus_eraser_release_x () { return _stylus_eraser_release_x; };
    DoubleProperty* stylus_eraser_release_y () { return _stylus_eraser_release_y; };
    DoubleProperty* stylus_eraser_pressure () { return _stylus_eraser_pressure; };
    CoreProcess*    get_display () { return _display->get_value (); }
    void            init_display (FatProcess* conn) { _display->set_value (conn, false); }
    void            set_frame ();
    FatProcess*     damaged () { return _damaged; }
    FatProcess*     holder () { return _holder; }
    void            set_holder (FatProcess* p) { _holder = p; }
    void            set_cursor (const string& path, int hotX, int hotY) { _win_impl->set_cursor (path, hotX, hotY); }
    void            set_cursor (int cursor_shape) { _win_impl->set_cursor (cursor_shape); }

    Blank* refreshed () { return _refreshed; }

    Spike*        screenshot () { return _screenshot; }
    TextProperty* screenshot_path () { return _screenshot_path; }
    void          perform_screenshot ();

    DoubleProperty* opacity () { return _opacity; }
    void            set_opacity ();

    DoubleProperty*  background_opacity () { return _background_opacity; }
    BackgroundColor* background_color () { return _background_color; }
    BoolProperty*    frameless () { return _frameless; }
    BackgroundRect*  background_rect () { return _background_rect; }
    void             set_background_opacity_and_color ();
    void             set_minimum_size ();
    void             update_geometry ();

  private:
    void            init_ui (const string& title, double x, double y, double w, double h);
    void            set_frame_to_component (FatProcess* c);
    DoubleProperty* _pos_x;
    DoubleProperty* _pos_y;
    DoubleProperty *_w_dx, *_w_dy;
    DoubleProperty *_w_x, *_w_y;
    DoubleProperty* _width;
    DoubleProperty* _height;
    TextProperty*   _title;
    DoubleProperty* _hidpi_scale;
    DoubleProperty* _mspf;
    RefProperty*    _display;
    FatProcess*     _close;
    FatProcess*     _press;
    FatProcess*     _move;
    FatProcess*     _release;
    FatProcess*     _wheel;
    FatProcess*     _stylus_pen;
    FatProcess*     _stylus_pen_press;
    FatProcess*     _stylus_pen_release;
    FatProcess*     _stylus_pen_move;
    FatProcess*     _stylus_eraser;
    FatProcess*     _stylus_eraser_press;
    FatProcess*     _stylus_eraser_release;
    FatProcess*     _stylus_eraser_move;
    FatProcess*     _left_button;
    FatProcess*     _right_button;
    FatProcess*     _middle_button;
    FatProcess*     _touches;
    DoubleProperty* _press_x;
    DoubleProperty* _press_y;
    DoubleProperty* _move_x;
    DoubleProperty* _move_y;
    DoubleProperty* _release_x;
    DoubleProperty* _release_y;
    DoubleProperty* _stylus_pen_press_x;
    DoubleProperty* _stylus_pen_press_y;
    DoubleProperty* _stylus_pen_move_x;
    DoubleProperty* _stylus_pen_move_y;
    DoubleProperty* _stylus_pen_release_x;
    DoubleProperty* _stylus_pen_release_y;
    DoubleProperty* _stylus_pen_pressure;
    DoubleProperty* _stylus_eraser_press_x;
    DoubleProperty* _stylus_eraser_press_y;
    DoubleProperty* _stylus_eraser_move_x;
    DoubleProperty* _stylus_eraser_move_y;
    DoubleProperty* _stylus_eraser_release_x;
    DoubleProperty* _stylus_eraser_release_y;
    DoubleProperty* _stylus_eraser_pressure;
    TextProperty*   _key_pressed_text;
    IntProperty*    _key_pressed;
    TextProperty*   _key_released_text;
    IntProperty*    _key_released;
    WinImpl*        _win_impl;
    UndelayedSpike* _damaged;
    Coupling*       _c_damaged_update_drawing_damaged;
    bool            _refresh;
    FatProcess*     _holder;

    Spike*            _screenshot;
    TextProperty*     _screenshot_path;
    ScreenshotAction* _screenshot_action;
    Coupling*         _c_screenshot;

    Blank* _refreshed;

    DoubleProperty* _opacity;
    OpacityAction*  _opacity_action;
    Coupling*       _c_opacity;

    // Note: could it be interactive ? not sure ?
    BoolProperty* _frameless;

    DoubleProperty*          _background_opacity;
    BackgroundOpacityAction* _background_opacity_action;
    Coupling*                _c_background_opacity;

    BackgroundColor*       _background_color;
    BackgroundColorAction* _background_color_action;
    Coupling *             _c_background_color_toValue, *_c_background_color_toRGB;

    BackgroundRect* _background_rect;

    IntProperty *      _min_width, *_min_height;
    MinimumSizeAction* _min_size_action;
    Coupling *         _c_min_width, *_c_min_height;

    GeometryAction* _geometry_action;
    Coupling*       _c_geometry_x;
    Coupling*       _c_geometry_y;
    Coupling*       _c_geometry_width;
    Coupling*       _c_geometry_height;
};

class Cursor : public FatProcess {
    class UpdateCursorAction : public Action {
      public:
        UpdateCursorAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        ~UpdateCursorAction () {}
        void impl_activate () override;
    };
    class UpdateCursorShapeAction : public Action {
      public:
        UpdateCursorShapeAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        ~UpdateCursorShapeAction () {}
        void impl_activate () override { ((Cursor*)get_parent ())->update_cursor_shape (); }
    };

  public:
    Cursor (CoreProcess* parent, const string& name, const string& path, int hotX, int hotY);
    Cursor (CoreProcess* parent, const string& name, int cursor_shape);
    virtual ~Cursor ();
    Window*      get_win ();
    void         impl_activate () override;
    void         impl_deactivate () override;
    void         update_cursor ();
    void         update_cursor_shape ();
    CoreProcess* find_child_impl (const string& n) override;

  private:
    struct raw_props_t {
        int    hot_x;
        int    hot_y;
        string path;
        int    cursor_shape;
    };
    raw_props_t              raw_props;
    Coupling *               _c_x, *_c_y, *_c_path, *_c_cursor_shape;
    UpdateCursorAction*      _action;
    UpdateCursorShapeAction* _shape_action;
    Window*                  _win;
};

} /* namespace djnn */
