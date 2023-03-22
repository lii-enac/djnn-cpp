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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "gui/shape/sdf.h"

#include "gui/widgets/multiline_edit.h"
#include "gui/transformation/homography.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/tree/component.h"
#include "display/window.h"

#include "core/utils/iostream.h"
#include "utils/debug.h" // UNIMPL
#include "core/utils/error.h"

namespace djnn
{

  MultilineEditor::MultilineEditor (ParentProcess *parent, const string &name,
                                    int x, int y, int width, int height,
                                    const string &text,
                                    bool enable_edit_on_activation,
                                    bool wrap_text) :
      AbstractGShape (parent, name), 
      _lines (this, "lines"), 
      _cursor_start_x (this, "cursor_start_x", 0), 
      _cursor_start_y (this, "cursor_start_y", 0),
      _cursor_end_x (this, "cursor_end_x", 0),
      _cursor_end_y (this, "cursor_end_y", 0),
      _cursor_height (this, "cursor_height", 16),
      _x (this, "x", x),
      _y (this, "y", y),
      _width (this, "width", width),
      _height (this, "height", height),
      _line_height (this, "line_height", 16),
      _spaces_for_tab (this, "spaces_for_tab", 0),
      _key_code_pressed (this, "key_pressed", 0),
      _key_code_released (this, "key_released", 0),
      _str_input (this, "string_input", ""),
      _copy_buffer (this, "copy_buffer", ""),
      _toggle_edit (this, "toggle_edit"),
      _content_changed (this, "content_changed"),
      _clear (this, "clear"),
      _line (nullptr),
      _toggle_action (this, "toggle_edit_action"),
      _on_press (this, "on_press_action"),
      _on_release (this, "on_release_action"),
      _on_move (this, "on_move_action"),
      _key_pressed (this, "key_pressed_action"),
      _key_released (this, "key_released_action"),
      _on_str_input (this, "on_str_input_action"),
      _on_clear (this, "on_clear"),
      _c_key_press (&_key_code_pressed, ACTIVATION, &_key_pressed, ACTIVATION),
      _c_key_release (&_key_code_released, ACTIVATION, &_key_released, ACTIVATION),
      _c_str_input (&_str_input, ACTIVATION, &_on_str_input, ACTIVATION), 
      _c_press (), 
      _c_release (), 
      _c_move (), 
      _c_x (&_x, ACTIVATION, nullptr, ACTIVATION),
      _c_y (&_y, ACTIVATION, nullptr, ACTIVATION),
      _c_toggle (&_toggle_edit, ACTIVATION, &_toggle_action, ACTIVATION),
      _c_clear (&_clear, ACTIVATION, &_on_clear, ACTIVATION),
      _font_metrics (nullptr),
      _ordering_node (),
      _index_x (0),
      _index_y (0),
      _ascent (0),
      _descent (0),
      _leading (0),
      _start_sel_x (0),
      _start_sel_y (0),
      _end_sel_x (0),
      _end_sel_y (0),
      _shift_on (false),
      _ctrl_on (false),
      _alt_on (false),
      _press_on (false),
      _enable_edit_on_activation (enable_edit_on_activation),
      _first_draw (true),
      _wrap_text (wrap_text),
      _init_text (text)
  {
    init_ui ();

    graph_add_edge (&_on_str_input, &_ordering_node);
    graph_add_edge (&_on_press, &_ordering_node);
    graph_add_edge (&_on_release, &_ordering_node);
    graph_add_edge (&_on_move, &_ordering_node);
    graph_add_edge (&_key_pressed, &_ordering_node);
    graph_add_edge (&_key_released, &_ordering_node);

    graph_add_edge (&_ordering_node, &_cursor_start_x);
    graph_add_edge (&_ordering_node, &_cursor_end_x);
    graph_add_edge (&_ordering_node, &_cursor_start_y);
    graph_add_edge (&_ordering_node, &_cursor_end_y);
    graph_add_edge (&_ordering_node, &_content_changed);

    finalize_construction (parent, name);
  }

