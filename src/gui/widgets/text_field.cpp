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

#include "text_field.h"
#include "../../core/utils/error.h"

namespace djnn
{
  TextField::TextField (Process *parent, const string &name, Process* text, Process* shape) :
      Process (name), _start_select (0), _end_select (0), _is_selecting (false)
  {
    _press_x = dynamic_cast<DoubleProperty*>(shape->find_component ("press/x"));
    _press_y = dynamic_cast<DoubleProperty*>(shape->find_component ("press/y"));
    _move_x  = dynamic_cast<DoubleProperty*>(shape->find_component ("move/x"));
    _move_y  = dynamic_cast<DoubleProperty*>(shape->find_component ("move/y"));

    if (_press_x == nullptr || _press_y == nullptr) {
      error (this, "wrong shape argument in LineEdit constructor");
    }
    _text = dynamic_cast<Text*> (text);
    if (_text == nullptr) {
      /* check for a possible SVGHolder */
      SVGHolder *holder = dynamic_cast<SVGHolder*> (text);
      if (holder != nullptr) {
        _text = dynamic_cast<Text*> (holder->get_gobj ());
      }
    }
    if (_text == nullptr) {
      error (this, "wrong text argument in LineEdit constructor");
    }
    _inverted_matrix = (Homography*) _text->find_component ("inverted_matrix");

    _move_left        = new Spike (this, "move_left");
    _move_right       = new Spike (this, "move_right");
    _del              = new Spike (this, "del");
    _suppr            = new Spike (this, "suppr");
    _clear            = new Spike (this, "clear");
    _cut              = new Spike (this, "cut");
    _start_selection  = new Spike (this, "start_selection");
    _end_selection    = new Spike (this, "end_selection");
    _pointer_move     = new Spike (this, "pointer_move");
    _select_all       = new Spike (this, "select_all");

    _x_start  = new IntProperty (this, "x_start", 0);
    _x_end    = new IntProperty (this, "x_cursor", 0);
    _index    = new IntProperty (this, "index", 0);
    _new_text = new TextProperty (this, "new_text", "");

    _clear_action             = new ClearAction (this, "clear_action");
    _delete_action            = new DeleteAction (this, "delete_action");
    _suppr_action             = new SupprAction (this, "suppr_action");
    _add_string_action        = new AddStringAction (this, "add_string_action");
    _move_left_action         = new MoveLeftAction (this, "move_left_action");
    _move_right_action        = new MoveRightAction (this, "move_right_action");
    _update_cursor_pos_action = new UpdateCursorPositionAction (this, "update_cursor_pos_action");
    _cut_action               = new CutAction (this, "cut_action");
    _pointer_move_action      = new PointerMoveAction (this, "pointer_move_action");
    _start_selection_action   = new StartSelectionAction (this, "start_selection_action");
    _end_selection_action     = new EndSelectionAction (this, "end_selection_action");
    _select_all_action        = new SelectAllAction (this, "select_all_action");

    _c_clear      = new Coupling (_clear, ACTIVATION, _clear_action, ACTIVATION, true);
    _c_on_del     = new Coupling (_del, ACTIVATION, _delete_action, ACTIVATION, true);
    _c_on_suppr   = new Coupling (_suppr, ACTIVATION, _suppr_action, ACTIVATION, true);
    _c_on_add     = new Coupling (_new_text, ACTIVATION, _add_string_action, ACTIVATION, true);
    _c_on_press   = new Coupling (shape->find_component ("press"), ACTIVATION, _update_cursor_pos_action, ACTIVATION, true);
    _c_cut        = new Coupling (_cut, ACTIVATION, _cut_action, ACTIVATION, true);
    _c_on_move    = new Coupling (_pointer_move, ACTIVATION, _pointer_move_action, ACTIVATION, true);
    _c_move_left  = new Coupling (_move_left, ACTIVATION, _move_left_action, ACTIVATION, true);
    _c_move_right = new Coupling (_move_right, ACTIVATION, _move_right_action, ACTIVATION, true);
    _c_start_sel  = new Coupling (_start_selection, ACTIVATION, _start_selection_action, ACTIVATION, true);
    _c_end_sel    = new Coupling (_end_selection, ACTIVATION, _end_selection_action, ACTIVATION, true);
    _c_select_all = new Coupling (_select_all, ACTIVATION, _select_all_action, ACTIVATION, true);

    Process::finalize_construction (parent, name, nullptr);
  }

