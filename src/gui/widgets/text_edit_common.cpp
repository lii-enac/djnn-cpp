/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2022)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 */

#include "gui/widgets/text_edit_common.h"

namespace djnn
{

  SimpleText::SimpleText (CoreProcess* parent, const string& name, double x, double y, const string& text) :
      AbstractGShape (parent, name), raw_props{.x=x, .y=y, .text=text},
      _text (this, "text", raw_props.text, notify_damaged_geometry),
      _cx (nullptr), _cy (nullptr),
      _ctext (&_text, ACTIVATION, nullptr, ACTIVATION ), _init (false)
  {
    if (parent == nullptr) return;
    CoreProcess* lines = parent->find_optional_child ("lines");
    if (lines == nullptr) {
      parent->add_symbol (name, this);
      FatProcess::set_parent (parent);
      return;
    }

    _init = true;
    FatProcess::set_parent (lines);
    lines->add_child (this, "");
    parent->add_symbol (name, this);
  }

  void
  SimpleText::set_parent (CoreProcess* parent)
  {
    if (_init)
      return;
    CoreProcess* lines = parent->find_optional_child ("lines");
    if (lines == nullptr) {
      return;
    }
    _init = true;
    FatProcess::set_parent (lines);
    lines->add_child (this, "");
  }

  void
  SimpleText::impl_activate ()
  {
    AbstractGShape::impl_activate ();
    auto * damaged = get_frame ()->damaged ();
    enable (_cx, damaged);
    enable (_cy, damaged);
    enable (&_ctext, damaged);
  }

  void
  SimpleText::impl_deactivate ()
  {
    AbstractGShape::impl_deactivate ();
    disable (_cx);
    disable (_cy);
    disable (&_ctext);
  }

  SimpleText::~SimpleText () {
    delete _cy;
    delete _cx;
  }

  CoreProcess*
  SimpleText::find_child_impl (const string& name)
  {
    auto * res = AbstractGObj::find_child_impl (name);
    if(res) return res;

    CouplingWithData ** coupling;
    double* rawpd;
    int notify_mask = notify_none;

    if(name=="x") {
      coupling=&_cx;
      rawpd=&raw_props.x;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y") {
      coupling=&_cy;
      rawpd=&raw_props.y;
      notify_mask = notify_damaged_transform;
    } else
      return nullptr;

    DoublePropertyProxy* prop = nullptr;
    res = create_GObj_prop(&prop, coupling, rawpd, name, notify_mask);

    return res;
  }

  void
  SimpleText::get_properties_values (double &x, double &y, string &text)
  {
    x = raw_props.x;
    y = raw_props.y;
    text = _text.get_value ();
  }

  coord_t
  next_index (const std::string &str, int idx)
  {
    if (idx > str.size ()) return idx;
    int offset = 1;
    auto cp = str.data () + idx;
    while (++cp <= (str.data () + str.size ())
        && ((*cp & 0b10000000) && !(*cp & 0b01000000)))
      {
        offset++;
      }
    return idx + offset;
  }

  coord_t
  previous_index (const std::string &str, int idx)
  {
    if (idx <= 0) return 0;
    int offset = 1;
    auto cp = str.data () + idx;
    while (--cp > str.data () && ((*cp & 0b10000000) && !(*cp & 0b01000000)))
      {
        offset++;
      }
    return idx - offset;
  }

  bool
  isspace (const char &c)
  {
    return c == ' ' || c == '\t';
  }

  bool
  isdelimiter (const char &c)
  {
    bool r = (c == '(' || c == ')' || c == '{' || c == '}' || c == '['
        || c == ']' || c == '\"');
    return r;
  }

  bool
  is_starting_word (const std::string &str, coord_t i)
  {
    if (i <= 0) return true;
    char c = str.at (i);
    char prev_c = str.at (i - 1);
    if ((isspace (prev_c) || isdelimiter (prev_c)) && !isspace (c)) return true;
    return false;
  }

  coord_t
  first_word (const std::string &str, coord_t idx)
  {
    int first_word_idx = 0;
    while (isspace (str.at (first_word_idx)) && first_word_idx < idx)
      {
        first_word_idx++;
      }
    if (first_word_idx == idx) // if we are already at the first word then got to the beginning of the line
    return 0;
    else return first_word_idx;
  }

  coord_t
  next_word (const std::string &str, coord_t idx)
  {
    int i = next_index (str, idx);
    while (i < str.length () && !is_starting_word (str, i))
      i = next_index (str, i);
    return i;
  }

  coord_t
  previous_word (const std::string &str, coord_t idx)
  {
    int i = previous_index (str, idx);
    while (i > 0 && !is_starting_word (str, i))
      {
        i = previous_index (str, i);
      }
    return i;
  }
}
