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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "../../core/control/coupling.h"
#include "shapes.h"
#include "../window.h"

namespace djnn
{
  void
  Text::TextSizeAction::activate ()
  {
    Backend::instance ()->update_text_geometry (_text, _ff, _fsz, _fs, _fw);
  }

  void
  Text::init_text (double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text)
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
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _update_size = new TextSizeAction (this, "size_action", this);
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _ctext = new Coupling (_text, ACTIVATION, update, ACTIVATION);
    _ctext->disable ();
    _ctext_size = new Coupling (_text, ACTIVATION, _update_size, ACTIVATION);
    _ctext_size->disable ();
    set_origin (x, y);
    Graph::instance ().add_edge (_text, _update_size);
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().add_edge (_parent->state_dependency (), _update_size);
  }

  Text::Text (Process *p, const std::string& n, double x, double y, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr), _cffamily (nullptr), _cfsize (nullptr), _cfstyle (
          nullptr), _cfweight (nullptr), _ffamily (nullptr), _fsize (nullptr), _fstyle (nullptr), _fweight (nullptr)
  {
    init_text (x, y, 0, 0, 0, 0, "", text);
    Process::finalize ();
  }

  Text::Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr), _cffamily (nullptr), _cfsize (nullptr), _cfstyle (
          nullptr), _cfweight (nullptr), _ffamily (nullptr), _fsize (nullptr), _fstyle (nullptr), _fweight (nullptr)
  {
    init_text (x, y, dx, dy, dxu, dyu, encoding, text);
    Process::finalize ();
  }

  Text::Text (double x, double y, const std::string &text) :
      AbstractGShape (), _text (nullptr), _cx (nullptr), _cy (nullptr), _cffamily (nullptr), _cfsize (nullptr), _cfstyle (
          nullptr), _cfweight (nullptr), _ffamily (nullptr), _fsize (nullptr), _fstyle (nullptr), _fweight (nullptr)
  {
    init_text (x, y, 0, 0, 0, 0, "", text);
  }

  Text::~Text ()
  {
    
    if (_parent && _parent->state_dependency () != nullptr)
      Graph::instance ().remove_edge (_parent->state_dependency (), _update_size);
    Graph::instance ().remove_edge (_text, _update_size);
    
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_ctext) {delete _ctext; _ctext = nullptr;}
    if (_ctext_size) {delete _ctext_size; _ctext_size = nullptr;}

    if (_cffamily) {
      Graph::instance ().remove_edge (_ffamily, _update_size);
      delete _cffamily;
      _cffamily = nullptr;
    }
    if (_cfsize) {
      Graph::instance ().remove_edge (_fsize, _update_size);
      delete _cfsize;
      _cfsize = nullptr;
    }
    if (_cfstyle) {
      Graph::instance ().remove_edge (_fstyle, _update_size);
      delete _cfstyle;
      _cfstyle = nullptr;
    }
    if (_cfweight) {
      Graph::instance ().remove_edge (_fweight, _update_size);
      delete _cfweight;
      _cfweight = nullptr;
    }

    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_text) {delete _text; _text = nullptr;}
    if (_dx) {delete _dx; _dx = nullptr;}
    if (_dy) {delete _dy; _dy = nullptr;}
    if (_dxU) {delete _dxU; _dxU = nullptr;}
    if (_dyU) {delete _dyU; _dyU = nullptr;}
    if (_width) {delete _width; _width = nullptr;}
    if (_height) {delete _height; _height = nullptr;}
    if (_encoding) {delete _encoding; _encoding = nullptr;}
  
  }

  void
  Text::activate ()
  {
    AbstractGObj::activate ();
    Container *c = dynamic_cast<Container*> (_parent);
    if (c) {
      Process* ff = c->get_from_context ("FontFamily");
      if (ff && ((FontFamily*) ff)->family () != _ffamily) {
        _ffamily = ((FontFamily*) ff)->family ();
        _update_size->_ff = (FontFamily*) ff;
        if (_cffamily != nullptr) {
          Graph::instance ().remove_edge (_ffamily, _update_size);
          delete _cffamily;
          _cffamily = nullptr;
        }
        if (_ffamily != nullptr) {
          _cffamily = new Coupling (_ffamily, ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (_ffamily, _update_size);
        }
      }
      Process* fsz = c->get_from_context ("FontSize");
      if (fsz && ((FontSize*) fsz)->size () != _fsize) {
        _fsize = ((FontSize*) fsz)->size ();
        _update_size->_fsz = (FontSize*) fsz;
        if (_cfsize != nullptr) {
          Graph::instance ().remove_edge (_fsize, _update_size);
          delete _cfsize;
          _cfsize = nullptr;
        }
        if (_fsize != nullptr) {
          _cfsize = new Coupling (_fsize, ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (_fsize, _update_size);
        }
      }
      Process* fs = c->get_from_context ("FontStyle");
      if (fs && ((FontStyle*) fs)->style () != _fstyle) {
        _fstyle = ((FontStyle*) fs)->style ();
        _update_size->_fs = (FontStyle*) fs;
        if (_cfstyle != nullptr) {
          Graph::instance ().remove_edge (_fstyle, _update_size);
          delete _cfstyle;
          _cfstyle = nullptr;
        }
        if (_fstyle != nullptr) {
          _cfstyle = new Coupling (_fstyle, ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (_fstyle, _update_size);
        }
      }
      Process* fw = c->get_from_context ("FontWeight");
      if (fw && ((FontWeight*) fw)->weight () != _fweight) {
        _fweight = ((FontWeight*) fw)->weight ();
        _update_size->_fw = (FontWeight*) fw;
        if (_cfweight != nullptr) {
          Graph::instance ().remove_edge (_fweight, _update_size);
          delete _cfweight;
          _cfweight = nullptr;
        }
        if (_fweight != nullptr) {
          _cfweight = new Coupling (_fweight, ACTIVATION, _update_size, ACTIVATION);
          Graph::instance ().add_edge (_fweight, _update_size);
        }
      }
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
    _update_size->activation ();
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
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
        Backend::instance ()->draw_text (this);
    }
  }

  Process*
  Text::clone ()
  {
    return new Text ( _x->get_value (), _y->get_value (), _text->get_value ());
  } 
} /* namespace djnn */
