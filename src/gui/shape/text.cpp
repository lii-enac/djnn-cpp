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

#include "gui/shape/text.h"
#include "gui/transformation/homography.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/tree/component.h"
#include "display/window.h"

#include <iostream>
#include "utils/debug.h" // UNIMPL
#include "core/utils/error.h"

namespace djnn
{
  void
  Text::TextSizeAction::impl_activate ()
  {
    // DEBUG
    // cout << endl << "TEXT SIZE ACTION ---- text: " << _text->text ()->get_value ()
    //                                 << " - family: " << ( _ff ? _ff->family ()->get_value () : "NO_family")
    //                                 << " - size: " <<  (_fsz ? _fsz->size ()->get_value () : 12 ) 
    //                                 << " - style: " << ( _fs ? _fs->style ()->get_value () : 0 )
    //                                 << " - weight:" << ( _fw ? _fw->weight ()->get_value () : 400 ) << endl;
                                   
    Backend::instance ()->update_text_geometry (_text, _ff, _fsz, _fs, _fw);
  }

  Text::Text (ParentProcess* parent, const std::string& name, double x, double y, const std::string& text) :
  Text(parent, name, x,y,0,0,0,0,"utf8", text) {}

  Text::Text (ParentProcess* parent, const std::string& name, double x, double y, double dx, double dy, int dxU, int dyU,
              const std::string& encoding, const std::string& text) :
      AbstractGShape (parent, name),
      /* FIXME: encoding - should be string or convert in Int  from Enum? */
      raw_props{.x=x, .y=y, .dx=dx, .dy=dy, .dxU=dxU, .dyU=dyU, .encoding=DJN_UTF8, .text=text},
      _cx (nullptr), _cy (nullptr), _cdx (nullptr), _cdy (nullptr), _cfsize (nullptr), _cdxU (nullptr), _cdyU (nullptr),
      _cencoding (nullptr), _cfstyle (nullptr), _cfweight (nullptr), _cffamily (nullptr),
      _fm (nullptr),
      _update_size (this, "size_action", this),
      _width (this, "width", 0),
      _height (this, "height", 0),
      _text (this, "text", raw_props.text, notify_damaged_geometry),
      _cupdate_size (&_text, ACTIVATION, &_update_size, ACTIVATION, true),
      //_ctext (&_text, ACTIVATION, UpdateDrawing::instance ()->get_damaged (), ACTIVATION )
      _ctext (&_text, ACTIVATION, nullptr, ACTIVATION )
  {
    set_origin (x, y);

    //graph_add_edge (&_text, &_update_size);
    //Graph:instance ().add_edge (&_text, UpdateDrawing::instance ()->get_damaged ());
    Backend::instance ()->update_text_geometry (this, nullptr, nullptr, nullptr, nullptr); // try to init geometry on creation
    finalize_construction (parent, name);
  }

