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

#include "../backend.h"
#include "../abstract_backend.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{

  void
  Rectangle::init_rectangle (double x, double y, double w, double h, double rx, double ry)
  {
    _x = new DoubleProperty (this, "x", x, notify_damaged_transform);
    _y = new DoubleProperty (this, "y", y, notify_damaged_transform);
    _width = new DoubleProperty (this, "width", w, notify_damaged_geometry);
    _height = new DoubleProperty (this, "height", h, notify_damaged_geometry);
    _rx = new DoubleProperty (this, "rx", rx, notify_damaged_geometry);
    _ry = new DoubleProperty (this, "ry", ry, notify_damaged_geometry);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
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
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_rect (this);
    }
  }

  void
  Rectangle::get_properties_values (double &x, double &y, double &w, double &h, double &rx, double &ry)
  {
    x = _x->get_value ();
    y = _y->get_value ();
    w = _width->get_value ();
    h = _height->get_value ();
    rx = _rx->get_value ();
    ry = _ry->get_value ();
  }

  Process*
  Rectangle::clone ()
  {
    return new Rectangle (_x->get_value (), _y->get_value (), 
                          _width->get_value (), _height->get_value (), 
                          _rx->get_value (), _ry->get_value ());
  }
} /* namespace djnn */
