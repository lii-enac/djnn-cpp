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

#include "gui/shape/abstract_gshape.h"

#include "abstract_prop_line.h"

namespace djnn {
AbstractPropLine::AbstractPropLine (CoreProcess* parent, const string& name, double x1, double y1, double x2, double y2)
    : AbstractGShape (parent, name),
      raw_props{.x1 = x1, .y1 = y1, .x2 = x2, .y2 = y2},
      _cx1 (nullptr), _cy1 (nullptr), _cx2 (nullptr), _cy2 (nullptr)
{
    set_origin (x1, y1);
}

AbstractPropLine::~AbstractPropLine ()
{
    remove_edge (_cx1);
    remove_edge (_cy1);
    remove_edge (_cx2);
    remove_edge (_cy2);
    delete _cx1;
    delete _cy1;
    delete _cx2;
    delete _cy2;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 2) {
        symtable_t::iterator it;

        it = find_child_iterator ("x1");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("y1");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("x2");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("y2");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractPropLine::find_child_impl (const string& name)
{
    auto* res = AbstractGShape::find_child_impl (name);
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

    if (name == "x1") {
        coupling    = &_cx1;
        rawp_Double = &raw_props.x1;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "y1") {
        coupling    = &_cy1;
        rawp_Double = &raw_props.y1;
        notify_mask = notify_damaged_transform;
        prop_Double = true;
    } else if (name == "x2") {
        coupling    = &_cx2;
        rawp_Double = &raw_props.x2;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "y2") {
        coupling    = &_cy2;
        rawp_Double = &raw_props.y2;
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
AbstractPropLine::get_properties_name () const
{
    static const vector<string> res = {
        "x1",
        "y1",
        "x2",
        "y2",
    };
    return res;
}

void
AbstractPropLine::get_properties_values (double& x1, double& y1, double& x2, double& y2)
{
    x1 = raw_props.x1;
    y1 = raw_props.y1;
    x2 = raw_props.x2;
    y2 = raw_props.y2;
}

void
AbstractPropLine::impl_activate ()
{
    AbstractGShape::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_cx1, _frame->damaged ());
        enable (_cy1, _frame->damaged ());
        enable (_cx2, _frame->damaged ());
        enable (_cy2, _frame->damaged ());
    } else
        djnn_warning (nullptr, "AbstractPropLine::impl_activate () - Unable to find frame\n\n");
}

void
AbstractPropLine::impl_deactivate ()
{
    disable (_cx1);
    disable (_cy1);
    disable (_cx2);
    disable (_cy2);
    AbstractGShape::impl_deactivate ();
}

} /* namespace djnn */