  MultilineEditor::~MultilineEditor () {
    graph_remove_edge (&_ordering_node, &_content_changed);
    graph_remove_edge (&_ordering_node, &_cursor_end_y);
    graph_remove_edge (&_ordering_node, &_cursor_start_y);
    graph_remove_edge (&_ordering_node, &_cursor_end_x);
    graph_remove_edge (&_ordering_node, &_cursor_start_x);

    graph_remove_edge (&_key_released, &_ordering_node);
    graph_remove_edge (&_key_pressed, &_ordering_node);
    graph_remove_edge (&_on_move, &_ordering_node);
    graph_remove_edge (&_on_release, &_ordering_node);
    graph_remove_edge (&_on_press, &_ordering_node);
    graph_remove_edge (&_on_str_input, &_ordering_node);
  }

  void
  MultilineEditor::impl_activate () {
    if (_line == nullptr) {
      if (_lines.children ().empty ()) {
        _line = new SimpleText (this, "", 0, 0, "");
        add_str (_init_text);
        _init_text = "";
        _index_y = _start_sel_y = _end_sel_y = 0;
        _index_x = _start_sel_x = _end_sel_x = 0;
        _line = get_line (0);
        update_cursor ();
        _content_changed.set_activation_flag (NONE_ACTIVATION);
      }
      else _line = get_line (0);
    }
    AbstractGShape::impl_activate ();
    _c_press.init (this->find_child ("press"), ACTIVATION, &_on_press, ACTIVATION, false);
    _c_release.init (get_frame ()->find_child ("release"), ACTIVATION, &_on_release, ACTIVATION, false);
    _c_move.init (get_frame ()->find_child ("move"), ACTIVATION, &_on_move, ACTIVATION, false);
    _lines.activate ();

    _c_x.set_dst (get_frame ()->damaged ());
    _c_y.set_dst (get_frame ()->damaged ());
    _c_x.enable ();
    _c_y.enable ();
    if (!_enable_edit_on_activation) {
      _c_str_input.disable ();
      _c_key_press.disable ();
      _c_press.disable ();
      _c_release.disable ();
      _c_move.disable ();
    }
    else {
      _c_str_input.enable ();
      _c_key_press.enable ();
      _c_press.enable ();
      _c_release.enable ();
      _c_move.enable ();
    }
    _shift_on = _ctrl_on = _press_on = false;
  }

  void
  MultilineEditor::impl_deactivate () {
    AbstractGShape::impl_deactivate ();
    _lines.deactivate ();
    _c_x.disable ();
    _c_y.disable ();
    _c_key_press.disable ();
    _c_press.disable ();
    _c_release.disable ();
    _c_move.disable ();
  }

  void
  MultilineEditor::toggle_edit () {
    if (_c_press.is_enabled ()) {
      _c_str_input.disable ();
      _c_key_press.disable ();
      _c_press.disable ();
      _c_release.disable ();
      _c_move.disable ();
    }
    else {
      _c_str_input.enable ();
      _c_key_press.enable ();
      _c_press.enable ();
      _c_release.enable ();
      _c_move.enable ();
    }
  }

  void
  MultilineEditor::update_selection () {

  }

  void
  MultilineEditor::update_cursor () {
    if (has_selection ()) {
      SimpleText *l1 = get_line (_start_sel_y);
      SimpleText *l2 = get_line (_end_sel_y);
      _cursor_start_x.set_value (
          Backend::instance ()->compute_x (_font_metrics, l1, _start_sel_x),
          true);
      _cursor_start_y.set_value (l1->get_y (), true);
      _cursor_end_x.set_value (
          Backend::instance ()->compute_x (_font_metrics, l2, _end_sel_x),
          true);
      _cursor_end_y.set_value (l2->get_y (), true);
    }
    else {
      _cursor_start_x.set_value (
          Backend::instance ()->compute_x (_font_metrics, _line, _index_x),
          true);
      _cursor_start_y.set_value (_line->get_y (), true);
      _cursor_end_x.set_value (_cursor_start_x.get_value (), true);
      _cursor_end_y.set_value (_cursor_start_y.get_value (), true);
    }
  }

  coord_t
  MultilineEditor::get_index_from_x (coord_t x) {
    return Backend::instance ()->compute_index (_font_metrics, _line, x);
  }

