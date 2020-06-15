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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "shapes.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"
#include "display/window.h"

#include "core/ontology/coupling.h"
#include "core/execution/graph.h"

#include "shapes.h"
#include "gui/shapes/sdf.h"

namespace djnn
{
  Image::Image (FatProcess *parent, const std::string& name, std::string path, double x, double y, double w,
    double h) :
      AbstractPathImage (parent, name, path, x, y, w, h),
      _cwatcher(nullptr),
      _watcher(nullptr), _pixel (this, "pixel", 0), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
    finalize_construction (parent, name);
  }

  Image::~Image ()
  {
    if (_cwatcher) {
      Graph::instance ().remove_edge ( this->path (), _watcher);
      delete _cwatcher;
    }
    delete _watcher;
    Backend::instance ()->delete_image_impl (this);
  }

  ChildProcess*
  Image::find_child (const std::string& name)
  {
    auto * res = AbstractPathImage::find_child(name);
    //if(res) return res;
    if (name == "pixel" && !has_ui()) {
      init_ui ();
    }
    if( name=="path" && _watcher == nullptr) {
      _watcher = new ImageWatcher (this);
      _cwatcher = new Coupling (res, ACTIVATION, _watcher, ACTIVATION);
      _cwatcher->disable ();
      Graph::instance ().add_edge (res, _watcher);
    }

    return res;
  }

  void
  Image::impl_activate ()
  {
    AbstractPathImage::impl_activate ();
    if (_cwatcher) _cwatcher->enable();
  }

  void
  Image::impl_deactivate ()
  {
    if (_cwatcher) _cwatcher->disable();
    AbstractPathImage::impl_deactivate ();
  }

  void
  Image::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      AbstractGShape::pre_draw ();
      Backend::instance ()->draw_image (this);
      AbstractGShape::post_draw ();
    }
  }

  void
  Image::get_bounding_box (double& x, double& y, double& w, double& h) const
  {
    x = AbstractImage::raw_props.x;
    y = AbstractImage::raw_props.y;
    w = AbstractImage::raw_props.width;
    h = AbstractImage::raw_props.height;
  }

  void
  Image::press (double x, double y)
  {
    if (x < AbstractImage::raw_props.width && y < AbstractImage::raw_props.height)
      _pixel.set_value (Backend::instance()->get_pixel (this, x, y), true);
  }

  void
  Image::move (double x, double y)
  {
    if (x < AbstractImage::raw_props.width && y < AbstractImage::raw_props.height)
      _pixel.set_value (Backend::instance()->get_pixel (this, x, y), true);
  }

  double
  Image::sdf (double x, double y) const
  {
    auto & rectangle = AbstractImage::raw_props;
    vec2 p = vec2(x + rectangle.x, y + rectangle.y);
    double d;

    vec2 size = vec2(rectangle.width, rectangle.height)/2.; // /2. why oh why?
    size      = floor(size);
    vec2 p1   = p-size;
    d = SDF_box(p1, size); // FIXME ry
    return d;
  }

  Image*
  Image::clone () 
  {
    return new Image (nullptr, get_name (), raw_props.path, AbstractImage::raw_props.x, AbstractImage::raw_props.y, AbstractImage::raw_props.width, AbstractImage::raw_props.height);
  }

  DataImage::DataImage (FatProcess *parent, const std::string& name, const std::string& data, double x, double y, double w,
    double h) :
      AbstractDataImage (parent, name, data, x, y, w, h),
      _cwatcher(nullptr),
      _watcher(nullptr), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
    finalize_construction (parent, name);
  }

  DataImage::DataImage (FatProcess *parent, const std::string& name, double x, double y, double w, double h)
  : DataImage(parent, name, std::string(), x, y, w, h)
  {
  }

  DataImage::~DataImage ()
  {
    if (_cwatcher) {
      Graph::instance ().remove_edge ( this->data (), _watcher);
      delete _cwatcher;
    }
    delete _watcher;
  }

  ChildProcess*
  DataImage::find_child (const std::string& name)
  {
    auto * res = AbstractDataImage::find_child(name);
    //if(res) return res;

    if( name=="data" && _watcher == nullptr) {
      _watcher = new DataImageWatcher (this);
      _cwatcher = new Coupling (res, ACTIVATION, _watcher, ACTIVATION);
      _cwatcher->disable ();
      Graph::instance ().add_edge (res, _watcher);
    }

    return res;
  }

  void
  DataImage::impl_activate ()
  {
    AbstractDataImage::impl_activate ();
    if (_cwatcher) _cwatcher->enable();
  }

  void
  DataImage::impl_deactivate ()
  {
    if (_cwatcher) _cwatcher->disable();
    AbstractDataImage::impl_deactivate ();
  }

  void
  DataImage::draw ()
  {
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
      Backend::instance ()->draw_data_image (this);
    }
  }

  DataImage*
  DataImage::clone ()
  {
    return new DataImage (nullptr, get_name(), raw_props.data, AbstractImage::raw_props.x, AbstractImage::raw_props.y, AbstractImage::raw_props.width, AbstractImage::raw_props.height);
  }
}
