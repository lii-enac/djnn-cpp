/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019-2026)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */
#include "gui/widgets/text_field.h"

#include "core/core-dev.h" // graph
#include "core/utils/error.h"
#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/sdf.h"
#include "gui/shape/svg_holder.h"
#include "gui/transformation/homography.h"

namespace djnn {

void 
TextField::ContentGeometryChangedAction::impl_activate() {
    TextField* tf = (TextField*)get_parent();
    Backend::instance()->update_text_field_geometry(tf);
}


TextField::TextField (CoreProcess* parent, const string& name, int x, int y,
                      int width, int height, const string& text,
                      bool enable_edit_on_activation)
    : AbstractGShape (parent, name),
      _line (this, "content", 0, 0, text),
      _cursor_start_x (this, "cursor_start_x", 0), _cursor_start_y (this, "cursor_start_y", 0),
      _cursor_end_x (this, "cursor_end_x", 0), _cursor_end_y (this, "cursor_end_y", 0),
      _cursor_height (this, "cursor_height", 16), _x (this, "x", x), _y (this, "y", y),
      _width (this, "width", width), _height (this, "height", height),
      _line_height (this, "line_height", 16),
      _content_width (this, "content_width", 0), _content_height (this, "content_height", 0),
      _content_ascent (this, "content_ascent", 0), _content_descent (this, "content_descent", 0),
      _key_code_pressed (this, "key_pressed", 0),
      _key_code_released (this, "key_released", 0),
      _text_color (this, "text_color", 0xffffff),
      _selected_text_color (this, "text_selected_color", 0xffffff),
      _selection_color (this, "selection_color", 0x429fe0), _str_input (this, "string_input", ""),
      _enable_edit (this, "enable_edit"), _disable_edit (this, "disable_edit"), _content_changed (this, "content_changed"),
      _clear (this, "clear"), _validate (this, "validate"),
      _on_enable_edit (this, "on_enable_edit"), _on_disable_edit (this, "on_disable_edit"),
      _on_press (this, "on_press_action"), _on_release (this, "on_release_action"),
      _on_move (this, "on_move_action"), _on_double_click (this, "on_double_click_action"), _key_pressed (this, "key_pressed_action"),
      _key_released (this, "key_released_action"), _on_str_input (this, "on_str_input_action"),
      _on_clear (this, "on_clear"), _on_selectable_changed (this, "on_selectable_changed"),
      _on_content_geometry_changed (this, "on_content_geometry_changed"),
      _c_key_press (&_key_code_pressed, ACTIVATION, &_key_pressed, ACTIVATION),
      _c_key_release (&_key_code_released, ACTIVATION, &_key_released, ACTIVATION),
      _c_str_input (&_str_input, ACTIVATION, &_on_str_input, ACTIVATION),
      _c_press (), _c_release (), _c_move (), _c_double_click (),
      _c_x (&_x, ACTIVATION, nullptr, ACTIVATION),
      _c_y (&_y, ACTIVATION, nullptr, ACTIVATION),
      _c_enable_edit (&_enable_edit, ACTIVATION, &_on_enable_edit, ACTIVATION),
      _c_disable_edit (&_disable_edit, ACTIVATION, &_on_disable_edit, ACTIVATION),
      _c_clear (&_clear, ACTIVATION, &_on_clear, ACTIVATION),
      _c_selectable (), _c_content_geometry (),
      _font_metrics (nullptr), _ordering_node (), _index_x (0), _ascent (0), _descent (0),
      _leading (0), _index_y (0), _start_sel_x (0), _end_sel_x (0), _start_sel_y (0),
      _end_sel_y (0), _shift_on (false), _ctrl_on (false), _alt_on (false), _press_on (false),
      _enable_edit_on_activation (enable_edit_on_activation), _first_draw (true),
      _edit_enabled (this, "edit_enabled", enable_edit_on_activation),
      _read_only (this, "read_only", false), _selectable (this, "selectable", true), _offset (0)
{
    init_ui ();
    
    graph_add_edge (&_on_str_input, &_ordering_node);
    graph_add_edge (&_on_press, &_ordering_node);
    graph_add_edge (&_on_release, &_ordering_node);
    graph_add_edge (&_on_move, &_ordering_node);
    graph_add_edge (&_on_double_click, &_ordering_node);
    graph_add_edge (&_key_pressed, &_ordering_node);
    graph_add_edge (&_key_released, &_ordering_node);

    graph_add_edge (&_ordering_node, &_cursor_start_x);
    graph_add_edge (&_ordering_node, &_cursor_end_x);
    graph_add_edge (&_ordering_node, &_cursor_start_y);
    graph_add_edge (&_ordering_node, &_cursor_end_y);
    graph_add_edge (&_ordering_node, &_content_changed);
    graph_add_edge (&_ordering_node, &_validate);

    Backend::instance ()->update_text_field_geometry (this);
    finalize_construction (parent, name);
}

TextField::~TextField ()
{
    graph_remove_edge (&_ordering_node, &_validate);
    graph_remove_edge (&_ordering_node, &_content_changed);
    graph_remove_edge (&_ordering_node, &_cursor_end_y);
    graph_remove_edge (&_ordering_node, &_cursor_start_y);
    graph_remove_edge (&_ordering_node, &_cursor_end_x);
    graph_remove_edge (&_ordering_node, &_cursor_start_x);

    graph_remove_edge (&_key_released, &_ordering_node);
    graph_remove_edge (&_key_pressed, &_ordering_node);
    graph_remove_edge (&_on_double_click, &_ordering_node);
    graph_remove_edge (&_on_move, &_ordering_node);
    graph_remove_edge (&_on_release, &_ordering_node);
    graph_remove_edge (&_on_press, &_ordering_node);
    graph_remove_edge (&_on_str_input, &_ordering_node);
}

void
TextField::impl_activate ()
{
    _index_x = _start_sel_x = _end_sel_x = 0;
    update_cursor ();
    _content_changed.set_activation_flag (NONE_ACTIVATION);

    AbstractGShape::impl_activate ();
    _c_press.init (this->find_child ("press"), ACTIVATION, &_on_press,
                   ACTIVATION, false);
    _c_release.init (get_frame ()->find_child ("release"), ACTIVATION,
                     &_on_release, ACTIVATION, false);
    _c_move.init (get_frame ()->find_child ("move"), ACTIVATION, &_on_move,
                  ACTIVATION, false);
    _c_double_click.init (this->find_child ("double_click"), ACTIVATION, &_on_double_click,
                   ACTIVATION, false);
    _c_x.set_dst (get_frame ()->damaged ());
    _c_y.set_dst (get_frame ()->damaged ());
    _c_x.enable ();
    _c_y.enable ();
    _c_press.enable ();
    _c_enable_edit.enable ();
    _c_disable_edit.enable ();
    _c_clear.enable ();
    _c_selectable.init (&_selectable, ACTIVATION, &_on_selectable_changed, ACTIVATION);
    _c_content_geometry.init (_line.find_child ("text"), ACTIVATION, &_on_content_geometry_changed, ACTIVATION);
    _c_selectable.enable ();
    _c_content_geometry.enable ();

    if (!_enable_edit_on_activation) {
        _c_str_input.disable ();
        _c_key_press.disable ();
        _c_release.disable ();
        _c_move.disable ();
        _c_double_click.disable ();
    } else {
        _c_str_input.enable ();
        _c_key_press.enable ();
        _c_release.enable ();
        _c_move.enable ();
        _c_double_click.enable ();
    }

    _line.activate ();
    Backend::instance ()->update_text_field_geometry (this);
    _shift_on = _ctrl_on = _press_on = false;
}

void
TextField::impl_deactivate ()
{
    AbstractGShape::impl_deactivate ();
    _c_x.disable ();
    _c_y.disable ();
    _c_key_press.disable ();
    _c_press.disable ();
    _c_release.disable ();
    _c_move.disable ();
    _c_double_click.disable ();
    _c_str_input.disable ();
    _c_enable_edit.disable ();
    _c_disable_edit.disable ();
    _c_clear.disable ();
    _c_selectable.disable ();
    _c_content_geometry.disable ();
    _line.deactivate ();
}

void
TextField::set_editable (bool v)
{
    if (v) {
        _c_str_input.enable ();
        _c_key_press.enable ();
        _c_release.enable ();
        _c_move.enable ();
        _c_double_click.enable ();
    } else {
        //clear the selection when disabling editing
        if (has_selection ()) {
            _start_sel_x = _end_sel_x = 0;
            update_cursor ();
        }
        _c_str_input.disable ();
        _c_key_press.disable ();
        _c_release.disable ();
        _c_move.disable ();
        _c_double_click.disable ();
        _offset = 0;
    }
    _edit_enabled.set_value (v, true);
}

void
TextField::update_cursor ()
{
    int start_x = Backend::instance ()->compute_x (_font_metrics, &_line, _start_sel_x);
    int end_x   = Backend::instance ()->compute_x (_font_metrics, &_line, _end_sel_x);
    int w       = _width.get_value ();
    if (has_selection ()) {
        int lenght_sel = end_x - start_x;
        if (end_x - _offset > w) {
            _offset = end_x - w;
            end_x   = w;
        } else if (end_x < _offset) {
            _offset -= _offset - end_x;
            end_x = 0;
        } else {
            end_x -= _offset;
        }
        start_x = end_x - lenght_sel;
    } else {
        if (start_x - _offset > w) {
            _offset = start_x - w;
            start_x = w;
        } else if (start_x < _offset) {
            _offset -= _offset - start_x;
            start_x = 0;
        } else {
            start_x -= _offset;
        }
        end_x = start_x;
    }
    _cursor_start_x.set_value (start_x, true);
    _cursor_end_x.set_value (end_x, true);
    update_drawing ();
}

coord_t
TextField::get_index_from_x (coord_t x)
{
    return Backend::instance ()->compute_index (_font_metrics, &_line, x);
}

coord_t
TextField::get_x_from_index ()
{
    return Backend::instance ()->compute_x (_font_metrics, &_line, _index_x);
}

void
TextField::update_index_from_xy (coord_t x, coord_t y)
{
    _index_x = Backend::instance ()->compute_index (_font_metrics, &_line, x);
}

pair<double, double>
TextField::get_local_coords (double x, double y)
{
    /* compute local coords */
    Homography* h     = dynamic_cast<Homography*> (this->inverted_matrix ());
    double      loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m14 - this->origin_x ()->get_value ();
    double      loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y + h->raw_props.m24 - this->origin_y ()->get_value ();
    return pair<double, double> (loc_x, loc_y);
}

void
TextField::mouse_press ()
{
    if (!_selectable.get_value ())
        return;
    if (!_edit_enabled.get_value ()) {
        set_editable (true);
        _edit_enabled.set_value (true, true);
    }
    if (getRef (get_frame ()->find_child ("touches/$added")) != nullptr) {
        setRef (get_frame ()->find_child ("touches/$added"), nullptr);
        return; // ignore and cancel touch
    }
    auto* press_x =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("press/x"));
    auto* press_y =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("press/y"));
    pair<double, double> local = get_local_coords (
        press_x->get_double_value (), press_y->get_double_value ());
    int x = (int)local.first - this->x () + _offset;
    int y = (int)local.second - this->y ();
    update_index_from_xy (x, y);
    _start_sel_x = _end_sel_x = _index_x;
    update_cursor ();
    _press_on = true;
}