  TextField::~TextField ()
  {
    delete _c_select_all;
    delete _c_end_sel;
    delete _c_start_sel;
    delete _c_move_right;
    delete _c_move_left;
    delete _c_on_move;
    delete _c_cut;
    delete _c_on_press;
    delete _c_on_add;
    delete _c_on_suppr;
    delete _c_on_del;
    delete _c_clear;

    delete _select_all_action;
    delete _end_selection_action;
    delete _start_selection_action;
    delete _pointer_move_action;
    delete _cut_action;
    delete _update_cursor_pos_action;
    delete _move_right_action;
    delete _move_left_action;
    delete _add_string_action;
    delete _suppr_action;
    delete _delete_action;
    delete _clear_action;

    delete _select_all;
    delete _new_text;
    delete _index;
    delete _x_end;
    delete _x_start;
    delete _pointer_move;
    delete _end_selection;
    delete _start_selection;
    delete _cut;
    delete _clear;
    delete _suppr;
    delete _del;
    delete _move_right;
    delete _move_left;
  }

  void
  TextField::impl_activate ()
  {
    _c_select_all->enable ();
    _c_end_sel->enable ();
    _c_start_sel->enable ();
    _c_move_right->enable ();
    _c_move_left->enable ();
    _c_on_move->enable ();
    _c_cut->enable ();
    _c_on_press->enable ();
    _c_on_add->enable ();
    _c_on_suppr->enable ();
    _c_on_del->enable ();
    _c_clear->enable ();
  }

  void
  TextField::impl_deactivate ()
  {
    _c_select_all->disable ();
    _c_end_sel->disable ();
    _c_start_sel->disable ();
    _c_move_right->disable ();
    _c_move_left->disable ();
    _c_on_move->disable ();
    _c_cut->disable ();
    _c_on_press->disable ();
    _c_on_add->disable ();
    _c_on_suppr->disable ();
    _c_on_del->disable ();
    _c_clear->disable ();
  }

  static int
  sizeof_back_utf8char(const std::string& str)
  {
    if (str.empty ())
      return 0;

    auto cp = str.data () + str.size ();
    int offset = 1;
    while (--cp >= str.data () && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
      offset++;
    }
    return offset;
  }

  static int
  sizeof_front_utf8char(const std::string& str)
  {
    if (str.empty ())
      return 0;

    auto cp = str.data ();
    int offset = 1;
    while (++cp <= (str.data () + str.size ()) && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
      offset++;
    }
    return offset;
  }

  void
  TextField::del ()
  {
    if (_start_select != _end_select) {
      cut ();
      return;
    }
    string cur_text = _text->get_raw_text ();
    int i = _index->get_value ();
    int offset = 0;
    if (i > 0) {
      if (cur_text.length () == 1) {
        delete_text (0, 1);
      }
      else {
        offset = sizeof_back_utf8char (cur_text);
        delete_text (i - offset, offset);
      }
      _index->set_value (i - offset, true);
      _start_select = _end_select = i - offset;
      int i = update_cursor_pos_from_index ();
      _x_start->set_value (i, true);
    }
  }

  void
  TextField::clear ()
  {
    _index->set_value (0, true);
    _text->text ()->set_value ("", true);
    _start_select = _end_select = 0;
    int i = update_cursor_pos_from_index ();
    _x_start->set_value (i, true);
  }

  void
  TextField::suppr ()
  {
    if (_start_select != _end_select) {
      cut ();
      return;
    }
    string cur_text = _text->get_raw_text ();
    if (_index->get_value () < cur_text.length ()) {
      string buff = cur_text.substr (_index->get_value ());
      int offset = sizeof_front_utf8char (buff);
      delete_text (_index->get_value(), offset);
      _start_select = _end_select = _index->get_value();
    }
  }

