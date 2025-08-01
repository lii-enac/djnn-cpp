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

#include "gui/shape/poly.h"

#include "core/control/blank.h"
#include "core/tree/list.h"
#include "core/utils/error.h"
#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/sdf.h"
#include "utils/debug.h"

namespace djnn {

PolyPoint::PolyPoint (CoreProcess* parent, const string& name, double x, double y)
    : AbstractGObj (parent, name),
      raw_props{.x = x, .y = y},
      _cx (nullptr), _cy (nullptr)
{
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr)
        return;

    /* if not cloning we test parent with dynamic_cast */
    Poly* poly = dynamic_cast<Poly*> (parent);
    if (poly == nullptr) {
        error (this, "Parent of polypoint must be <Polygon|Polyline>\n");
        return;
    }
    poly->points ()->add_child (this, "");
    parent->add_symbol (name, this);
}

PolyPoint::~PolyPoint ()
{
    remove_edge (_cx);
    remove_edge (_cy);
    delete _cx;
    delete _cy;

    if (children_empty () == false) {
        symtable_t::iterator it;

        it = find_child_iterator ("x");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("y");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
PolyPoint::find_child_impl (const string& name)
{
    auto* res = AbstractGObj::find_child_impl (name);
    if (res)
        return res;

    CouplingWithData** coupling;
    double*            rawp;
    int                notify_mask = notify_none;

    if (name == "x") {
        coupling    = &_cx;
        rawp        = &raw_props.x;
        notify_mask = notify_damaged_geometry;
    } else if (name == "y") {
        coupling    = &_cy;
        rawp        = &raw_props.y;
        notify_mask = notify_damaged_geometry;
    } else
        return nullptr;

    DoublePropertyProxy* prop = nullptr;
    res                       = create_GObj_prop (&prop, coupling, rawp, name, notify_mask);

    return res;
}

void
PolyPoint::impl_activate ()
{
    AbstractGObj::impl_activate ();
    auto* damaged = get_frame ()->damaged ();
    enable (_cx, damaged);
    enable (_cy, damaged);
}

void
PolyPoint::impl_deactivate ()
{
    disable (_cx);
    disable (_cy);
    AbstractGObj::impl_deactivate ();
}

void
PolyPoint::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->draw_poly_point (raw_props.x, raw_props.y);
    }
}

PolyPoint*
PolyPoint::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new PolyPoint (nullptr, name, raw_props.x, raw_props.y);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

Poly::Poly (CoreProcess* parent, const string& name, int closed)
    : AbstractGShape (parent, name), _closed (closed)
{
    _points       = new List (this, "points");
    _bounding_box = new Blank (this, "bounding_box");
    _bbx          = new DoubleProperty (nullptr, "bbx", 0);
    _bby          = new DoubleProperty (nullptr, "bby", 0);
    _bbw          = new DoubleProperty (nullptr, "bbw", 0);
    _bbh          = new DoubleProperty (nullptr, "bbh", 0);
    _bounding_box->add_symbol ("x", _bbx);
    _bounding_box->add_symbol ("y", _bby);
    _bounding_box->add_symbol ("width", _bbw);
    _bounding_box->add_symbol ("height", _bbh);

    finalize_construction (parent, name);
}

Poly::~Poly ()
{
    delete _bbh;
    delete _bbw;
    delete _bby;
    delete _bbx;
    delete _bounding_box;
    delete _points;
}

void
Poly::impl_activate ()
{
    AbstractGObj::impl_activate ();
    _points->activate ();
}

void
Poly::impl_deactivate ()
{
    _points->deactivate ();
    AbstractGShape::impl_deactivate ();
}

void
Poly::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        AbstractGShape::pre_draw ();
        Backend::instance ()->draw_poly (this);
        AbstractGShape::post_draw ();
    }
}

void
Poly::get_bounding_box (double& x, double& y, double& w, double& h) const
{
    UNIMPL;
}

double
Poly::sdf (double x, double y) const
{
    UNIMPL;
    return 0;
}

void
Poly::set_bounding_box (double x, double y, double w, double h)
{
    if (_bbx->get_double_value () != x)
        _bbx->set_value (x, true);
    if (_bby->get_double_value () != y)
        _bby->set_value (y, true);
    if (_bbw->get_double_value () != w)
        _bbw->set_value (w, true);
    if (_bbh->get_double_value () != h)
        _bbh->set_value (h, true);
}

Poly*
Poly::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto* clone = new Poly (nullptr, name, _closed);
    for (auto c : _points->children ()) {
        auto cclone = c->impl_clone (origs_clones, name);
        // origs_clones[c] = cclone;
        clone->_points->add_child (cclone, this->find_child_name (c));
    }
    origs_clones[this] = clone;
    return clone;
}
} // namespace djnn
