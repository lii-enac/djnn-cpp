/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/control/coupling.h"
#include "shapes.h"
#include "../window.h"

namespace djnn
{
  shared_ptr<TextContext>
  TextContextManager::get_current ()
  {
    return _context_list.back ();
  }
  void
  TextContextManager::push ()
  {
    if (_context_list.empty ())
      _context_list.push_back (make_shared<TextContext> ());
    else {
      _context_list.push_back (make_shared<TextContext> (_context_list.back ()));
    }
  }

  void
  TextContextManager::pop ()
  {
    _context_list.pop_back ();
  }

  TextContext::TextContext () :
      _font_family (nullptr), _font_size (nullptr), _font_style (nullptr), _font_weight (nullptr)
  {
  }

  TextContext::TextContext (shared_ptr<TextContext> context)
  {
    _font_family = context->_font_family;
    _font_size = context->_font_size;
    _font_style = context->_font_style;
    _font_weight = context->_font_weight;
  }

  void
  Text::TextSizeAction::activate ()
  {
    Backend::instance ()->update_text_geometry (_text, _ff, _fsz, _fs, _fw);
  }

  void
  Text::init_text (double x, double y, const std::string &text)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _dx = new DoubleProperty (this, "dx", 0);
    _dy = new DoubleProperty (this, "dy", 0);
    _dxU = new IntProperty (this, "dxU", 0);
    _dyU = new IntProperty (this, "dyU", 0);
    _width = new IntProperty (this, "width", 0);
    _height = new IntProperty (this, "height", 0);
    _encoding = new IntProperty (this, "encoding", djnUtf8);
    _text = new TextProperty (this, "text", text);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _update_size = new TextSizeAction (this, "size_action", this);
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _ctext = new Coupling (_text, ACTIVATION, update, ACTIVATION);
    _ctext_size = new Coupling (_text, ACTIVATION, _update_size, ACTIVATION);
    set_origin (x, y);
    Graph::instance ().add_edge (_text, _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
    Process::finalize ();
  }

  Text::Text (Process *p, const std::string& n, double x, double y, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr), _cffamily (nullptr), _cfsize (nullptr), _cfstyle (
          nullptr), _cfweight (nullptr), _ffamily (nullptr), _fsize (nullptr), _fstyle (nullptr), _fweight (nullptr)
  {
    init_text (x, y, text);
  }

  Text::Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr), _cffamily (nullptr), _cfsize (nullptr), _cfstyle (
          nullptr), _cfweight (nullptr), _ffamily (nullptr), _fsize (nullptr), _fstyle (nullptr), _fweight (nullptr)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _dx = new DoubleProperty (this, "dx", dx);
    _dy = new DoubleProperty (this, "dy", dy);
    _dxU = new IntProperty (this, "dxU", dxu);
    _dyU = new IntProperty (this, "dyU", dyu);
    _width = new IntProperty (this, "width", 0);
    _height = new IntProperty (this, "height", 0);
    djnTextEncoding code = djnUtf8;
    if (encoding.compare ("ISO-8859-1") == 0)
      code = djnLatin1;
    else if (encoding.compare ("ASCII") == 0)
      code = djnAscii;
    _encoding = new IntProperty (this, "encoding", code);
    _text = new TextProperty (this, "text", text);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _update_size = new TextSizeAction (this, "size_action", this);
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _ctext = new Coupling (_text, ACTIVATION, update, ACTIVATION);
    _ctext_size = new Coupling (_text, ACTIVATION, _update_size, ACTIVATION);
    Graph::instance ().add_edge (_text, _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
    Process::finalize ();
  }

  Text::Text (double x, double y, const std::string &text) :
      AbstractGShape (), _text (nullptr), _cx (nullptr), _cy (nullptr)
  {
    init_text (x, y, text);
  }

  Text::~Text ()
  {
    Graph::instance ().remove_edge (_text, _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _update_size);
    delete _cx;
    delete _cy;
    delete _ctext;
    delete _ctext_size;
    if (_cffamily) {
      Graph::instance ().remove_edge (_ffamily, _update_size);
      delete _cffamily;
    }
    if (_cfsize) {
      Graph::instance ().remove_edge (_fsize, _update_size);
      delete _cfsize;
    }
    if (_cfstyle) {
      Graph::instance ().remove_edge (_fstyle, _update_size);
      delete _cfstyle;
    }
    if (_cfweight) {
      Graph::instance ().remove_edge (_fweight, _update_size);
      delete _cfweight;
    }
    delete _x;
    delete _y;
    delete _text;
    delete _dx;
    delete _dy;
    delete _dxU;
    delete _dyU;
    delete _width;
    delete _height;
    delete _encoding;
  }

  void
  Text::activate ()
  {
    AbstractGObj::activate ();
    shared_ptr<TextContext> tc = Backend::instance ()->get_text_context_manager ()->get_current ();
    if (!_cffamily && tc->font_family ()) {
      _ffamily = tc->font_family ()->family ();
      _cffamily = new Coupling (_ffamily, ACTIVATION, _update_size, ACTIVATION);
      _update_size->_ff = tc->font_family ();
      Graph::instance ().add_edge (_ffamily, _update_size);
    }
    if (!_cfsize && tc->font_size ()) {
      _fsize = tc->font_size ()->size ();
      _cfsize = new Coupling (_fsize, ACTIVATION, _update_size, ACTIVATION);
      _update_size->_fsz = tc->font_size ();
      Graph::instance ().add_edge (_fsize, _update_size);
    }
    if (!_cfstyle && tc->font_style ()) {
      _fstyle = tc->font_style ()->style ();
      _cfstyle = new Coupling (_fstyle, ACTIVATION, _update_size, ACTIVATION);
      _update_size->_fs = tc->font_style ();
      Graph::instance ().add_edge (_fstyle, _update_size);
    }
    if (!_cfweight && tc->font_weight ()) {
      _fweight = tc->font_weight ()->weight ();
      _cfweight = new Coupling (_fweight, ACTIVATION, _update_size, ACTIVATION);
      _update_size->_fw = tc->font_weight ();
      Graph::instance ().add_edge (_fweight, _update_size);
    }
    _cx->enable (_frame);
    _cy->enable (_frame);
    _ctext->enable (_frame);
    if (_cffamily)
      _cffamily->enable ();
    if (_cfsize)
      _cfsize->enable ();
    if (_cfstyle)
      _cfstyle->enable ();
    if (_cfweight)
      _cfweight->enable ();
    _ctext_size->enable ();
  }

  void
  Text::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _ctext->disable ();
    if (_cffamily)
      _cffamily->disable ();
    if (_cfsize)
      _cfsize->disable ();
    if (_cfstyle)
      _cfstyle->disable ();
    if (_cfweight)
      _cfweight->disable ();
    _ctext_size->disable ();
  }

  void
  Text::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_text (this);
    }
  }
} /* namespace djnn */
