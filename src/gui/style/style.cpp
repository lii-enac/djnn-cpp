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
 *
 */

#include "style.h"

#include "core/ontology/coupling.h"
#include "core/tree/list.h"
#include "display/abstract_display.h"
#include "display/display-dev.h" // DisplayBackend
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"

// #include "core/utils/iostream.h"
#include "core/utils/error.h"

namespace djnn {

AbstractStyle::AbstractStyle (CoreProcess* parent, const string& name)
    : AbstractGObj (parent, name)
{
}

AbstractStyle::~AbstractStyle ()
{
}

void
FillColor::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_fill_color (this);
    }
}

FillColor*
FillColor::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FillColor (nullptr, get_name (), raw_props.r, raw_props.g, raw_props.b);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
OutlineColor::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_outline_color (this);
    }
}

OutlineColor*
OutlineColor::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new OutlineColor (nullptr, get_name (), raw_props.r, raw_props.g, raw_props.b);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
NoOutline::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_no_outline ();
    }
}

NoOutline*
NoOutline::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new NoOutline (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

NoPickOutline*
NoPickOutline::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new NoPickOutline (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
NoPickOutline::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_no_pick_outline ();
    }
}

PickOutline*
PickOutline::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new PickOutline (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
PickOutline::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_pick_outline ();
    }
}

void
NoFill::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_no_fill ();
    }
}

NoFill*
NoFill::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new NoFill (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
NoPickFill::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_no_pick_fill ();
    }
}

NoPickFill*
NoPickFill::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new NoPickFill (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
PickFill::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_pick_fill ();
    }
}

PickFill*
PickFill::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new PickFill (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
OutlineOpacity::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_outline_opacity (this);
    }
}

OutlineOpacity*
OutlineOpacity::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new OutlineOpacity (nullptr, get_name (), raw_props.a);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
FillOpacity::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_fill_opacity (this);
    }
}

FillOpacity*
FillOpacity::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FillOpacity (nullptr, get_name (), raw_props.a);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
DashArray::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_dash_array (this);
    }
}

DashArray*
DashArray::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto* res = new DashArray (nullptr, get_name ());
    for (auto d : _dash_array) {
        res->_dash_array.push_back (d);
    }
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
NoDashArray::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_no_dash_array ();
    }
}

NoDashArray*
NoDashArray::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new NoDashArray (nullptr, get_name ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

GradientStop::GradientStop (CoreProcess* parent, const string& name, double r, double g, double b, double a, double offset)
    : AbstractPropGradientStop (parent, name, r, g, b, a, offset)
{
    /* avoid dynamic_cast for cloning */
    if (parent == nullptr)
        return;

    /* if not cloning we test parent with dynamic_cast */
    AbstractGradient* grad = dynamic_cast<AbstractGradient*> (parent);
    if (grad == nullptr) {
        error (this, "parent of Gradient Stop must be <LinearGradient|RadialGradient>\n");
        return;
    }
    grad->stops ()->add_child (this, "");
}

GradientStop::~GradientStop ()
{
}

GradientStop*
GradientStop::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new GradientStop (nullptr, get_name (), raw_props.r, raw_props.g, raw_props.b, raw_props.a, raw_props.offset);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

void
GradientStop::draw ()
{
    Backend::instance ()->load_gradient_stop (this);
}

AbstractGradient::AbstractGradient (CoreProcess* parent, const string& name, int spread, int coords)
    : AbstractPropGradient (parent, name, spread, coords),
      bstops (true), btransforms (true), bspread (true), bcoords (true), _g (nullptr), _linear (false)
{
    _transforms = new List (this, "transforms");
    _stops      = new List (this, "stops");
}

AbstractGradient::~AbstractGradient ()
{

    // delete his own stops and transforms (NO merging has been applied)
    if (btransforms)
        delete _transforms;
    if (bstops)
        delete _stops;

    // remove only from symtable his own spread and coords (because merging has been applied)
    if (!bspread)
        this->remove_symbol ("spread");
    if (!bcoords)
        this->remove_symbol ("coords");
}

LinearGradient::LinearGradient (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2,
                                djnFillSpread s, djnFillCoords fc)
    : AbstractPropLinearGradient (parent, name, x1, y1, x2, y2, s, fc)
{
    _linear = true;
    finalize_construction (parent, name);
}

LinearGradient::LinearGradient (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2, int s,
                                int fc)
    : AbstractPropLinearGradient (parent, name, x1, y1, x2, y2, s, fc)
{
    _linear = true;
    finalize_construction (parent, name);
}

LinearGradient*
LinearGradient::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    /*LinearGradient *g = new LinearGradient (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2,
                                            AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto s : _stops->children ()) {
      g->stops ()->add_child (s->clone (), s->get_name (_stops));
    }
    for (auto t : _transforms->children ()) {
      g->transforms ()->add_child (t->clone (), t->get_name (_stops));
    }
    return g;*/

    auto* clone = new LinearGradient (nullptr, get_name (), raw_props.x1, raw_props.y1, raw_props.x2, raw_props.y2,
                                      AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto c : _stops->children ()) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->stops ()->add_child (cclone, c->find_child_name (_stops));
    }
    for (auto c : _transforms->children ()) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->transforms ()->add_child (cclone, c->find_child_name (_stops));
    }
    origs_clones[this] = clone;
    return clone;
}

