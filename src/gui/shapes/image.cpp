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

  Image::Image (Process *p, const std::string& n, const std::string& path, double x, double y, double w,
		double h) :
      AbstractGShape (p, n), _invalid_cache (true)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    _path = new TextProperty (this, "path", path);
    set_cache (nullptr);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _watcher = new ImageWatcher (this);
    _cpath = new Coupling (_path, ACTIVATION, _watcher, ACTIVATION);
    set_origin (x, y);
    Process::finalize ();
  }

  Image::~Image ()
  {
    delete _cx;
    delete _cy;
    delete _cwidth;
    delete _cheight;
    delete _cpath;
    delete _watcher;
    delete _x;
    delete _y;
    delete _width;
    delete _height;
    delete _path;
  }

  void
  Image::activate ()
  {
    AbstractGObj::activate ();
    _cx->enable (_frame);
    _cy->enable (_frame);
    _cwidth->enable (_frame);
    _cheight->enable (_frame);
    _cpath->enable (_frame);
  }

  void
  Image::deactivate ()
  {
    AbstractGObj::deactivate ();
    _cx->disable ();
    _cy->disable ();
    _cwidth->disable ();
    _cheight->disable ();
    _cpath->disable ();
  }

  void
  Image::draw ()
  {
    if (_activation_state <= activated && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_image (this);
    }
  }
} /* namespace djnn */
