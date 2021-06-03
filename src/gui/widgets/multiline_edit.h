#pragma once

#include "core/control/action.h"
#include "core/tree/double_property.h"
#include "core/tree/int_property.h"
#include "core/tree/text_property.h"
#include "core/tree/ref_property.h"
#include "core/tree/list.h"
//#include "gen/abstract_prop_text.h"
#include "gui/shape/abstract_gshape.h"

#include "gui/shape/text.h" // FontFamily FontSize FontWeight

namespace djnn {

  class SimpleText : public AbstractGShape {
    public:
      SimpleText (ParentProcess* parent, const std::string& name, double x, double y, const std::string& text);
      virtual ~SimpleText ();
      void impl_activate () override;
      void impl_deactivate () override;
      void set_parent (ParentProcess *p) override;
      std::string get_content () { return raw_props.text; }
      void set_content (std::string& new_content) { _text.set_value (new_content, true); };
      double get_x () { return raw_props.x; }
      double get_y () { return raw_props.y; }
      void set_x (double x) { raw_props.x = x; }
      void set_y (double y) { raw_props.y = y; }
      void get_properties_values (double &x, double &y, std::string &text);
      virtual FatChildProcess* find_child_impl (const std::string&) override;
    private:
      struct raw_props_t { double x, y; std::string text; };
      raw_props_t raw_props;
      TextPropertyProxy _text;
      CouplingWithData *_cx, *_cy;
      CouplingWithData _ctext;
      bool _init;
  };

  class MultilineEditor : public AbstractGShape {
      class ToggleEditAction : public Action {
        public:
          ToggleEditAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~ToggleEditAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->toggle_edit (); }
      };
      class MousePressAction : public Action {
        public:
          MousePressAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~MousePressAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->mouse_press (); }
      };
      class MouseReleaseAction : public Action {
        public:
          MouseReleaseAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~MouseReleaseAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->mouse_release (); }
      };
      class MouseMoveAction : public Action {
        public:
          MouseMoveAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~MouseMoveAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->mouse_move (); }
      };
      class KeyPressedAction : public Action {
        public:
          KeyPressedAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~KeyPressedAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->key_pressed (); }
      };
      class KeyReleasedAction : public Action {
        public:
          KeyReleasedAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~KeyReleasedAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->key_released (); }
      };
      class StrInputAction : public Action {
        public:
          StrInputAction (ParentProcess* parent, const std::string& name) : Action (parent, name) {}
          virtual ~StrInputAction () {}
          void impl_activate () override { ((MultilineEditor*)get_parent ())->add_string_input (); }
      };
    public:
      MultilineEditor (ParentProcess* parent, const std::string& name, int x, int y, int width, int height, const std::string &text = std::string(), bool enable_edit_on_activation = true);
      virtual ~MultilineEditor ();
      void impl_activate () override;
      void impl_deactivate () override;

      List* lines () { return &_lines; }
      int x () { return _x.get_value (); }
      int y () { return _y.get_value (); }
      int width () { return _width.get_value (); }
      int height () { return _height.get_value (); }
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
      SimpleText* get_line (int y) { return (SimpleText*)_lines.children ().at(y); }
      void delete_line (SimpleText* st);
      void sort_selection ();
      void del_selection ();
      void mouse_press ();
      void mouse_release ();
      void mouse_move ();
      void key_pressed ();
      void key_released ();
      void update_index_from_xy (int x, int y);
      void update_selection ();
      void update_cursor ();
      void update_lines_position ();
      void copy ();
      void paste ();
      void toggle_edit ();
      int get_x_from_index ();
      int get_index_from_x (int x);
      void add_string_input ();
      void add_str (const std::string& str );
      void draw () override;
      std::pair<double, double> get_local_coords (double x, double y);
      List _lines;
      IntProperty _cursor_start_x, _cursor_start_y, _cursor_end_x, _cursor_end_y, _cursor_height;
      IntProperty _x, _y, _width, _height, _line_height, _spaces_for_tab;
      IntProperty _key_code_pressed, _key_code_released;
      TextProperty _str_input, _copy_buffer;
      Spike _toggle_edit, _content_changed;
      SimpleText* _line;
      ToggleEditAction _toggle_action;
      MousePressAction _on_press;
      MouseReleaseAction _on_release;
      MouseMoveAction _on_move;
      KeyPressedAction _key_pressed;
      KeyReleasedAction _key_released;
      StrInputAction _on_str_input;
      Coupling _c_key_press, _c_key_release, _c_str_input, _c_press, _c_release, _c_move, _c_x, _c_y, _c_toggle;
      FontMetricsImpl _font_metrics;
      VoidProcess _ordering_node;
      int _index_x, _index_y, _ascent, _descent, _leading;
      int _start_sel_x, _start_sel_y, _end_sel_x, _end_sel_y;
      bool _shift_on, _ctrl_on, _alt_on, _press_on, _enable_edit_on_activation, _first_draw;
      std::string _init_text;
  };
}