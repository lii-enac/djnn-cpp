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

namespace djnn
{
  Image::Image (Process *parent, const std::string& name, std::string path, double x, double y, double w,
    double h) :
      AbstractImage (parent, name, path, x, y, w, h),
      _cwatcher(nullptr),
      _watcher(nullptr), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
    Process::finalize_construction (parent, name);
  }

  Image::Image (std::string path, double x, double y, double w, double h) :
      AbstractImage (path, x, y, w, h),
      _cwatcher(nullptr),
      _watcher(nullptr), _cache(nullptr), _invalid_cache (true)
  {
    set_origin (x, y);
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

  Process*
  Image::find_component (const string& name)
  {
    Process* res = AbstractImage::find_component(name);
    if(res) return res;

    if( name=="path" ) {
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
    AbstractImage::impl_activate ();
    if (_cwatcher) _cwatcher->enable();
  }

  void
  Image::impl_deactivate ()
  {
    if (_cwatcher) _cwatcher->disable();
    AbstractImage::impl_deactivate ();
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
}
