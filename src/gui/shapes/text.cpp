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
#include "../../core/coupling.h"
#include "shapes.h"
#include "../window.h"

namespace djnn
{

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
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _ctext = new Coupling (_text, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  Text::Text (Process *p, const std::string& n, double x, double y, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr)
  {
    init_text (x, y, text);
  }

  Text::Text (Process *p, const std::string& n, double x, double y, double dx, double dy, int dxu, int dyu,
              const std::string &encoding, const std::string &text) :
      AbstractGShape (p, n), _text (nullptr), _cx (nullptr), _cy (nullptr)
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
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _ctext = new Coupling (_text, ACTIVATION, update, ACTIVATION);
    Process::finalize ();
  }

  Text::Text (double x, double y, const std::string &text) :
      AbstractGShape (), _text (nullptr), _cx (nullptr), _cy (nullptr)
  {
    init_text (x, y, text);
  }

  Text::~Text ()
  {
    delete _cx;
    delete _cy;
    delete _ctext;
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
    _cx->enable (_frame);
    _cy->enable (_frame);
    _ctext->enable (_frame);
  }

  void
  Text::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _ctext->disable ();
  }

  void
  Text::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_text (this);
    }
  }
} /* namespace djnn */
