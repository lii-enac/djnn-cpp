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

#include "gui/shape/ellipse.h"

#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/sdf.h"

namespace djnn {
Ellipse::Ellipse (CoreProcess* parent, const string& name, double cx, double cy, double rx, double ry)
    : AbstractPropEllipse (parent, name, cx, cy, rx, ry)
{
    finalize_construction (parent, name);
}

Ellipse*
Ellipse::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new Ellipse (nullptr, name, raw_props.cx, raw_props.cy, raw_props.rx, raw_props.ry);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
Ellipse::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        AbstractGShape::pre_draw ();
        Backend::instance ()->draw_ellipse (this);
        AbstractGShape::post_draw ();
    }
}

void
Ellipse::get_bounding_box (double& x, double& y, double& w, double& h) const
{
    x = raw_props.cx - raw_props.rx;
    y = raw_props.cy - raw_props.ry;
    w = raw_props.rx + raw_props.rx;
    h = raw_props.ry + raw_props.ry;
}

double
Ellipse::sdf (double x, double y) const
{
    auto&  ellipse = raw_props;
    vec2   p       = vec2 (x - ellipse.cx, y - ellipse.cy);
    double d;
    d = SDF_ellipse (p, vec2 (ellipse.rx, ellipse.ry));
    return d;
}

} /* namespace djnn */