void
TextField::mouse_release ()
{
    _press_on = false;
}

void
TextField::mouse_move ()
{
    if (!_selectable.get_value ())
        return;
    if (!_press_on)
        return;
    auto* move_x =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("move/x"));
    auto* move_y =
        dynamic_cast<AbstractSimpleProperty*> (get_frame ()->find_child ("move/y"));
    pair<double, double> local = get_local_coords (move_x->get_double_value (),
                                                   move_y->get_double_value ());
    int                  x     = (int)local.first - this->x () + _offset;
    int                  y     = (int)local.second - this->y ();
    update_index_from_xy (x, y);
    _end_sel_x = _index_x;
    update_cursor ();
}

void
TextField::mouse_double_click ()
{
    if (!_selectable.get_value ())
        return;
    _start_sel_x = 0;
    _end_sel_x = _index_x = _line.get_content ().length ();
    update_cursor ();
}

void
TextField::key_released ()
{
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
TextField::key_pressed ()
{
    if (!_selectable.get_value ())
        return;
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
        // add_str ("\t");
        return;
    }
    if (key == DJN_Key_Return) {
        if (_read_only.get_value ())
            return;
        _validate.notify_activation ();
        set_editable (false);
        return;
    }
    if (key == DJN_Key_Left) {
        if (has_selection () && !_shift_on) {
            sort_selection ();
            _index_x = _end_sel_x = _start_sel_x;
            if (_alt_on) {
                _index_x = _end_sel_x = _start_sel_x = previous_word (
                    _line.get_content (), _index_x);
            } else if (_ctrl_on && _index_x > 0) {
                string str = _line.get_content ();
                _index_x = _end_sel_x = _start_sel_x = first_word (str, _index_x);
            }
        } else {
            if (_index_x > 0) {
                _index_x =
                    _alt_on ? previous_word (_line.get_content (), _index_x) : (_ctrl_on ? first_word (_line.get_content (), _index_x) : previous_index (_line.get_content (), _index_x));
            }
            if (_shift_on) {
                _end_sel_x = _index_x;
            } else {
                _start_sel_x = _end_sel_x = _index_x;
            }
        }
        update_cursor ();
        return;
    }
    if (key == DJN_Key_Right) {
        if (has_selection () && !_shift_on) {
            sort_selection ();
            _index_x = _start_sel_x = _end_sel_x;
            if (_alt_on) {
                _index_x = _end_sel_x = _start_sel_x = next_word (
                    _line.get_content (), _index_x);
            } else if (_ctrl_on) {
                _index_x = _end_sel_x = _start_sel_x = _line.get_content ().length ();
            }
        } else {
            if (_index_x < _line.get_content ().size ()) {
                _index_x =
                    _alt_on ? next_word (_line.get_content (), _index_x) : (_ctrl_on ? _line.get_content ().length () : next_index (_line.get_content (), _index_x));
            }
            if (_shift_on) {
                _end_sel_x = _index_x;
            } else {
                _start_sel_x = _end_sel_x = _index_x;
            }
        }
        update_cursor ();
        return;
    }
    if (key == DJN_Key_Up) {
        if (_ctrl_on) {
            if (_shift_on) {
                if (_end_sel_x > _start_sel_x) {
                    _end_sel_x = _start_sel_x;
                } else {
                    sort_selection ();
                }
                _start_sel_x = _index_x = 0;
            } else {
                _end_sel_x = _index_x = _start_sel_x = 0;
            }
            update_cursor ();
            return;
        }
        if (has_selection () && !_shift_on) {
            sort_selection ();
            _end_sel_x = _index_x = _start_sel_x;
            update_cursor ();
            return;
        }
        _index_x = 0;
        if (_shift_on) {
            _end_sel_x = _index_x;
        } else {
            _end_sel_x = _start_sel_x = _index_x;
        }
        update_cursor ();
        return;
    }
    if (key == DJN_Key_Down) {
        if (_ctrl_on) {
            if (_shift_on) {
                sort_selection ();
                _start_sel_x = _end_sel_x;
            } else {
                _start_sel_x = _line.get_content ().size ();
            }
            _end_sel_x = _index_x = _line.get_content ().size ();
            update_cursor ();
            return;
        }
        _index_x = _line.get_content ().size ();
        if (_shift_on) {
            _end_sel_x = _index_x;
        } else {
            _end_sel_x = _start_sel_x = _index_x;
        }
        update_cursor ();
        return;
    }
    if (key == DJN_Key_Backspace) {
        if (_read_only.get_value ())
            return;
        if (has_selection ()) {
            del_selection ();
        } else {
            if (_index_x == 0) {
                return;
            } else {
                string str      = _line.get_content ();
                int    prev_idx = previous_index (str, _index_x);
                int    l        = _index_x - prev_idx;
                str.erase (prev_idx, l);
                _start_sel_x = _end_sel_x = _index_x = prev_idx;
                _line.set_content (str);
            }
            _content_changed.set_activation_flag (ACTIVATION);
        }
        update_cursor ();
        return;
    }
    if (key == DJN_Key_Delete) {
        if (_read_only.get_value ())
            return;
        if (has_selection ()) {
            del_selection ();
        } else {
            if (_index_x == _line.get_content ().size ()) {
                return;
            } else {
                string str      = _line.get_content ();
                int    next_idx = next_index (str, _index_x);
                int    l        = next_idx - _index_x;
                str.erase (_index_x, l);
                _line.set_content (str);
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
        if (_read_only.get_value ())
            return;
        paste ();
        return;
    }
    if (key == DJN_Key_X && _ctrl_on) {
        if (_read_only.get_value ())
            return;
        copy ();
        del_selection ();
        return;
    }
    if (key == DJN_Key_A && _ctrl_on) {
        _start_sel_x = 0;
        _end_sel_x = _index_x = _line.get_content ().length ();
        update_cursor ();
    }
}

void
TextField::sort_selection ()
{
    if (_start_sel_x > _end_sel_x) {
        int buff     = _start_sel_x;
        _start_sel_x = _end_sel_x;
        _end_sel_x   = buff;
    }
}

void
TextField::clear ()
{
    if (_read_only.get_value ())
        return;
    _start_sel_x = 0;
    _end_sel_x   = _line.get_content ().length ();
    del_selection ();
}

void
TextField::del_selection ()
{
    if (_read_only.get_value ())
        return;
    sort_selection ();
    string cur_text = _line.get_content ();
    cur_text.erase (_start_sel_x, _end_sel_x - _start_sel_x);
    _line.set_content (cur_text);
    _index_x = _end_sel_x = _start_sel_x;
    update_cursor ();
    _content_changed.set_activation_flag (ACTIVATION);
}

void
TextField::add_string_input ()
{
    if (_read_only.get_value ())
        return;
    string& str = _str_input.get_value ();
    if (str.empty ())
        return;
    add_str (str);
}

void
TextField::add_str (const string& str)
{
    if (_read_only.get_value ())
        return;
    if (str.empty ())
        return;
    string        cur_text = _line.get_content ();
    const string& cpy      = str;
    if (has_selection ()) {
        del_selection ();
        cur_text = _line.get_content ();
    }

    if (_index_x == cur_text.length ()) {
        cur_text.append (cpy);
        _index_x = _start_sel_x = _end_sel_x = cur_text.length ();
        _line.set_content (cur_text);
    } else {
        cur_text.insert (_index_x, cpy);
        _index_x = _start_sel_x = _end_sel_x = _index_x + cpy.length ();
        _line.set_content (cur_text);
    }
    update_cursor ();
    _content_changed.set_activation_flag (ACTIVATION);
}

void
TextField::copy ()
{
    if (_start_sel_x == _end_sel_x)
        return; 
    sort_selection ();
    string content         = _line.get_content ();
    int    length_init_sel = _end_sel_x - _start_sel_x;
    string str             = content.substr (_start_sel_x, length_init_sel);
    Backend::instance ()->set_clipboard_text (str);
}

void
TextField::paste ()
{
    string str = Backend::instance ()->get_clipboard_text ();
    add_str (str);
}

void
TextField::selectable_changed ()
{
    if (!_selectable.get_value () && has_selection ()) {
        _start_sel_x = _end_sel_x = _index_x;
        update_cursor ();
    }
}

void
TextField::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        AbstractGShape::pre_draw ();
        Backend::instance ()->draw_text_field (this);
        _first_draw = false;
        int cursor_height  = static_cast<int> (_ascent + _descent);
        int cursor_start_y = text_y_offset (cursor_height);
        _cursor_height.set_value (cursor_height, true);
        _cursor_start_y.set_value (cursor_start_y, true);
        _cursor_end_y.set_value (cursor_start_y + cursor_height, true);
        _line_height.set_value (static_cast<int> (_ascent + _descent + _leading), true);
        AbstractGShape::post_draw ();
    }
}
} // namespace djnn
