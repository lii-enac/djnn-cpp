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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "../backend.h"
#include "../abstract_backend.h"
#include "shapes.h"
#include "../../core/control/coupling.h"

namespace djnn
{
  Image::Image (const std::string& path, double x, double y, double w, double h) :
      AbstractGShape (),
      raw_props{.x=x, .y=y, .width=w, .height=h, .path=path},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _cpath (nullptr),
      _invalid_cache (true)
  {
    set_origin (x, y);
  }

  Image::Image (Process *p, const std::string& n, const std::string& path, double x, double y, double w,
		double h) :
      AbstractGShape (p, n),
      raw_props{.x=x, .y=y, .width=w, .height=h, .path=path},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _cpath (nullptr),
      _invalid_cache (true)
  {
    set_cache (nullptr);
    set_origin (x, y);
    Process::finalize ();
  }

  Image::~Image ()
  {
    delete _cx; _cx = nullptr;
    delete _cy; _cy = nullptr;
    delete _cwidth; _cwidth = nullptr;
    delete _cheight; _cheight = nullptr;
    delete _cpath; _cpath = nullptr;
    delete _watcher; _watcher = nullptr;
  }

  Process*
  Image::find_component (const string& name)
  {
    Process* res = AbstractGShape::find_component(name);
    if(res) return res;

    bool propd = false;
    bool propt = false;
    Coupling ** coupling;
    double* rawpd;
    string* rawps;

    if(name=="x") {
      propd = true;
      coupling=&_cx;
      rawpd=&raw_props.x;
    } else
    if(name=="y") {
      propd = true;
      coupling=&_cy;
      rawpd=&raw_props.y;
    } else
    if(name=="width") {
      propd = true;
      coupling=&_cwidth;
      rawpd=&raw_props.width;
    } else
    if(name=="height") {
      propd = true;
      coupling=&_cheight;
      rawpd=&raw_props.height;
    } else
    if(name=="path") {
      propt = true;
      coupling=&_cpath;
      rawps=&raw_props.path;
    } else
    return nullptr;
    
    if(propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpd, name);
    }
    else if(propt) {
      TextPropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawps, name);
    } 

    return res;
  }

  void
  Image::activate ()
  {
    AbstractGObj::activate ();
    if (_cx) _cx->enable (_frame);
    if (_cy) _cy->enable (_frame);
    if (_cwidth) _cwidth->enable (_frame);
    if (_cheight) _cheight->enable (_frame);
    if (_cpath) _cpath->enable (_frame);
  }

  void
  Image::deactivate ()
  {
    AbstractGObj::deactivate ();
    if (_cx) _cx->disable ();
    if (_cy) _cy->disable ();
    if (_cwidth) _cwidth->disable ();
    if (_cheight) _cheight->disable ();
    if (_cpath) _cpath->disable ();
  }

  void
  Image::draw ()
  {
    if (somehow_activating () && Backend::instance ()->window () == _frame) {
      Backend::instance ()->draw_image (this);
    }
  }

  Process*
  Image::clone () 
  {
    return new Image (raw_props.path, raw_props.x, raw_props.y, raw_props.width, raw_props.height);
  } 
} /* namespace djnn */