LinearGradient::~LinearGradient ()
{
}

void
LinearGradient::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_linear_gradient (this);
    }
}

RefLinearGradient::RefLinearGradient (CoreProcess* parent, const string& name, LinearGradient* lg)
    : AbstractStyle (parent, name), _lg (lg)
{
    activate_children ();
    finalize_construction (parent, name);
}

void
RefLinearGradient::activate_children ()
{
    _lg->set_activation_flag (ACTIVATION);
    _lg->transforms ()->set_activation_flag (ACTIVATION);
    for (auto t : _lg->transforms ()->children ()) {
        t->set_activation_flag (ACTIVATION);
    }
    _lg->stops ()->set_activation_flag (ACTIVATION);
    for (auto s : _lg->stops ()->children ()) {
        s->set_activation_flag (ACTIVATION);
    }
}

void
RefLinearGradient::impl_activate ()
{
    AbstractStyle::impl_activate ();
}

void
RefLinearGradient::impl_deactivate ()
{
    AbstractStyle::impl_deactivate ();
}

void
RefLinearGradient::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_linear_gradient (_lg);
    }
}

RefLinearGradient*
RefLinearGradient::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new RefLinearGradient (nullptr, get_name (), _lg);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

RadialGradient::RadialGradient (CoreProcess* parent, const string& name, double cx, double cy, double r, double fx,
                                double fy, djnFillSpread s, djnFillCoords fc)
    : AbstractPropRadialGradient (parent, name, cx, cy, r, fx, fy, s, fc)
{
    finalize_construction (parent, name);
}

RadialGradient::RadialGradient (CoreProcess* parent, const string& name, double cx, double cy, double r, double fx,
                                double fy, int s, int fc)
    : AbstractPropRadialGradient (parent, name, cx, cy, r, fx, fy, s, fc)
{
    finalize_construction (parent, name);
}

RadialGradient::~RadialGradient () {}

RadialGradient*
RadialGradient::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    /*RadialGradient *rg = new RadialGradient (nullptr, get_name (), raw_props.cx, raw_props.cy, raw_props.r, raw_props.fx,
                                             raw_props.fy, AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto s : _stops->children ()) {
      rg->stops ()->add_child (s->clone (), s->get_name (_stops));
    }
    for (auto t : _transforms->children ()) {
      rg->transforms ()->add_child (t->clone (), t->get_name (_stops));
    }
    return rg;*/

    auto* clone = new RadialGradient (nullptr, get_name (), raw_props.cx, raw_props.cy, raw_props.r, raw_props.fx,
                                      raw_props.fy, AbstractGradient::raw_props.spread, AbstractGradient::raw_props.coords);
    for (auto c : _stops->children ()) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->stops ()->add_child (cclone, c->find_child_name (_stops));
    }
    for (auto c : _transforms->children ()) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->transforms ()->add_child (cclone, c->find_child_name (_stops));
    }
    origs_clones[this] = clone;
    return clone;
}

void
RadialGradient::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_radial_gradient (this);
    }
}