  coord_t
  MultilineEditor::get_x_from_index () {
    return Backend::instance ()->compute_x (_font_metrics, _line, _index_x);
  }
  void
  MultilineEditor::update_index_from_xy (coord_t x, coord_t y) {
    if (_lines.children ().empty ()) {
      warning(this, "No more line in simple text editor");
      return;
    }
    _line = nullptr;
    // find the right line
    if (y <= 0) {
      _line = (SimpleText*) _lines.children ().at (0);
      _index_y = 0;
    }
    else {
      for (int i = 0; i < _lines.children ().size (); i++) {
        int cur_y = (get_line (i))->get_y () + _ascent;
        if (y <= (cur_y + _descent) && y >= (cur_y - _ascent)) {
          _line = get_line (i);
          _index_y = i;
          break;
        }
      }
    }
    if (_line == nullptr) {
      _line = (SimpleText*) _lines.children ().back ();
      _index_x = _line->get_content ().size ();
      _index_y = _lines.children ().size () - 1;
    }
    else {
      _index_x = Backend::instance ()->compute_index (_font_metrics, _line, x);
    }
  }

  pair<double, double>
  MultilineEditor::get_local_coords (double x, double y) {
    /* compute local coords */
    Homography *h = dynamic_cast<Homography*> (this->inverted_matrix ());
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y
        + h->raw_props.m14 - this->origin_x ()->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y
        + h->raw_props.m24 - this->origin_y ()->get_value ();
    return pair<double, double> (loc_x, loc_y);
  }