  void
  TextField::add_string ()
  {
    if (_start_select != _end_select)
      cut ();
    if (_new_text->get_value().length() == 0)
      return;
    string cur_text = _text->get_raw_text ();
    if (_index->get_value () >= cur_text.length()) {
      cur_text = cur_text.append (_new_text->get_value());
      _index->set_value ((int)cur_text.length (), true);
    }
    else {
      cur_text = cur_text.insert (_index->get_value (), _new_text->get_value());
      _index->set_value (_index->get_value () +  (int)_new_text->get_value ().length (), true);
    }
    _text->text ()->set_value (cur_text, true);
    int i = update_cursor_pos_from_index ();
    _x_start->set_value (i, true);
  }

  void
  TextField::move_left ()
  {
    if (_index->get_value () > 0) {
      string buff = _text->get_raw_text ().substr (0, _index->get_value ());
      int offset = sizeof_back_utf8char (buff);
      _index->set_value (_index->get_value() - offset, true);
      int i = update_cursor_pos_from_index ();
      _end_select = _index->get_value ();
      if (!_is_selecting) {
        _start_select = _end_select;
        _x_start->set_value (i, true);
      }
    }
  }

  void
  TextField::move_right ()
  {
    string cur_text = _text->get_raw_text ();
    if (_index->get_value () < cur_text.length()) {
      string buff = cur_text.substr (_index->get_value ());
      int offset = sizeof_front_utf8char (buff);
      _index->set_value (_index->get_value() + offset, true);
      int i = update_cursor_pos_from_index ();
      _end_select = _index->get_value ();
      if (!_is_selecting) {
        _x_start->set_value (i, true);
        _start_select = _end_select;
      }
    }
  }

  int
  TextField::update_cursor_pos_from_index ()
  {
    int x = _text->get_cursor_from_index (_index->get_value());
    _x_end->set_value (x, true);
    return x;
  }

  void
  TextField::update_cursor_pos_from_press ()
  {
    // compute local x
    double resultX = _inverted_matrix->raw_props.m11 * _press_x->get_value ()
        + _inverted_matrix->raw_props.m12 * _press_y->get_value () + _inverted_matrix->raw_props.m14
        - _text->origin_x ()->get_value ();
    pair<double,int> res = _text->get_cursor_from_local_x (resultX);
    _x_start->set_value (res.first, true);
    _x_end->set_value (res.first, true);
    _index->set_value (res.second, true);
    _start_select = _end_select = res.second;
  }

  void
  TextField::pointer_move ()
  {
    if (!_is_selecting)
      return;
    // compute local x from move
    double resultX = _inverted_matrix->raw_props.m11 * _move_x->get_value ()
        + _inverted_matrix->raw_props.m12 * _move_y->get_value () + _inverted_matrix->raw_props.m14
        - _text->origin_x ()->get_value ();
    pair<double,int> res = _text->get_cursor_from_local_x (resultX);
    _x_end->set_value (res.first, true);
    _index->set_value (res.second, true);
    _end_select = res.second;
  }

  void
  TextField::delete_text (int from, int to)
  {
    string cur_text = _text->get_raw_text ();
    if (cur_text.empty () || from < 0)
      return;
    cur_text = cur_text.erase (from, to);
    _text->text ()->set_value (cur_text, true);
  }

  void
  TextField::cut ()
  {
    if (_start_select == _end_select)
      return;
    string cur_text = _text->get_raw_text ();
    if (_start_select < _end_select)
    {
      delete_text (_start_select, _end_select - _start_select);
      _index->set_value (_start_select, true);
      _end_select = _start_select;
    } else {
      delete_text (_end_select, _start_select - _end_select);
      _index->set_value (_end_select, true);
      _start_select = _end_select;
    }
    int i = update_cursor_pos_from_index ();
    _x_start->set_value (i, true);
  }

  void
  TextField::start_selection ()
  {
    _start_select = _end_select = _index->get_value ();
    _is_selecting = true;
  }

  void
  TextField::end_selection ()
  {
    _start_select = _end_select = _index->get_value ();
    _is_selecting = false;
  }

  void
  TextField::select_all ()
  {
    _start_select = 0;
    _end_select = _text->get_raw_text ().size ();
    _index->set_value (_end_select, true);
    _x_start->set_value (0, true);
    update_cursor_pos_from_index ();
  }
}
