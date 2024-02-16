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

#include "abstract_scaling.h"

#include "audio/style/audio_style.h"
#include "display/abstract_display.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/abstract_gshape.h"
#include "gui/style/abstract_style.h"
#include "gui/transformation/abstract_transformation.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"

namespace djnn {
AbstractScaling::AbstractScaling (CoreProcess* parent, const string& name, double sx, double sy, double cx, double cy)
    : AbstractTransformation (parent, name),
      raw_props{.sx = sx, .sy = sy, .cx = cx, .cy = cy},
      _csx (nullptr), _csy (nullptr), _ccx (nullptr), _ccy (nullptr)
{
}

AbstractScaling::~AbstractScaling ()
{
    remove_edge (_csx);
    remove_edge (_csy);
    remove_edge (_ccx);
    remove_edge (_ccy);
    delete _csx;
    delete _csy;
    delete _ccx;
    delete _ccy;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("sx");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("sy");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("cx");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("cy");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractScaling::find_child_impl (const string& name)
{
    auto* res = AbstractTransformation::find_child_impl (name);
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

    if (name == "sx") {
        coupling    = &_csx;
        rawp_Double = &raw_props.sx;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "sy") {
        coupling    = &_csy;
        rawp_Double = &raw_props.sy;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "cx") {
        coupling    = &_ccx;
        rawp_Double = &raw_props.cx;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "cy") {
        coupling    = &_ccy;
        rawp_Double = &raw_props.cy;
        notify_mask = notify_damaged_transform;
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

const djnnstl::vector<djnnstl::string>&
AbstractScaling::get_properties_name () const
{
    static const vector<string> res = {
        "sx",
        "sy",
        "cx",
        "cy",
    };
    return res;
}

void
AbstractScaling::get_properties_values (double& sx, double& sy, double& cx, double& cy)
{
    sx = raw_props.sx;
    sy = raw_props.sy;
    cx = raw_props.cx;
    cy = raw_props.cy;
}

void
AbstractScaling::impl_activate ()
{
    AbstractTransformation::impl_activate ();
    auto _frame = get_frame ();
    enable (_csx, _frame->damaged ());
    enable (_csy, _frame->damaged ());
    enable (_ccx, _frame->damaged ());
    enable (_ccy, _frame->damaged ());
}

void
AbstractScaling::impl_deactivate ()
{
    disable (_csx);
    disable (_csy);
    disable (_ccx);
    disable (_ccy);
    AbstractTransformation::impl_deactivate ();
}

} /* namespace djnn */
