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
  Image::Image (const std::string& path, double x, double y, double w, double h) :
      _invalid_cache (true)
  {
    _x = new DoubleProperty (this, "x", x);
    _y = new DoubleProperty (this, "y", y);
    _width = new DoubleProperty (this, "width", w);
    _height = new DoubleProperty (this, "height", h);
    _path = new TextProperty (this, "path", path);
    set_cache (nullptr);
    UpdateDrawing *update = UpdateDrawing::instance ();
    _cx = new Coupling (_x, ACTIVATION, update, ACTIVATION);
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cwidth->disable ();
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _cheight->disable ();
    _watcher = new ImageWatcher (this);
    _cpath = new Coupling (_path, ACTIVATION, _watcher, ACTIVATION);
    _cpath->disable ();
    set_origin (x, y);
  }

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
    _cx->disable ();
    _cy = new Coupling (_y, ACTIVATION, update, ACTIVATION);
    _cy->disable ();
    _cwidth = new Coupling (_width, ACTIVATION, update, ACTIVATION);
    _cwidth->disable ();
    _cheight = new Coupling (_height, ACTIVATION, update, ACTIVATION);
    _cheight->disable ();
    _watcher = new ImageWatcher (this);
    _cpath = new Coupling (_path, ACTIVATION, _watcher, ACTIVATION);
    _cpath->disable ();
    set_origin (x, y);
    Process::finalize ();
  }

  Image::~Image ()
  {
    if (_cx) {delete _cx; _cx = nullptr;}
    if (_cy) {delete _cy; _cy = nullptr;}
    if (_cwidth) {delete _cwidth; _cwidth = nullptr;}
    if (_cheight) {delete _cheight; _cheight = nullptr;}
    if (_cpath) {delete _cpath; _cpath = nullptr;}
    if (_watcher) {delete _watcher; _watcher = nullptr;}
    if (_x) {delete _x; _x = nullptr;}
    if (_y) {delete _y; _y = nullptr;}
    if (_width) {delete _width; _width = nullptr;}
    if (_height) {delete _height; _height = nullptr;}
    if (_path) {delete _path; _path = nullptr;}
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

  Process*
  Image::clone () 
  {
    return new Image (_path->get_value (), _x->get_value (), _y->get_value (), _width->get_value (), _height->get_value ());
  } 
} /* namespace djnn */
