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
 *  !! this file has been automatically generated - do NOT modify !!
 *
 */

#include "gui/shape/abstract_gshape.h"
#include "display/window.h"
#include "gui/style/abstract_style.h"

#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/abstract_display.h"

#include "audio/style/audio_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"

#include "abstract_prop_phy_obj.h"

namespace djnn {
AbstractPropPhyObj::AbstractPropPhyObj (CoreProcess* parent, const string& name, double x, double y, double z, double dx, double dy, double dz, double roll, double pitch, double yall, double mass, double density, double friction)
    : AbstractPObj (parent, name),
      raw_props{.x = x, .y = y, .z = z, .dx = dx, .dy = dy, .dz = dz, .roll = roll, .pitch = pitch, .yall = yall, .mass = mass, .density = density, .friction = friction},
      _cx (nullptr), _cy (nullptr), _cz (nullptr), _cdx (nullptr), _cdy (nullptr), _cdz (nullptr), _croll (nullptr), _cpitch (nullptr), _cyall (nullptr), _cmass (nullptr), _cdensity (nullptr), _cfriction (nullptr)
{
}

AbstractPropPhyObj::~AbstractPropPhyObj ()
{
    remove_edge (_cx);
    remove_edge (_cy);
    remove_edge (_cz);
    remove_edge (_cdx);
    remove_edge (_cdy);
    remove_edge (_cdz);
    remove_edge (_croll);
    remove_edge (_cpitch);
    remove_edge (_cyall);
    remove_edge (_cmass);
    remove_edge (_cdensity);
    remove_edge (_cfriction);
    delete _cx;
    delete _cy;
    delete _cz;
    delete _cdx;
    delete _cdy;
    delete _cdz;
    delete _croll;
    delete _cpitch;
    delete _cyall;
    delete _cmass;
    delete _cdensity;
    delete _cfriction;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("x");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("y");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("z");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("dx");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("dy");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("dz");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("roll");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("pitch");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("yall");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("mass");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("density");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("friction");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractPropPhyObj::find_child_impl (const string& name)
{
    auto* res = AbstractPObj::find_child_impl (name);
    if (res)
        return res;

    bool            prop_Double = false, prop_Int = false, prop_Text = false, prop_Textp = false;
    Coupling**      coupling    = nullptr;
    double*         rawp_Double = nullptr;
    int*            rawp_Int    = nullptr;
    typedef string  text;
    text*           rawp_Text = nullptr;
    typedef string* textp;
    textp*          rawp_Textp  = nullptr;
    int             notify_mask = notify_none;

    if (name == "x") {
        coupling    = &_cx;
        rawp_Double = &raw_props.x;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "y") {
        coupling    = &_cy;
        rawp_Double = &raw_props.y;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "z") {
        coupling    = &_cz;
        rawp_Double = &raw_props.z;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "dx") {
        coupling    = &_cdx;
        rawp_Double = &raw_props.dx;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "dy") {
        coupling    = &_cdy;
        rawp_Double = &raw_props.dy;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "dz") {
        coupling    = &_cdz;
        rawp_Double = &raw_props.dz;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "roll") {
        coupling    = &_croll;
        rawp_Double = &raw_props.roll;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "pitch") {
        coupling    = &_cpitch;
        rawp_Double = &raw_props.pitch;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "yall") {
        coupling    = &_cyall;
        rawp_Double = &raw_props.yall;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "mass") {
        coupling    = &_cmass;
        rawp_Double = &raw_props.mass;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "density") {
        coupling    = &_cdensity;
        rawp_Double = &raw_props.density;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "friction") {
        coupling    = &_cfriction;
        rawp_Double = &raw_props.friction;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else
        return nullptr;

    if (prop_Double) {
        DoublePropertyProxy* prop = nullptr; // do not cache
        res                       = create_GObj_prop (&prop, coupling, rawp_Double, name, notify_mask);
    } else if (prop_Int) {
        IntPropertyProxy* prop = nullptr; // do not cache
        res                    = create_GObj_prop (&prop, coupling, rawp_Int, name, notify_mask);
    } else if (prop_Text) {
        TextPropertyProxy* prop = nullptr; // do not cache
        res                     = create_GObj_prop (&prop, coupling, rawp_Text, name, notify_mask);
    } else if (prop_Textp) {
        TextPropertyProxy* prop = nullptr; // do not cache
        res                     = create_GObj_prop (&prop, coupling, rawp_Textp, name, notify_mask);
    }

    return res;
}

const vector<string>&
AbstractPropPhyObj::get_properties_name () const
{
    static const vector<string> res = {
        "x",
        "y",
        "z",
        "dx",
        "dy",
        "dz",
        "roll",
        "pitch",
        "yall",
        "mass",
        "density",
        "friction",
    };
    return res;
}

void
AbstractPropPhyObj::get_properties_values (double& x, double& y, double& z, double& dx, double& dy, double& dz, double& roll, double& pitch, double& yall, double& mass, double& density, double& friction)
{
    x        = raw_props.x;
    y        = raw_props.y;
    z        = raw_props.z;
    dx       = raw_props.dx;
    dy       = raw_props.dy;
    dz       = raw_props.dz;
    roll     = raw_props.roll;
    pitch    = raw_props.pitch;
    yall     = raw_props.yall;
    mass     = raw_props.mass;
    density  = raw_props.density;
    friction = raw_props.friction;
}

void
AbstractPropPhyObj::impl_activate ()
{
    AbstractPObj::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_cx, _frame->damaged ());
        enable (_cy, _frame->damaged ());
        enable (_cz, _frame->damaged ());
        enable (_cdx, _frame->damaged ());
        enable (_cdy, _frame->damaged ());
        enable (_cdz, _frame->damaged ());
        enable (_croll, _frame->damaged ());
        enable (_cpitch, _frame->damaged ());
        enable (_cyall, _frame->damaged ());
        enable (_cmass, _frame->damaged ());
        enable (_cdensity, _frame->damaged ());
        enable (_cfriction, _frame->damaged ());
    } else
        djnn_warning (nullptr, "AbstractPropPhyObj::impl_activate () - Unable to find frame\n\n");
}

void
AbstractPropPhyObj::impl_deactivate ()
{
    disable (_cx);
    disable (_cy);
    disable (_cz);
    disable (_cdx);
    disable (_cdy);
    disable (_cdz);
    disable (_croll);
    disable (_cpitch);
    disable (_cyall);
    disable (_cmass);
    disable (_cdensity);
    disable (_cfriction);
    AbstractPObj::impl_deactivate ();
}

} /* namespace djnn */
