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

namespace djnn
{

  RectangleClip::RectangleClip (Process *p, const std::string& n, double x, double y, double w, double h) :
      AbstractGShape (p, n)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cwidth->disable ();
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _cheight->disable ();
    set_origin (x, y);
    Process::finalize ();
  }

  RectangleClip::RectangleClip (double x, double y, double w, double h) :
      AbstractGShape ()
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    Process *update = UpdateDrawing::instance ()->get_damaged ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cwidth->disable ();
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _cheight->disable ();
  }

  RectangleClip::~RectangleClip ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_cwidth) {delete _cwidth; _cwidth = nullptr;}
    if (_cheight) {delete _cheight; _cheight = nullptr;}
    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_width) {delete _width; _width = nullptr;}
    if (_height) {delete _height; _height = nullptr;}
  }

  void
  RectangleClip::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _cwidth->enable (_frame);
    _cheight->enable (_frame);
  }

  void
  RectangleClip::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _cwidth->disable ();
    _cheight->disable ();
  }

  void
  RectangleClip::draw ()
  {
    //if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      double x = _x->get_value ();
      double y = _y->get_value ();
      double w = _width->get_value ();
      double h = _height->get_value ();
      Backend::instance ()->draw_rect_clip (this, x, y, w, h);
    }
  }

  Process*
  RectangleClip::clone() {
    return new RectangleClip (_x->get_value (), _y->get_value (), _width->get_value (), _height->get_value ());
  }
} /* namespace djnn */