RefRadialGradient::RefRadialGradient (CoreProcess* parent, const string& name, RadialGradient* rg)
    : AbstractStyle (parent, name), _rg (rg)
{
    activate_children ();
    finalize_construction (parent, name);
}

void
RefRadialGradient::activate_children ()
{
    _rg->set_activation_flag (ACTIVATION);
    _rg->transforms ()->set_activation_flag (ACTIVATION);
    for (auto t : _rg->transforms ()->children ()) {
        t->set_activation_flag (ACTIVATION);
    }
    _rg->stops ()->set_activation_flag (ACTIVATION);
    for (auto s : _rg->stops ()->children ()) {
        s->set_activation_flag (ACTIVATION);
    }
}

void
RefRadialGradient::impl_activate ()
{
    AbstractStyle::impl_activate ();
}

void
RefRadialGradient::impl_deactivate ()
{
    AbstractStyle::impl_deactivate ();
}

void
RefRadialGradient::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_radial_gradient (_rg);
    }
}

RefRadialGradient*
RefRadialGradient::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new RefRadialGradient (nullptr, get_name (), _rg);
    origs_clones[this] = res;
    return res;
}

void
AbstractGradient::update ()
{
    if (stops () != (List*)find_child ("stops"))
        _stops = (List*)find_child ("stops");
    if (_transforms != (List*)find_child ("transforms"))
        _transforms = (List*)find_child ("transforms");
}

FontSize::FontSize (CoreProcess* parent, const string& name, djnLengthUnit unit, double size)
    : AbstractPropFontSize (parent, name, unit, size)
{
    finalize_construction (parent, name);
}

FontSize::FontSize (CoreProcess* parent, const string& name, int unit, double size)
    : AbstractPropFontSize (parent, name, unit, size)
{
    finalize_construction (parent, name);
}

void
FontSize::impl_activate ()
{
    Container* c = dynamic_cast<Container*> (get_parent ());
    if (c) {
        c->add_to_context ("FontSize", this);
    }
    AbstractPropFontSize::impl_activate ();
}

void
FontSize::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_font_size (this);
    }
}

FontSize*
FontSize::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FontSize (nullptr, get_name (), raw_props.unit, raw_props.size);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

FontWeight::FontWeight (CoreProcess* parent, const string& name, int weight)
    : AbstractPropFontWeight (parent, name, weight)
{
    finalize_construction (parent, name);
}

void
FontWeight::impl_activate ()
{
    Container* c = dynamic_cast<Container*> (get_parent ());
    if (c)
        c->add_to_context ("FontWeight", this);
    AbstractPropFontWeight::impl_activate ();
}

void
FontWeight::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_font_weight (this);
    }
}

FontWeight*
FontWeight::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FontWeight (nullptr, get_name (), raw_props.weight);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

FontStyle::FontStyle (CoreProcess* parent, const string& name, djnFontSlope style)
    : AbstractPropFontStyle (parent, name, style)
{
    finalize_construction (parent, name);
}

FontStyle::FontStyle (CoreProcess* parent, const string& name, int style)
    : AbstractPropFontStyle (parent, name, style)
{
    finalize_construction (parent, name);
}

void
FontStyle::impl_activate ()
{
    Container* c = dynamic_cast<Container*> (get_parent ());
    if (c)
        c->add_to_context ("FontStyle", this);
    AbstractPropFontStyle::impl_activate ();
}

void
FontStyle::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_font_style (this);
    }
}

FontStyle*
FontStyle::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FontStyle (nullptr, get_name (), raw_props.style);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

FontFamily::FontFamily (CoreProcess* parent, const string& name, const string& family)
    : AbstractPropFontFamily (parent, name, family)
{
    finalize_construction (parent, name);
}

void
FontFamily::impl_activate ()
{
    AbstractPropFontFamily::impl_activate ();
    Container* c = dynamic_cast<Container*> (get_parent ());
    if (c)
        c->add_to_context ("FontFamily", this);
}

void
FontFamily::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_font_family (this);
    }
}

FontFamily*
FontFamily::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new FontFamily (nullptr, get_name (), raw_props.family);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}
} // namespace djnn
