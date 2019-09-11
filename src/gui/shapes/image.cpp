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
#include "../../display/display.h"
#include "../../display/abstract_display.h"
#include "../../display/window.h"
#include "shapes.h"
#include "../../core/ontology/coupling.h"

namespace djnn
{
  Image::Image (Process *p, const std::string& n, std::string path, double x, double y, double w,
    double h) :
      AbstractImage (p, n, path, x, y, w, h),
      //raw_props{.x=x, .y=y, .width=w, .height=h, .path=path},
      //_cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _cpath (nullptr),
      _cwatcher(nullptr),
      _watcher(nullptr), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
    Process::finalize_construction (p);
  }

  Image::Image (std::string path, double x, double y, double w, double h) :
      AbstractImage (path, x, y, w, h),
      //raw_props{.x=x, .y=y, .width=w, .height=h, .path=path},
      //_cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr), _cpath (nullptr),
      _cwatcher(nullptr),
      _watcher(nullptr), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
  }

  Image::~Image ()
  {
    // delete _cx;
    // delete _cy;
    // delete _cwidth;
    // delete _cheight;
    // delete _cpath;  
    //delete _cache; // can't delete void*
    if (_cwatcher) {
      Graph::instance ().remove_edge ( this->path (), _watcher);
      delete _cwatcher;
    }
    delete _watcher;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */ 
    // if (_symtable.size () > 2) {
    //   std::map<std::string, Process*>::iterator it;

    //   it = _symtable.find ("x");
    //   if (it != _symtable.end ())
    //     delete it->second;

    //   it = _symtable.find ("y");
    //   if (it != _symtable.end ())
    //     delete it->second;

    //   it = _symtable.find ("width");
    //   if (it != _symtable.end ())
    //     delete it->second;

    //   it = _symtable.find ("height");
    //   if (it != _symtable.end ())
    //     delete it->second;

    //   it = _symtable.find ("path");
    //   if (it != _symtable.end ())
    //     delete it->second;
    // }
  }

  Process*
  Image::find_component (const string& name)
  {
    Process* res = AbstractImage::find_component(name);
    if(res) return res;
/*
    bool propd = false;
    bool propt = false;
    Coupling ** coupling;
    double* rawpd;
    string* rawps;
    int notify_mask = notify_none;

    if(name=="x") {
      propd = true;
      coupling=&_cx;
      rawpd=&raw_props.x;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="y") {
      propd = true;
      coupling=&_cy;
      rawpd=&raw_props.y;
      notify_mask = notify_damaged_transform;
    } else
    if(name=="width") {
      propd = true;
      coupling=&_cwidth;
      rawpd=&raw_props.width;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="height") {
      propd = true;
      coupling=&_cheight;
      rawpd=&raw_props.height;
      notify_mask = notify_damaged_geometry;
    } else
    if(name=="path") {
      propt = true;
      coupling=&_cpath;
      rawps=&raw_props.path;
      notify_mask = notify_damaged_geometry;
    } else
    return nullptr;
    
    if(propd) {
      DoublePropertyProxy* prop = nullptr;
      res = create_GObj_prop(&prop, coupling, rawpd, name, notify_mask);
    }
    else if(propt) {
      TextPropertyProxy* prop = nullptr; 
      res = create_GObj_prop(&prop, coupling, rawps, name, notify_mask);
*/
    if( name=="path" ) {
      //textProperty is only use for path SO should be: name = "path"
      _watcher = new ImageWatcher (this);
      _cwatcher = new Coupling (res, ACTIVATION, _watcher, ACTIVATION);
      _cwatcher->disable ();
      Graph::instance ().add_edge (res, _watcher);
    }

    return res;
  }
/*
  void
  Image::get_properties_values (string &path, double &x, double &y, double &w, double &h)
  {
    path = raw_props.path;
    x = raw_props.x;
    y = raw_props.y;
    w = raw_props.width;
    h = raw_props.height;
  }
*/
  void
  Image::impl_activate ()
  {
    AbstractImage::impl_activate ();
    // if (_cx) _cx->enable (_frame);
    // if (_cy) _cy->enable (_frame);
    // if (_cwidth) _cwidth->enable (_frame);
    // if (_cheight) _cheight->enable (_frame);
    // if (_cpath) _cpath->enable (_frame);
    if (_cwatcher) _cwatcher->enable();
  }

  void
  Image::impl_deactivate ()
  {
    AbstractImage::impl_deactivate ();
    // if (_cx) _cx->disable ();
    // if (_cy) _cy->disable ();
    // if (_cwidth) _cwidth->disable ();
    // if (_cheight) _cheight->disable ();
    // if (_cpath) _cpath->disable ();
    if (_cwatcher) _cwatcher->disable();
  }

  void
  Image::draw ()
  {
    auto _frame = frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_image (this);
    }
  }

  Process*
  Image::clone () 
  {
    return new Image (raw_props.path, raw_props.x, raw_props.y, raw_props.width, raw_props.height);
  } 
} /* namespace djnn */