  void
  MultilineEditor::mouse_press () {
    if (getRef (get_frame ()->find_child ("touches/$added")) != nullptr) {
      setRef (get_frame ()->find_child ("touches/$added"), nullptr);
      return; // ignore and cancel touch
    }
    auto * press_x =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("press/x"));
    auto * press_y =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("press/y"));
    pair<double, double> local = get_local_coords (
        press_x->get_double_value (), press_y->get_double_value ());
    int x = (int) local.first - this->x ();
    int y = (int) local.second - this->y ();
    update_index_from_xy (x, y);
    _start_sel_x = _end_sel_x = _index_x;
    _start_sel_y = _end_sel_y = _index_y;
    update_cursor ();
    _press_on = true;
  }

  void
  MultilineEditor::mouse_release () {
    _press_on = false;
  }

  void
  MultilineEditor::mouse_move () {
    if (!_press_on) return;
    auto * move_x =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("move/x"));
    auto * move_y =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("move/y"));
    pair<double, double> local = get_local_coords (move_x->get_double_value (), move_y->get_double_value ());
    int x = (int) local.first - this->x ();
    int y = (int) local.second - this->y ();
    update_index_from_xy (x, y);
    _end_sel_x = _index_x;
    _end_sel_y = _index_y;
    update_cursor ();
  }

  void
  MultilineEditor::key_released () {
    int key = _key_code_released.get_value ();
    if (key == DJN_Key_Shift) {
      _shift_on = false;
      return;
    }
    if (key == DJN_Key_Control) {
      _ctrl_on = false;
      return;
    }
    if (key == DJN_Key_Alt) {
      _alt_on = false;
      return;
    }
  }

  void
  MultilineEditor::key_pressed () {
    int key = _key_code_pressed.get_value ();
    if (key == DJN_Key_Shift) {
      _shift_on = true;
      return;
    }
    if (key == DJN_Key_Control) {
      _ctrl_on = true;
      return;
    }
    if (key == DJN_Key_Alt) {
      _alt_on = true;
      return;
    }
    if (key == DJN_Key_Tab) {
      string tab;
      if (_spaces_for_tab.get_value () == 0) tab = "\t";
      else {
        for (int i = 0; i < _spaces_for_tab.get_value (); i++)
          tab.append (" ");
      }
      add_str (tab);
      return;
    }
    if (key == DJN_Key_Return) {
      if (has_selection ()) del_selection ();
      int height = _ascent + _descent;
      int y = (get_line (_index_y))->get_y () + height + _leading;
      SimpleText *buff_line = _line;
      //check if we are at the end of the line
      if (_index_x == _line->get_content ().size ()) _line = new SimpleText (
          this, "", 0, y, "");
      else {
        //if not, cut the current line
        string cur_content = _line->get_content ();
        string sub = cur_content.substr (_index_x);
        cur_content.erase (_index_x);
        _line->set_content (cur_content);
        _line = new SimpleText (this, "", 0, y, sub);
      }
      _lines.move_child (_line, AFTER, buff_line);
      _index_x = 0;
      _index_y++;
      _start_sel_x = _end_sel_x = 0;
      _start_sel_y = _end_sel_y = _index_y;
      _height.set_value (
          static_cast<int> ((_height.get_value () + height + _leading)), true);
      if (_index_y < _lines.children ().size ()) {
        update_lines_position ();
      }
      update_cursor ();
      _content_changed.set_activation_flag (ACTIVATION);
      return;
    }
    if (key == DJN_Key_Left) {
      if (has_selection () && !_shift_on) {
        sort_selection ();
        _line = get_line (_start_sel_y);
        _index_x = _end_sel_x = _start_sel_x;
        _index_y = _end_sel_y = _start_sel_y;
        if (_alt_on) {
          _index_x = _end_sel_x = _start_sel_x = previous_word (
              _line->get_content (), _index_x);
        }
        else if (_ctrl_on && _index_x > 0) {
          string str = _line->get_content ();
          _index_x = _end_sel_x = _start_sel_x = first_word (str, _index_x);
        }
      }
      else {
        if (_index_x > 0) {
          _index_x =
              _alt_on ?
                  previous_word (_line->get_content (), _index_x) :
                  (_ctrl_on ?
                      first_word (_line->get_content (), _index_x) :
                      previous_index (_line->get_content (), _index_x));
        }
        else if (_index_y > 0) {
          _index_y--;
          _line = get_line (_index_y);
          _index_x = _line->get_content ().size ();
        }
        if (_shift_on) {
          _end_sel_x = _index_x;
          _end_sel_y = _index_y;
        }
        else {
          _start_sel_x = _end_sel_x = _index_x;
          _start_sel_y = _end_sel_y = _index_y;
        }
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Right) {
      if (has_selection () && !_shift_on) {
        sort_selection ();
        _line = get_line (_end_sel_y);
        _index_x = _start_sel_x = _end_sel_x;
        _index_y = _start_sel_y = _end_sel_y;
        if (_alt_on) {
          _index_x = _end_sel_x = _start_sel_x = next_word (
              _line->get_content (), _index_x);
        }
        else if (_ctrl_on) {
          _index_x = _end_sel_x = _start_sel_x =
              _line->get_content ().length ();
        }
      }
      else {
        if (_index_x < _line->get_content ().size ()) {
          _index_x =
              _alt_on ?
                  next_word (_line->get_content (), _index_x) :
                  (_ctrl_on ?
                      _line->get_content ().length () :
                      next_index (_line->get_content (), _index_x));
        }
        else if (_index_y < _lines.children ().size () - 1) {
          _index_y++;
          _line = get_line (_index_y);
          _index_x = 0;
        }
        if (_shift_on) {
          _end_sel_x = _index_x;
          _end_sel_y = _index_y;
        }
        else {
          _start_sel_x = _end_sel_x = _index_x;
          _start_sel_y = _end_sel_y = _index_y;
        }
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Up) {
      bool moved = false;
      if (_ctrl_on) {
        if (_shift_on) {
          if (_end_sel_y > _start_sel_y || _end_sel_x > _start_sel_x) {
            _end_sel_y = _start_sel_y;
            _end_sel_x = _start_sel_x;
          }
          else {
            sort_selection ();
          }
          _start_sel_x = _start_sel_y = _index_y = _index_x = 0;
        }
        else {
          _index_y = _end_sel_y = _start_sel_y = 0;
          _end_sel_x = _index_x = _start_sel_x = 0;
        }
        _line = get_line (_index_y);
        update_cursor ();
        return;
      }
      if (has_selection () && !_shift_on) {
        sort_selection ();
        _index_y = _end_sel_y = _start_sel_y;
        _end_sel_x = _index_x = _start_sel_x;
        _line = get_line (_index_y);
        update_cursor ();
        return;
      }
      if (_index_y > 0) {
        _index_y--;
        moved = true;
      }
      _line = get_line (_index_y);
      if (_index_x > _line->get_content ().size () && moved) _index_x =
          _line->get_content ().size ();
      else if (moved) {
        //this may appear strange but it is not, because the line has changed
        _index_x = get_index_from_x (get_x_from_index ());
      }
      else _index_x = 0;
      if (_shift_on) {
        _end_sel_x = _index_x;
        _end_sel_y = _index_y;
      }
      else {
        _end_sel_x = _start_sel_x = _index_x;
        _end_sel_y = _start_sel_y = _index_y;
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Down) {
      if (_ctrl_on) {
        int last = _lines.children ().size () - 1;
        if (_shift_on) {
          sort_selection ();
          _start_sel_y = _end_sel_y;
          _start_sel_x = _end_sel_x;
        }
        else {
          _start_sel_y = last;
          _start_sel_x = get_line (last)->get_content ().size ();
        }
        _end_sel_y = _index_y = last;
        _line = get_line (_index_y);
        _end_sel_x = _index_x = _line->get_content ().size ();
        update_cursor ();
        return;
      }
      bool moved = false;
      if (_index_y < _lines.children ().size () - 1) {
        _index_y++;
        moved = true;
      }
      _line = get_line (_index_y);
      if (_index_x > _line->get_content ().size () || !moved) _index_x =
          _line->get_content ().size ();
      else _index_x = get_index_from_x (get_x_from_index ());
      if (_shift_on) {
        _end_sel_x = _index_x;
        _end_sel_y = _index_y;
      }
      else {
        _end_sel_x = _start_sel_x = _index_x;
        _end_sel_y = _start_sel_y = _index_y;
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Backspace) {
      if (has_selection ()) {
        del_selection ();
      }
      else {
        if (_index_x == 0) {
          if (_index_y > 0) {
            SimpleText *prev_line = get_line (_index_y - 1);
            string content = prev_line->get_content () + _line->get_content ();
            _end_sel_x = _start_sel_x = _index_x =
                prev_line->get_content ().size ();
            prev_line->set_content (content);
            _end_sel_y = _start_sel_y = _index_y - 1;
            _index_y--;
            delete_line (_line);
            _line = prev_line;
            update_lines_position ();
          }
        }
        else {
          string str = _line->get_content ();
          int prev_idx = previous_index (str, _index_x);
          int l = _index_x - prev_idx;
          str.erase (prev_idx, l);
          _start_sel_x = _end_sel_x = _index_x = prev_idx;
          _line->set_content (str);
        }
        _content_changed.set_activation_flag (ACTIVATION);
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Delete) {
      if (has_selection ()) {
        del_selection ();
      }
      else {
        if (_index_x == _line->get_content ().size ()) {
          if (_index_y < _lines.children ().size ()) {
            SimpleText *next_line = get_line (_index_y + 1);
            string content = _line->get_content () + next_line->get_content ();
            _end_sel_x = _start_sel_x = _index_x;
            _line->set_content (content);
            _end_sel_y = _start_sel_y = _index_y;
            delete_line (next_line);
            update_lines_position ();
          }
        }
        else {
          string str = _line->get_content ();
          int next_idx = next_index (str, _index_x);
          int l = next_idx - _index_x;
          str.erase (_index_x, l);
          _line->set_content (str);
        }
        _content_changed.set_activation_flag (ACTIVATION);
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_C && _ctrl_on) {
      copy ();
      return;
    }
    if (key == DJN_Key_V && _ctrl_on) {
      paste ();
      return;
    }
    if (key == DJN_Key_X && _ctrl_on) {
      copy ();
      del_selection ();
      return;
    }
    if (key == DJN_Key_A && _ctrl_on) {
      _line = (SimpleText*) _lines.children ().back ();
      _start_sel_x = _start_sel_y = 0;
      _end_sel_y = _index_y = _lines.children ().size () - 1;
      _end_sel_x = _index_x = _line->get_content ().length ();
      update_cursor ();
    }
  }

  void
  MultilineEditor::wrap () {
    if (_font_metrics == nullptr)
      return;

    int average_width = Backend::instance ()->get_average_char_width (_font_metrics);
    int max_width = (int) _width.get_value ();
    int nb_of_char = max_width/average_width;
    std::string s;
    int sz = _lines.children ().size();
    for (int i = 0; i < sz; i++) {
      SimpleText* cur_line = (SimpleText*)_lines.children().at(i);
      std::string content = cur_line->get_content ();
      if (content.empty())
        content = "\n";
      s += content;
      if (i < sz -1 && s.back() != '\n')
        s += "\n";
    }
    int i = 0;
    int len = s.length();
    int cur_idx = 0;
    int start_idx = 0;
    while (cur_idx < len) {
      sz = _lines.children ().size(); //sz may have changed
      start_idx = cur_idx;
      for (int j = 0; j < nb_of_char; j++) {
        cur_idx = next_index(s, cur_idx);
        if (cur_idx >= len || s.at(cur_idx) == '\n')
          break;
      }
      if (cur_idx < len) {
        int buff_cur_idx = cur_idx;
        while (!std::isspace (s.at(cur_idx))) { // got to the space before the current word
          cur_idx--;
          if (cur_idx == start_idx) { // if the current (too long) word is the first then cut it
            cur_idx = buff_cur_idx;
            break;
          }
        }
      }
      if (s.at(start_idx) == '\n')
        start_idx++;
      std::string line = s.substr (start_idx, cur_idx - start_idx);
      if (line.back() == '\n')
        line.pop_back();
      if (i < sz) {
        SimpleText* cur_line = (SimpleText*)_lines.children().at(i);
        cur_line->set_content (line);
      } else {
        SimpleText *buff = new SimpleText (this, "", 0, 0, line);
        _lines.move_child (buff, LAST);
      }
      i++;
    }
    if (i < sz - 1) {
      for (int a = i; a < sz; a++) {
        SimpleText* cur_line = (SimpleText*)_lines.children().at(a);
        std::string str = "";
        cur_line->set_content (str);
      }
    }
  }

  void
  MultilineEditor::sort_selection () {
    if (_start_sel_y == _end_sel_y) {
      if (_start_sel_x > _end_sel_x) {
        int buff = _start_sel_x;
        _start_sel_x = _end_sel_x;
        _end_sel_x = buff;
      }
    }
    else if (_start_sel_y > _end_sel_y) {
      int buff_x = _start_sel_x;
      int buff_y = _start_sel_y;
      _start_sel_x = _end_sel_x;
      _start_sel_y = _end_sel_y;
      _end_sel_x = buff_x;
      _end_sel_y = buff_y;
    }
  }

  void
  MultilineEditor::delete_line (SimpleText *st) {
    st->deactivate ();
    _lines.remove_child (st);
    st->schedule_deletion ();
  }

  void
  MultilineEditor::clear () {
    _start_sel_y = 0;
    _end_sel_y = _lines.children ().size () - 1;
    _start_sel_x = 0;
    _end_sel_x = 1000; //large arbitrary value to catch the last character of the last line, it should be actually calculated
    del_selection ();
  }

  void
  MultilineEditor::del_selection () {
    sort_selection ();

    if (_start_sel_y == _end_sel_y) {
      // Single line selection
      string cur_text = _line->get_content ();
      cur_text.erase (_start_sel_x, _end_sel_x - _start_sel_x);
      _line->set_content (cur_text);
    }
    else {
      // Multi-line selection
      vector<SimpleText*> to_delete;
      SimpleText *start_line = (SimpleText*) _lines.children ().at (
          _start_sel_y);
      SimpleText *end_line = (SimpleText*) _lines.children ().at (_end_sel_y);
      string start_text = start_line->get_content ();
      start_text.erase (_start_sel_x);
      string end_text = end_line->get_content ();
      end_text.erase (0, _end_sel_x);
      start_text.append (end_text);
      start_line->set_content (start_text);

      for (int i = _start_sel_y + 1; i <= _end_sel_y; i++) {
        to_delete.push_back ((SimpleText*) _lines.children ().at (i));
      }
      for (auto p : to_delete) {
        delete_line (p);
      }
    }
    _index_y = _end_sel_y = _start_sel_y;
    _index_x = _end_sel_x = _start_sel_x;
    _line = (SimpleText*) _lines.children ().at (_index_y);
    update_lines_position ();
    update_cursor ();
    _content_changed.set_activation_flag (ACTIVATION);
  }

  void
  MultilineEditor::update_lines_position () {
    int height = _ascent + _descent;
    for (int i = 0; i < _lines.children ().size (); i++) {
      ((DoubleProperty*) (_lines.children ().at (i)->find_child ("y")))->set_value (
          static_cast<int> (i * (height + _leading)), true);
    }
  }

  void
  MultilineEditor::add_string_input () {
    string str = _str_input.get_value ();
    if (str.empty ()) return;
    add_str (str);
  }

  void
  MultilineEditor::add_str (const string &str) {
    if (str.empty ()) return;
    string cur_text = _line->get_content ();
    string cpy = str;
    if (has_selection ()) del_selection ();

    int off_y = _ascent + _descent + _leading;
    size_t found = cpy.find ("\n");

    if (_index_x == cur_text.length ()) {
      if (found == string::npos) {
        cur_text.append (cpy);
        _index_x = _start_sel_x = _end_sel_x = cur_text.length ();
        _line->set_content (cur_text);
      }
      else {
        cur_text.append (cpy.substr (0, found));
        _line->set_content (cur_text);
        cpy = cpy.substr (found + 1);
        found = cpy.find ("\n");
        int y = (get_line (_index_y))->get_y () + off_y;
        while (found != string::npos) {
          SimpleText *buff = new SimpleText (this, "", 0, y, cpy.substr (0, found));
          _lines.move_child (buff, AFTER, _line);
          _line = buff;
          cpy = cpy.substr (found + 1);
          found = cpy.find ("\n");
          y += off_y;
          _index_y++;
        }
        SimpleText *buff = new SimpleText (this, "", 0, y, cpy);
        _lines.move_child (buff, AFTER, _line);
        _line = buff;
        _index_y++;
        _index_x = _line->get_content ().size ();
        update_lines_position ();
      }
    }
    else {
      if (found == string::npos) {
        cur_text.insert (_index_x, cpy);
        _index_x = _start_sel_x = _end_sel_x = _index_x + cpy.length ();
        _line->set_content (cur_text);
      }
      else {
        string start = cur_text.substr (0, _index_x);
        string end = cur_text.substr (_index_x);
        cur_text.erase (_index_x);
        cur_text.insert (_index_x, cpy.substr (0, found));
        _line->set_content (cur_text);
        cpy = cpy.substr (found + 1);
        found = cpy.find ("\n");
        int y = (get_line (_index_y))->get_y () + off_y;
        while (found != string::npos) {
          SimpleText *buff = new SimpleText (this, "", 0, y, cpy.substr (0, found));
          _lines.move_child (buff, AFTER, _line);
          _line = buff;
          cpy = cpy.substr (found + 1);
          found = cpy.find ("\n");
          y += off_y;
          _index_y++;
        }
        cpy.append (end);
        SimpleText *buff = new SimpleText (this, "", 0, y, cpy);
        _lines.move_child (buff, AFTER, _line);
        _line = buff;
        _index_y++;
        _index_x = _line->get_content ().size ();
        update_lines_position ();
      }
    }
    if (_wrap_text) {
      wrap ();
      update_lines_position ();
    }
    update_cursor ();
    _content_changed.set_activation_flag (ACTIVATION);
  }

  void
  MultilineEditor::copy () {
    if (_start_sel_x == _end_sel_x && _start_sel_y == _end_sel_y) return;
    sort_selection ();
    string content = (get_line (_start_sel_y))->get_content ();
    int length_init_sel =
        _end_sel_y == _start_sel_y ?
            _end_sel_x - _start_sel_x : content.length () - _start_sel_x;
    string str = content.substr (_start_sel_x, length_init_sel);

    if (_end_sel_y != _start_sel_y) {
      str.append ("\n");
      int i = _start_sel_y + 1;
      while (i < _end_sel_y) {
        str.append ((get_line (i++))->get_content ());
        str.append ("\n");
      }
      str.append (
          (get_line (_end_sel_y))->get_content ().substr (0, _end_sel_x));
    }
    _copy_buffer.set_value (str, true);
  }

  void
  MultilineEditor::paste () {
    string str = _copy_buffer.get_value ();
    add_str (str);
  }

  void
  MultilineEditor::draw () {
    auto _frame = get_frame ();
    if (somehow_activating ()
        && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_simple_text_edit (this);
      _first_draw = false;
      _cursor_height.set_value (static_cast<int> (_ascent + _descent), true);
      _line_height.set_value (static_cast<int> (_ascent + _descent + _leading),
                              true);
      AbstractGShape::post_draw ();
    }
  }
} /* namespace djnn */
