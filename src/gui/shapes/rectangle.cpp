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
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{

  void
  Rectangle::init_rectangle (double x, double y, double w, double h, double rx, double ry)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    _rx = new DoubleProperty (this, "rx", rx);
    _ry = new DoubleProperty (this, "ry", ry);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    set_origin (x, y);
    Process::finalize ();
  }

  Rectangle::Rectangle (Process *p, const std::string& n, double x, double y, double w, double h, double rx,
                        double ry) :
      AbstractGShape (p, n), _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (
          nullptr)
  {
    init_rectangle (x, y, w, h, rx, ry);
  }

  Rectangle::Rectangle (double x, double y, double w, double h, double rx = 0, double ry = 0) :
      AbstractGShape (), _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _crx (nullptr), _cry (
          nullptr)
  {
    init_rectangle (x, y, w, h, rx, ry);
  }

  Rectangle::~Rectangle ()
  {
    delete _cx;
    delete _cy;
    delete _cwidth;
    delete _cheight;
    delete _crx;
    delete _cry;

    delete _x;
    delete _y;
    delete _width;
    delete _height;
    delete _rx;
    delete _ry;
  }

  void
  Rectangle::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _cwidth->enable (_frame);
    _cheight->enable (_frame);
    _crx->enable (_frame);
    _cry->enable (_frame);
  }

  void
  Rectangle::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _cwidth->disable ();
    _cheight->disable ();
    _crx->disable ();
    _cry->disable ();
  }

  void
  Rectangle::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      double x = _x->get_value ();
      double y = _y->get_value ();
      double w = _width->get_value ();
      double h = _height->get_value ();
      double rx = _rx->get_value ();
      double ry = _ry->get_value ();
      Backend::instance ()->draw_rect (this, x, y, w, h, rx, ry);
    }
  }
} /* namespace djnn */
