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
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cwidth->disable ();
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _cheight->disable ();
    _crx = new Coupling (_rx, ACTIVATION, update, ACTIVATION);
    _crx->disable ();
    _cry = new Coupling (_ry, ACTIVATION, update, ACTIVATION);
    _cry->disable ();
    set_origin (x, y);
    Process::finalize ();
  }

  Rectangle::Rectangle (Process *p, const std::string& n, double x, double y, double w, double h, double rx = 0,
                        double ry = 0) :
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
    if (_cx) {delete _cx; _cx = nullptr;};
    if (_cy) {delete _cy; _cy = nullptr;};
    if (_cwidth) {delete _cwidth; _cwidth = nullptr;};
    if (_cheight) {delete _cheight; _cheight = nullptr;};
    if (_crx) {delete _crx; _crx = nullptr;};
    if (_cry) {delete _cry; _cry = nullptr;};

    if (_x) {delete _x; _x = nullptr;};
    if (_y) {delete _y; _y = nullptr;};
    if (_width) {delete _width; _width = nullptr;};
    if (_height) {delete _height; _height = nullptr;};
    if (_rx) {delete _rx; _rx = nullptr;};
    if (_ry) {delete _ry; _ry = nullptr;};
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

  Process*
  Rectangle::clone ()
  {
    return new Rectangle (_x->get_value (), _y->get_value (), 
                          _width->get_value (), _height->get_value (), 
                          _rx->get_value (), _ry->get_value ());
  }
} /* namespace djnn */
