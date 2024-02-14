/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 */

#pragma once

#include <gui/widgets/text_edit_common.h>
#include "core/ontology/process.h"
#include "core/control/action.h"
#include "core/control/blank.h"
#include "core/property/text_property.h"
#include "core/property/double_property.h"
#include "core/property/int_property.h"
#include "core/property/bool_property.h"
#include "gui/shape/text.h"

namespace djnn {


  class TextField : public AbstractGShape
  {
  private:
    class EnableEditAction : public Action {
      public:
        EnableEditAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~EnableEditAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->set_editable (true); }
    };
    class DisableEditAction : public Action {
      public:
        DisableEditAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~DisableEditAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->set_editable (false); }
    };
    class MousePressAction : public Action {
      public:
        MousePressAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~MousePressAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->mouse_press (); }
    };
    class MouseReleaseAction : public Action {
      public:
        MouseReleaseAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~MouseReleaseAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->mouse_release (); }
    };
    class MouseMoveAction : public Action {
      public:
        MouseMoveAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~MouseMoveAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->mouse_move (); }
    };
    class KeyPressedAction : public Action {
      public:
        KeyPressedAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~KeyPressedAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->key_pressed (); }
    };
    class KeyReleasedAction : public Action {
      public:
        KeyReleasedAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~KeyReleasedAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->key_released (); }
    };
    class StrInputAction : public Action {
      public:
        StrInputAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~StrInputAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->add_string_input (); }
    };
    class ClearAction : public Action {
      public:
        ClearAction (CoreProcess* parent, const string& name) : Action (parent, name) {}
        virtual ~ClearAction () {}
        void impl_activate () override { ((TextField*)get_parent ())->clear (); }
    };
  public:
    TextField (CoreProcess* parent, const string& name, int x, int y, int width, int height, const string &text = string(), bool enable_edit_on_activation = true);
    virtual ~TextField ();
    void impl_activate () override;
    void impl_deactivate () override;

    SimpleText* content () { return &_line; }
    int x () { return _x.get_value (); }
    int y () { return _y.get_value (); }
    int width () { return _width.get_value (); }
    int height () { return _height.get_value (); }
    int offset () { return _offset; }
    int text_color () { return _text_color.get_value (); }
    int selected_text_color () { return _selected_text_color.get_value (); }
    int selection_color () { return _selection_color.get_value (); }
    int start_x () { return _cursor_start_x.get_value (); }
    int end_x () { return _cursor_end_x.get_value (); }
    void set_width (int w) { _width.set_value (w, true); }
    void set_height (int h) { _height.set_value (h, true); }
    bool has_selection () { return _start_sel_x != _end_sel_x || _start_sel_y != _end_sel_y; }
    void set_ascent (int v) { _ascent = v; }
    void set_descent (int v) { _descent = v; }
    void set_leading (int v) { _leading = v; }
    FontMetricsImpl get_font_metrics () { return _font_metrics; }
    void set_font_metrics (FontMetricsImpl fm) { _font_metrics = fm; }
    bool first_draw () { return _first_draw; }

  private:
    void sort_selection ();
    void del_selection ();
    void clear ();
    void mouse_press ();
    void mouse_release ();
    void mouse_move ();
    void key_pressed ();
    void key_released ();
    void update_index_from_xy (coord_t x, coord_t y);
    void update_cursor ();
    void update_lines_position ();
    void copy ();
    void paste ();
    void set_editable (bool v);
    coord_t get_x_from_index ();
    coord_t get_index_from_x (coord_t x);
    void add_string_input ();
    void add_str (const string& str );
    void draw () override;
    djnnstl::pair<double, double> get_local_coords (double x, double y);

    SimpleText _line;

    IntProperty _cursor_start_x, _cursor_start_y, _cursor_end_x, _cursor_end_y, _cursor_height;
    IntProperty _x, _y, _width, _height, _line_height;
    IntProperty _key_code_pressed, _key_code_released;
    IntProperty _text_color, _selected_text_color, _selection_color;
    TextProperty _str_input, _copy_buffer;

    Spike _enable_edit, _disable_edit, _content_changed, _clear;
    Blank _validate;

    EnableEditAction _on_enable_edit;
    DisableEditAction _on_disable_edit;
    MousePressAction _on_press;
    MouseReleaseAction _on_release;
    MouseMoveAction _on_move;
    KeyPressedAction _key_pressed;
    KeyReleasedAction _key_released;
    StrInputAction _on_str_input;
    ClearAction _on_clear;

    Coupling _c_key_press, _c_key_release, _c_str_input, _c_press, _c_release, _c_move, _c_x, _c_y, _c_enable_edit, _c_disable_edit, _c_clear;

    FontMetricsImpl _font_metrics;
    VoidProcess _ordering_node;
    coord_t _index_x, _ascent, _descent, _leading;
    const coord_t _index_y;
    coord_t _start_sel_x, _end_sel_x;
    const coord_t _start_sel_y, _end_sel_y;
    bool _shift_on, _ctrl_on, _alt_on, _press_on, _enable_edit_on_activation, _first_draw;
    BoolProperty _edit_enabled;
    int _offset;
  };
}
