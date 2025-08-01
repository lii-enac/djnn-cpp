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

#include "gui/shape/image.h"

#include "core/core-dev.h"            // graph add/remove edge
#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/sdf.h"

namespace djnn {
Image::Image (CoreProcess* parent, const string& name, const string& path, double x, double y, double w,
              double h)
    : AbstractPathImage (parent, name, path, x, y, w, h),
      _cwatcher (nullptr),
      _watcher (nullptr), _pixel (this, "pixel", 0), _cache (nullptr), _invalid_cache (true)
{
    set_origin (x, y);
    finalize_construction (parent, name);
}

Image::~Image ()
{
    if (_cwatcher) {
        delete _cwatcher;
    }
    delete _watcher;
    Backend::instance ()->delete_image_impl (this);
}

CoreProcess*
Image::find_child_impl (const string& name)
{
    auto* res = AbstractPathImage::find_child_impl (name);
    // if(res) return res;
    if (name == "pixel" && !has_ui ()) {
        init_ui ();
    }
    if (name == "path" && _watcher == nullptr) {
        _watcher  = new ImageWatcher (this);
        _cwatcher = new Coupling (res, ACTIVATION, _watcher, ACTIVATION);
        if (!somehow_activating ())
            _cwatcher->disable ();
    }

    return res;
}

void
Image::impl_activate ()
{
    AbstractPathImage::impl_activate ();
    if (_cwatcher)
        _cwatcher->enable ();
}

void
Image::impl_deactivate ()
{
    if (_cwatcher)
        _cwatcher->disable ();
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

// Note: should not do like this : MP 15.10.2021
// void
// Image::press (double x, double y)
// {
//   if (x < AbstractImage::raw_props.width && y < AbstractImage::raw_props.height)
//     _pixel.set_value (Backend::instance()->get_pixel (this, x, y), true);
// }

// void
// Image::move (double x, double y) : MP 15.10.2021
// {
//   if (x < AbstractImage::raw_props.width && y < AbstractImage::raw_props.height)
//     _pixel.set_value (Backend::instance()->get_pixel (this, x, y), true);
// }

double
Image::sdf (double x, double y) const
{
    auto&  rectangle = AbstractImage::raw_props;
    vec2   p         = vec2 (x + rectangle.x, y + rectangle.y);
    double d;

    vec2 size = vec2 (rectangle.width, rectangle.height) / 2.; // /2. why oh why?
    size      = floor (size);
    vec2 p1   = p - size;
    d         = SDF_box (p1, size); // FIXME ry
    return d;
}

Image*
Image::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new Image (nullptr, name, raw_props.path, AbstractImage::raw_props.x, AbstractImage::raw_props.y, AbstractImage::raw_props.width, AbstractImage::raw_props.height);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

DataImage::DataImage (CoreProcess* parent, const string& name, string* data, int format, double x, double y, double w,
                      double h)
    : AbstractDataImage (parent, name, data, format, x, y, w, h),
      _cwatcher (nullptr),
      _watcher (nullptr), _cache (nullptr), _invalid_cache (true)
{
    if (data)
        raw_props.data = new string (*data);
    set_origin (x, y);
    finalize_construction (parent, name);
}

DataImage::DataImage (CoreProcess* parent, const string& name, string* data, double x, double y, double w, double h)
    : DataImage (parent, name, data, -1, x, y, w, h)
{
}

DataImage::DataImage (CoreProcess* parent, const string& name, double x, double y, double w, double h)
    : DataImage (parent, name, nullptr, -1, x, y, w, h)
{
}

DataImage::~DataImage ()
{
    delete raw_props.data;
    if (_cwatcher) {
        delete _cwatcher;
    }
    delete _watcher;
}

CoreProcess*
DataImage::find_child_impl (const string& name)
{
    auto* res = AbstractDataImage::find_child_impl (name);
    // if(res) return res;

    if (name == "data" && _watcher == nullptr) {
        _watcher  = new DataImageWatcher (this);
        _cwatcher = new Coupling (res, ACTIVATION, _watcher, ACTIVATION);
        if (!somehow_activating ())
            _cwatcher->disable ();
    }

    return res;
}

void
DataImage::impl_activate ()
{
    AbstractDataImage::impl_activate ();
    if (_cwatcher)
        _cwatcher->enable ();
}

void
DataImage::impl_deactivate ()
{
    if (_cwatcher)
        _cwatcher->disable ();
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
DataImage::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new DataImage (nullptr, name, raw_props.data, raw_props.format, AbstractImage::raw_props.x, AbstractImage::raw_props.y, AbstractImage::raw_props.width, AbstractImage::raw_props.height);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}
} // namespace djnn