  Text::~Text ()
  {
    ////remove_state_dependency (get_parent (), UpdateDrawing::instance ()->get_damaged ());
    //graph_remove_edge (&_text, UpdateDrawing::instance ()->get_damaged ());
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_update_size);
    }
    //graph_remove_edge (&_text, &_update_size);
    
    delete _cffamily;
    delete _cfsize;
    delete _cfstyle;
    delete _cfweight;
    delete _cx;
    delete _cy;
    delete _cdx;
    delete _cdy;
    delete _cdxU;
    delete _cdyU;
    delete _cencoding;

    if (children_empty () == false) {
      symtable_t::iterator it;

      it = find_child_iterator ("x");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("y");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("dx");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("dy");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("dxU");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("dyU");
      if (it != children_end ())
        delete it->second;

      it = find_child_iterator ("encoding");
      if (it != children_end ())
        delete it->second;
    }

    Backend::instance ()->delete_text_impl (this);
  }

  void
  Text::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
       remove_state_dependency (get_parent (), &_update_size);
    }

    add_state_dependency (parent, &_update_size);
    
    FatProcess::set_parent (parent); 
  }

  FatChildProcess*
  Text::find_child_impl (const std::string& name)
  {
    auto * res = AbstractGShape::find_child_impl (name);
    if(res) return res;

    bool propd = false;
    bool propi = false;
    CouplingWithData ** coupling;
    double* rawpd;
    int* rawpi;
    int notify_mask = notify_none;

    if(name=="x") {
      propd = true;
      coupling=&_cx;
      rawpd=&raw_props.x;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y") {
      propd = true;
      coupling=&_cy;
      rawpd=&raw_props.y;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dx") {
      propd = true;
      coupling=&_cdx;
      rawpd=&raw_props.dx;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dy") {
      propd = true;
      coupling=&_cdy;
      rawpd=&raw_props.dy;
      notify_mask = notify_damaged_transform;
    } else 
    if(name=="dxU") {
      propi = true;
      coupling=&_cdxU;
      rawpi=&raw_props.dxU;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="dyU") {
      propi = true;
      coupling=&_cdyU;
      rawpi=&raw_props.dyU;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="encoding") {
      propi = true;
      coupling=&_cencoding;
      rawpi=&raw_props.encoding;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    if (propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpd, name, notify_mask);
    }
    else if (propi) {
      IntPropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpi, name, notify_mask);
    }
    
    return res;
  }

  void
  Text::get_properties_values (double &x, double &y, double &dx, double &dy, int &dxU, int &dyU, int &width, int &height, int &encoding, std::string &text)
  {
    x = raw_props.x;
    y = raw_props.y;
    dx = raw_props.dx;
    dy = raw_props.dy;
    dxU = raw_props.dxU;
    dyU = raw_props.dyU;
    width = _width.get_value ();
    height = _height.get_value ();
    encoding = raw_props.encoding;
    text = _text.get_value ();
  }

  void
  Text::impl_activate ()
  {
    AbstractGObj::impl_activate ();
    Container *c = dynamic_cast<Container*> (get_parent ());
    if (c) {
      
      /* fontfamily changed ? */
      FontFamily* ff =  (FontFamily*) c->get_from_context ("FontFamily");
      if (ff) {

        /* remove old coupling */
        if (_cffamily != nullptr) {
          delete _cffamily;
          _cffamily = nullptr;
        }

        /* update value */
        _update_size._ff = ff;
        _cffamily = new CouplingWithData (ff->family (), ACTIVATION, &_update_size, ACTIVATION);
      }

      /* fontsize changed ? */
      FontSize* fsz = (FontSize*) c->get_from_context ("FontSize");
      if (fsz) {
        
        /* remove old coupling */
        if (_cfsize != nullptr) {
          delete _cfsize;
          _cfsize = nullptr;
        }

        /* update value */
        _update_size._fsz = fsz;
        _cfsize = new CouplingWithData (fsz->size (), ACTIVATION, &_update_size, ACTIVATION);
      }

      /* fontstyle changed ? */
      FontStyle* fs = (FontStyle*) c->get_from_context ("FontStyle");
      if (fs) {
        
        /* remove old coupling */
        if (_cfstyle != nullptr) {
          delete _cfstyle;
          _cfstyle = nullptr;
        }

        /* update value */
        _update_size._fs = fs;
        _cfstyle = new CouplingWithData (fs->style (), ACTIVATION, &_update_size, ACTIVATION);
      }

      /* fontweight changed ? */
      FontWeight* fw = (FontWeight*) c->get_from_context ("FontWeight");
      if (fw) {
        
        /* remove old coupling */
        if (_cfweight != nullptr) {
          delete _cfweight;
          _cfweight = nullptr;
        }

        /* update value */
        _update_size._fw = fw;
        _cfweight = new CouplingWithData (fw->weight (), ACTIVATION, &_update_size, ACTIVATION);
      }
      Backend::instance ()->update_text_geometry (this, ff, fsz, fs, fw);
    } else {
      Backend::instance ()->update_text_geometry (this, nullptr, nullptr, nullptr, nullptr);
    }

    auto * damaged = get_frame ()->damaged ();

    enable (_cx, damaged);
    enable (_cy, damaged);
    enable (_cdx, damaged);
    enable (_cdy, damaged);
    enable (_cdxU, damaged);
    enable (_cdyU, damaged);
    enable (_cencoding, damaged);
    //enable (_cffamily, damaged);
    //enable (_cfsize, damaged);
    //enable (_cfstyle, damaged);
    //enable (_cfweight, damaged);
    enable (&_cupdate_size, damaged);
    enable (&_ctext, damaged);

    //_update_size.activate ();
  }

  void
  Text::impl_deactivate ()
  {
    //_ctext.disable();

    disable (_cx);
    disable (_cy);
    disable (_cdx);
    disable (_cdy);
    disable (_cdxU);
    disable (_cdyU);
    disable (_cencoding);
    // disable (_cffamily);
    // disable (_cfsize);
    // disable (_cfstyle);
    // disable (_cfweight);
    disable (&_cupdate_size);
    disable (&_ctext);

    /*if (_cx) _cx->disable ();
    if (_cy) _cy->disable ();
    //_ctext.disable ();
    if (_cdx) _cdx->disable ();
    if (_cdy) _cdy->disable ();
    if (_cdxU) _cdxU->disable ();
    if (_cdyU) _cdyU->disable ();
    _cupdate_size.disable ();
    if (_cencoding) _cencoding->disable ();*/

    /*if (_cffamily) _cffamily->disable ();
    if (_cfsize) _cfsize->disable ();
    if (_cfstyle) _cfstyle->disable ();
    if (_cfweight) _cfweight->disable ();*/
  
    AbstractGShape::impl_deactivate ();
  }

  void
  Text::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_text (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Text::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    UNIMPL;
  }

  double
  Text::sdf (double x, double y) const
  {
    UNIMPL;
    return 0;
  }

  double
  Text::get_cursor_from_index (int index)
  {
    return Backend::instance ()->get_cursor_from_index (this, index);
  }

  std::pair<double, int>
  Text::get_cursor_from_local_x (double pos)
  {
    return Backend::instance ()->get_cursor_from_local_x (this, pos);
  }

  Text*
  Text::clone ()
  {
    return new Text (nullptr, this->get_name (), raw_props.x, raw_props.y, _text.get_value ());
  }

  SimpleText::SimpleText (ParentProcess* parent, const std::string& name, double x, double y, const std::string& text) :
      AbstractGShape (parent, name), raw_props{.x=x, .y=y, .text=text},
      _text (this, "text", raw_props.text, notify_damaged_geometry),
      _cx (nullptr), _cy (nullptr),
      _ctext (&_text, ACTIVATION, nullptr, ACTIVATION ), _init (false)
  {
    SimpleTextEdit* ste = dynamic_cast<SimpleTextEdit*> (parent);
    if (ste == nullptr) {
      return;
    }
    _init = true;
    FatProcess::set_parent (ste->lines());
    ste->lines()->add_child (this, "");
    parent->add_symbol (name, this);
  }

  void
  SimpleText::set_parent (ParentProcess* parent)
  {
    if (_init)
      return;
    SimpleTextEdit* ste = dynamic_cast<SimpleTextEdit*> (parent);
    if (ste == nullptr) {
      error (this, "The Parent of SimpleText must be SimpleTextEdit");
    return;
    }
    _init = true;
    FatProcess::set_parent (ste->lines());
    ste->lines()->add_child (this, "");
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

  SimpleText::~SimpleText () {}

  FatChildProcess*
  SimpleText::find_child_impl (const std::string& name)
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
  SimpleText::get_properties_values (double &x, double &y, std::string &text)
  {
    x = raw_props.x;
    y = raw_props.y;
    text = _text.get_value ();
  }

  SimpleTextEdit::SimpleTextEdit (ParentProcess* parent, const std::string& name, int x, int y, int width, int height, const std::string &text, bool enable_edit_on_activation) :
    AbstractGShape (parent, name), _lines (this, "lines"),
    _cursor_start_x (this, "cursor_start_x", 0), _cursor_start_y (this, "cursor_start_y", 0),
    _cursor_end_x (this, "cursor_end_x", 0), _cursor_end_y (this, "cursor_end_y", 0),
    _cursor_height (this, "cursor_height", 16), _x (this, "x", x), _y(this, "y", y),
    _width (this, "width", width), _height (this, "height", height), _line_height (this, "line_height", 16), _spaces_for_tab (this, "spaces_for_tab", 0),
    _key_code_pressed (this, "key_pressed", 0), _key_code_released (this, "key_released", 0),
    _str_input (this, "string_input", ""), _copy_buffer (this, "copy_buffer", ""), _toggle_edit (this, "toggle_edit"), _line (nullptr),
    _toggle_action (this, "toggle_edit_action"), _on_press (this, "on_press_action"), _on_release (this, "on_release_action"), _on_move (this, "on_move_action"),
    _key_pressed (this, "key_pressed_action"),  _key_released (this, "key_released_action"),
    _on_str_input (this, "on_str_input_action"),
    _c_key_press (&_key_code_pressed, ACTIVATION, &_key_pressed, ACTIVATION),
    _c_key_release (&_key_code_released, ACTIVATION, &_key_released, ACTIVATION),
    _c_str_input (&_str_input, ACTIVATION, &_on_str_input, ACTIVATION),
    _c_press (), _c_release (), _c_move (),
    _c_x (&_x, ACTIVATION, nullptr, ACTIVATION),
    _c_y (&_y, ACTIVATION, nullptr, ACTIVATION),
    _c_toggle (&_toggle_edit, ACTIVATION, &_toggle_action, ACTIVATION),
    _font_metrics (nullptr), _ordering_node (),
    _index_x (0), _index_y (0), _ascent (0), _descent (0), _leading (0),
    _start_sel_x (0), _start_sel_y (0), _end_sel_x (0), _end_sel_y (0), _shift_on (false), _ctrl_on (false), _alt_on (false), _press_on (false),
    _enable_edit_on_activation (enable_edit_on_activation), _first_draw (true),
    _init_text (text)

  {
    init_ui();

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

    finalize_construction (parent, name);
  }

  SimpleTextEdit::~SimpleTextEdit()
  {
    graph_remove_edge (&_on_str_input, &_ordering_node);
    graph_remove_edge (&_on_press, &_ordering_node);
    graph_remove_edge (&_on_release, &_ordering_node);
    graph_remove_edge (&_on_move, &_ordering_node);
    graph_remove_edge (&_key_pressed, &_ordering_node);
    graph_remove_edge (&_key_released, &_ordering_node);

    graph_remove_edge (&_ordering_node, &_cursor_start_x);
    graph_remove_edge (&_ordering_node, &_cursor_end_x);
    graph_remove_edge (&_ordering_node, &_cursor_start_y);
    graph_remove_edge (&_ordering_node, &_cursor_end_y);
  }

  void
   SimpleTextEdit::impl_activate()
   {
     if (_line == nullptr) {
       if (_lines.children().empty()) {
         _line = new SimpleText (this, "", 0, 0, "");
         add_str (_init_text);
         _init_text = "";
         _index_y = _start_sel_y = _end_sel_y = 0;
         _index_x = _start_sel_x = _end_sel_x = 0;
         _line = get_line (0);
         update_cursor ();
       }
       else
         _line = get_line (0);
     }
     AbstractGShape::impl_activate ();
     _c_press.init (this->find_child("press"), ACTIVATION, &_on_press, ACTIVATION, false);
     _c_release.init (get_frame()->find_child("release"), ACTIVATION, &_on_release, ACTIVATION, false);
     _c_move.init (get_frame()->find_child("move"), ACTIVATION, &_on_move, ACTIVATION, false);
     _lines.activate ();

     _c_x.set_dst (get_frame()->damaged());
     _c_y.set_dst (get_frame()->damaged());
     _c_x.enable();
     _c_y.enable();
     if (!_enable_edit_on_activation) {
       _c_str_input.disable ();
       _c_key_press.disable ();
       _c_press.disable ();
       _c_release.disable ();
       _c_move.disable ();
     } else {
       _c_str_input.enable ();
       _c_key_press.enable ();
       _c_press.enable ();
       _c_release.enable ();
       _c_move.enable ();
     }
     _shift_on = _ctrl_on = _press_on = false;
   }

   void
   SimpleTextEdit::impl_deactivate()
   {
     AbstractGShape::impl_deactivate ();
     _lines.deactivate ();
     _c_x.disable();
     _c_y.disable();
     _c_key_press.disable ();
     _c_press.disable ();
     _c_release.disable ();
     _c_move.disable ();
   }

  static int
  next_index (const std::string &str, int idx)
  {
    if (idx > str.size ())
      return idx;
    int offset = 1;
    auto cp = str.data () + idx;
    while (++cp <= (str.data () + str.size ()) && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
     offset++;
    }
    return idx + offset;
  }

  static int
  previous_index (const std::string &str, int idx)
  {
    if (idx <= 0)
      return 0;
    int offset = 1;
    auto cp = str.data () + idx;
    while (--cp > str.data () && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {
     offset++;
    }
    return idx - offset;
  }

  static bool
  isspace (const char &c) {
    return c == ' ' || c == '\t';
  }

  static bool
  isdelimiter (const char &c) {
    bool r = (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '\"');
    return r;
  }

  static bool
  is_starting_word (const std::string& str, int i)
  {
    if (i <= 0)
      return true;
    char c = str.at(i);
    char prev_c = str.at(i - 1);
    if ( (isspace (prev_c) || isdelimiter (prev_c)) && !isspace (c))
      return true;
    return false;
  }

  static int
  first_word (const std::string &str, int idx)
  {
    int first_word_idx = 0;
    while (isspace (str.at (first_word_idx)) && first_word_idx < idx) {
      first_word_idx++;
    }
    if (first_word_idx == idx) // if we are already at the first word then got to the beginning of the line
      return 0;
    else
      return first_word_idx;
  }

  static int
  next_word (const std::string &str, int idx)
  {
    int i = next_index (str, idx);
    while (i < str.length() && !is_starting_word(str, i))
      i = next_index (str, i);
    return i;
  }

  static int
  previous_word (const std::string &str, int idx)
  {
    int i = previous_index (str, idx) ;
    while (i > 0 && !is_starting_word (str, i) ) {
      i = previous_index (str, i);
    }
    return i;
  }

  void
  SimpleTextEdit::toggle_edit ()
  {
    if (_c_press.is_enabled()) {
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
  SimpleTextEdit::update_selection ()
  {

  }

  void
  SimpleTextEdit::update_cursor ()
  {
    if (has_selection()) {
      SimpleText* l1 = get_line (_start_sel_y);
      SimpleText* l2 = get_line (_end_sel_y);
      _cursor_start_x.set_value (Backend::instance ()->compute_x (_font_metrics, l1, _start_sel_x), true);
      _cursor_start_y.set_value (l1->get_y (), true);
      _cursor_end_x.set_value (Backend::instance ()->compute_x (_font_metrics, l2, _end_sel_x), true);
      _cursor_end_y.set_value (l2->get_y (), true);
    } else {
      _cursor_start_x.set_value (Backend::instance ()->compute_x (_font_metrics, _line, _index_x), true);
      _cursor_start_y.set_value (_line->get_y (), true);
      _cursor_end_x.set_value (_cursor_start_x.get_value (), true);
      _cursor_end_y.set_value (_cursor_start_y.get_value (), true);
    }
  }

  int
  SimpleTextEdit::get_index_from_x (int x)
  {
    return Backend::instance ()->compute_index (_font_metrics, _line, x);
  }

  int
  SimpleTextEdit::get_x_from_index ()
  {
    return Backend::instance ()->compute_x (_font_metrics, _line, _index_x);
  }
  void
  SimpleTextEdit::update_index_from_xy (int x, int y)
  {
    if (_lines.children ().empty ()) {
      warning (this, "No more line in simple text editor");
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
      _index_x = _line->get_content().size();
      _index_y = _lines.children().size() - 1;
    } else {
      _index_x =  Backend::instance ()->compute_index (_font_metrics, _line, x);
    }
  }

  std::pair<double, double>
  SimpleTextEdit::get_local_coords (double x, double y)
  {
    /* compute local coords */
    Homography *h = dynamic_cast<Homography*> (this->inverted_matrix ());
    double loc_x = h->raw_props.m11 * x + h->raw_props.m12 * y + h->raw_props.m14
        - this->origin_x ()->get_value ();
    double loc_y = h->raw_props.m21 * x + h->raw_props.m22 * y  + h->raw_props.m24
        - this->origin_y ()->get_value ();
    return std::pair<double, double> (loc_x, loc_y);
  }

  void
  SimpleTextEdit::mouse_press ()
  {
    AbstractProperty* press_x = dynamic_cast<AbstractProperty*> (get_frame()->find_child ("press/x"));
    AbstractProperty* press_y = dynamic_cast<AbstractProperty*> (get_frame()->find_child ("press/y"));
    std::pair<double, double> local = get_local_coords(press_x->get_double_value(), press_y->get_double_value());
    int x = (int) local.first - this->x ();
    int y = (int) local.second - this->y ();
    update_index_from_xy (x, y);
    _start_sel_x = _end_sel_x = _index_x;
    _start_sel_y = _end_sel_y = _index_y;
    update_cursor ();
    _press_on = true;
  }

  void
  SimpleTextEdit::mouse_release ()
  {
    _press_on = false;
  }

  void
  SimpleTextEdit::mouse_move ()
  {
    if (!_press_on)
      return;
    AbstractProperty* move_x = dynamic_cast<AbstractProperty*> (get_frame()->find_child ("move/x"));
    AbstractProperty* move_y = dynamic_cast<AbstractProperty*> (get_frame()->find_child ("move/y"));
    std::pair<double, double> local = get_local_coords(move_x->get_double_value(), move_y->get_double_value());
    int x = (int) local.first - this->x ();
    int y = (int) local.second - this->y ();
    update_index_from_xy (x, y);
    _end_sel_x = _index_x;
    _end_sel_y = _index_y;
    update_cursor ();
  }

  void
  SimpleTextEdit::key_released ()
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
  SimpleTextEdit::key_pressed ()
  {
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
      std::string tab;
      if (_spaces_for_tab.get_value() == 0)
        tab = "\t";
      else {
        for (int i = 0; i < _spaces_for_tab.get_value(); i++)
          tab.append(" ");
      }
      add_str (tab);
      return;
    }
    if(key == DJN_Key_Return) {
      if (has_selection())
        del_selection();
      int height = _ascent + _descent;
      int y = (get_line(_index_y))->get_y() + height + _leading;
      SimpleText* buff_line = _line;
      //check if we are at the end of the line
      if (_index_x == _line->get_content().size())
        _line = new SimpleText (this, "", 0, y, "");
      else {
        //if not, cut the current line
        std::string cur_content = _line->get_content();
        std::string sub = cur_content.substr(_index_x);
        cur_content.erase(_index_x);
        _line->set_content(cur_content);
        _line = new SimpleText (this, "", 0, y, sub);
      }
      _lines.move_child(_line, AFTER, buff_line);
      _index_x = 0;
      _index_y++;
      _start_sel_x = _end_sel_x = 0;
      _start_sel_y = _end_sel_y = _index_y;
      _height.set_value(_height.get_value() + height + _leading, true);
      if (_index_y < _lines.children().size()) {
        update_lines_position();
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Left) {
      if (has_selection() && !_shift_on) {
        sort_selection ();
        _line = get_line (_start_sel_y);
        _index_x = _end_sel_x = _start_sel_x;
        _index_y = _end_sel_y = _start_sel_y;
        if (_alt_on) {
          _index_x = _end_sel_x = _start_sel_x = previous_word (_line->get_content (), _index_x);
        } else if (_ctrl_on && _index_x > 0){
            std::string str = _line->get_content ();
            _index_x = _end_sel_x = _start_sel_x = first_word (str, _index_x);
        }
      } else {
        if (_index_x > 0) {
          _index_x = _alt_on ? previous_word (_line->get_content (), _index_x) :
                        (_ctrl_on ? first_word (_line->get_content (), _index_x) : previous_index (_line->get_content (), _index_x));
        } else if (_index_y > 0) {
          _index_y--;
          _line = get_line (_index_y);
          _index_x = _line->get_content().size ();
        }
        if (_shift_on) {
          _end_sel_x = _index_x;
          _end_sel_y = _index_y;
        } else {
          _start_sel_x = _end_sel_x = _index_x;
          _start_sel_y = _end_sel_y = _index_y;
        }
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Right) {
      if (has_selection() && !_shift_on) {
        sort_selection ();
        _line = get_line (_end_sel_y);
        _index_x = _start_sel_x = _end_sel_x;
        _index_y = _start_sel_y = _end_sel_y;
        if (_alt_on) {
          _index_x = _end_sel_x = _start_sel_x = next_word (_line->get_content (), _index_x);
        } else if (_ctrl_on){
          _index_x = _end_sel_x = _start_sel_x = _line->get_content ().length();
        }
      } else {
        if (_index_x < _line->get_content().size ()) {
          _index_x = _alt_on ? next_word (_line->get_content (), _index_x) :
                        (_ctrl_on ? _line->get_content ().length() : next_index (_line->get_content(), _index_x));
        } else if (_index_y < _lines.children().size () - 1) {
          _index_y++;
          _line = get_line (_index_y);
          _index_x = 0;
        }
        if (_shift_on) {
          _end_sel_x = _index_x;
          _end_sel_y = _index_y;
        } else {
          _start_sel_x = _end_sel_x = _index_x;
          _start_sel_y = _end_sel_y = _index_y;
        }
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Up) {
      bool moved = false;
      int x_layout = get_x_from_index ();
      if (_index_y > 0) {
        _index_y--;
        moved = true;
      }
      _line = get_line (_index_y);

      if (_index_x > _line->get_content().size () && moved)
        _index_x = _line->get_content().size ();
      else if (moved)
         _index_x = get_index_from_x (x_layout);
      else
        _index_x = 0;

      if (_shift_on) {
        _end_sel_x = _index_x;
        _end_sel_y = _index_y;
      } else {
        _end_sel_x = _start_sel_x = _index_x;
        _end_sel_y = _start_sel_y = _index_y;
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Down) {
      bool moved = false;
      int x_layout = get_x_from_index ();
      if (_index_y < _lines.children().size() - 1) {
        _index_y++;
        moved = true;
      }
      _line = get_line (_index_y);
      if (_index_x > _line->get_content().size () || !moved)
        _index_x = _line->get_content().size ();
      else
        _index_x = get_index_from_x (x_layout);
      if (_shift_on) {
        _end_sel_x = _index_x;
        _end_sel_y = _index_y;
      } else {
        _end_sel_x = _start_sel_x = _index_x;
        _end_sel_y = _start_sel_y = _index_y;
      }
      update_cursor ();
      return;
    }
    if (key == DJN_Key_Backspace) {
      if (has_selection()) {
        del_selection();
      } else {
        if (_index_x == 0) {
          if (_index_y > 0) {
            SimpleText* prev_line = get_line(_index_y - 1);
            std::string content = prev_line->get_content() + _line->get_content();
            _end_sel_x = _start_sel_x = _index_x =  prev_line->get_content().size();
            prev_line->set_content (content);
            _end_sel_y = _start_sel_y = _index_y-1;
            _index_y--;
            delete_line (_line);
            _line = prev_line;
            update_lines_position();
          }
        } else {
          std::string str = _line->get_content ();
          int prev_idx = previous_index (str, _index_x);
          int l = _index_x - prev_idx;
          str.erase (prev_idx, l);
          _start_sel_x = _end_sel_x = _index_x = prev_idx;
          _line->set_content(str);
        }
      }
      update_cursor();
      return;
    }
    if (key == DJN_Key_Delete) {
      if (has_selection()) {
        del_selection();
      } else {
        if (_index_x == _line->get_content().size()) {
          if (_index_y < _lines.children().size()) {
            SimpleText* next_line = get_line(_index_y + 1);
            std::string content =  _line->get_content() + next_line->get_content();
            _end_sel_x = _start_sel_x = _index_x;
            _line->set_content (content);
            _end_sel_y = _start_sel_y = _index_y;
            delete_line (next_line);
            update_lines_position();
          }
        } else {
          std::string str = _line->get_content ();
          int next_idx = next_index (str, _index_x);
          int l = next_idx - _index_x;
          str.erase (_index_x, l);
          _line->set_content(str);
        }
      }
      update_cursor();
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
      del_selection();
      return;
    }
  }

  void
  SimpleTextEdit::sort_selection ()
  {
    if (_start_sel_y == _end_sel_y) {
      if (_start_sel_x > _end_sel_x) {
        int buff = _start_sel_x;
        _start_sel_x = _end_sel_x;
        _end_sel_x = buff;
      }
    } else if (_start_sel_y > _end_sel_y ){
        int buff_x = _start_sel_x;
        int buff_y = _start_sel_y;
        _start_sel_x = _end_sel_x;
        _start_sel_y = _end_sel_y;
        _end_sel_x = buff_x;
        _end_sel_y = buff_y;
    }
  }

  void
  SimpleTextEdit::delete_line (SimpleText *st)
  {
    st->deactivate ();
    _lines.remove_child (st);
    delete st;
  }

  void
  SimpleTextEdit::del_selection ()
  {
    sort_selection ();

    if (_start_sel_y == _end_sel_y) {
      // Single line selection
      std::string cur_text = _line->get_content();
      cur_text.erase(_start_sel_x, _end_sel_x - _start_sel_x);
      _line->set_content (cur_text);
    } else {
      // Multi-line selection
      std::vector<SimpleText*> to_delete;
      SimpleText *start_line = (SimpleText*) _lines.children ().at(_start_sel_y);
      SimpleText *end_line = (SimpleText*)_lines.children ().at(_end_sel_y);
      std::string start_text = start_line->get_content();
      start_text.erase(_start_sel_x);
      std::string end_text = end_line->get_content();
      end_text.erase (0, _end_sel_x);
      start_text.append (end_text);
      start_line->set_content (start_text);

      for (int i = _start_sel_y + 1; i <= _end_sel_y; i++) {
        to_delete.push_back ((SimpleText*)_lines.children ().at(i));
      }
      for (auto p : to_delete) {
        delete_line (p);
      }
    }
    _index_y = _end_sel_y = _start_sel_y;
    _index_x = _end_sel_x = _start_sel_x;
    _line = (SimpleText*)_lines.children ().at(_index_y);
    update_lines_position ();
    update_cursor ();
  }

  void
  SimpleTextEdit::update_lines_position ()
  {
    int height = _ascent + _descent;
    for (int i = 0; i < _lines.children ().size (); i++) {
      ((DoubleProperty*)(_lines.children().at (i)->find_child("y")))->set_value( i * (height + _leading), true);
    }
  }

  void
  SimpleTextEdit::add_string_input ()
  {
    std::string str = _str_input.get_value ();
    if (str.empty())
      return;
    add_str (str);
  }

  void
  SimpleTextEdit::add_str (const std::string& str)
  {
    if (str.empty())
      return;
    std::string cur_text = _line->get_content();
    std::string cpy = str;
    if (has_selection())
      del_selection ();

    int off_y = _ascent + _descent + _leading;
    std::size_t found = cpy.find("\n");

    if (_index_x == cur_text.length ()) {
      if (found == std::string::npos) {
        cur_text.append (cpy);
        _index_x = _start_sel_x = _end_sel_x = cur_text.length ();
        _line->set_content (cur_text);
      } else {
        cur_text.append (cpy.substr (0, found));
        _line->set_content (cur_text);
        cpy = cpy.substr(found + 1);
        found = cpy.find("\n");
        int y = (get_line(_index_y))->get_y() + off_y;
        while (found != std::string::npos) {
          SimpleText* buff = new SimpleText (this, "", 0, y, cpy.substr (0, found));
          _lines.move_child(buff, AFTER, _line);
          _line = buff;
          cpy = cpy.substr(found + 1);
          found = cpy.find("\n");
          y += off_y;
          _index_y++;
        }
        SimpleText* buff = new SimpleText (this, "", 0, y, cpy);
        _lines.move_child(buff, AFTER, _line);
        _line = buff;
        _index_y++;
        _index_x = _line->get_content ().size ();
        update_lines_position ();
      }
    } else {
      if (found == std::string::npos) {
        cur_text.insert (_index_x, cpy);
        _index_x = _start_sel_x = _end_sel_x = _index_x + cpy.length ();
        _line->set_content (cur_text);
      } else {
        std::string start = cur_text.substr (0, _index_x);
        std::string end = cur_text.substr(_index_x);
        cur_text.erase (_index_x);
        cur_text.insert (_index_x, cpy.substr (0, found));
        _line->set_content (cur_text);
        cpy = cpy.substr(found + 1);
        found = cpy.find("\n");
        int y = (get_line(_index_y))->get_y() + off_y;
        while (found != std::string::npos) {
          SimpleText* buff = new SimpleText (this, "", 0, y, cpy.substr (0, found));
          _lines.move_child(buff, AFTER, _line);
          _line = buff;
          cpy = cpy.substr(found + 1);
          found = cpy.find("\n");
          y += off_y;
          _index_y++;
        }
        cpy.append(end);
        SimpleText* buff = new SimpleText (this, "", 0, y, cpy);
        _lines.move_child(buff, AFTER, _line);
        _line = buff;
        _index_y++;
        _index_x = _line->get_content ().size ();
        update_lines_position ();
      }
    }
    update_cursor ();
  }

  void
  SimpleTextEdit::copy ()
  {
    if (_start_sel_x == _end_sel_x && _start_sel_y == _end_sel_y)
      return;
    sort_selection();
    std::string content = (get_line (_start_sel_y))->get_content();
    int length_init_sel = _end_sel_y == _start_sel_y ? _end_sel_x - _start_sel_x: content.length() - _start_sel_x;
    std::string str = content.substr(_start_sel_x, length_init_sel);

    if (_end_sel_y != _start_sel_y) {
      str.append ("\n");
      int i = _start_sel_y + 1;
      while (i < _end_sel_y) {
        str.append ((get_line (i++))->get_content());
        str.append ("\n");
      }
      str.append ((get_line (_end_sel_y))->get_content().substr(0, _end_sel_x));
    }
    _copy_buffer.set_value (str, true);
  }

  void
  SimpleTextEdit::paste ()
  {
    std::string str = _copy_buffer.get_value();
    add_str (str);
  }

  void
  SimpleTextEdit::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_simple_text_edit (this);
      _first_draw = false;
      _cursor_height.set_value (_ascent + _descent, true);
      _line_height.set_value (_ascent + _descent + _leading, true);
      AbstractGShape::post_draw ();
    }
  }
} /* namespace djnn */
