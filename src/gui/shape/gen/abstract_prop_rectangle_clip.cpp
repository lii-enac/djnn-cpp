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

#include "abstract_prop_rectangle_clip.h"

namespace djnn {
AbstractPropRectangleClip::AbstractPropRectangleClip (CoreProcess* parent, const string& name, double x, double y, double width, double height)
    : AbstractGShape (parent, name),
      raw_props{.x = x, .y = y, .width = width, .height = height},
      _cx (nullptr), _cy (nullptr), _cwidth (nullptr), _cheight (nullptr)
{
    set_origin (x, y);
}

AbstractPropRectangleClip::~AbstractPropRectangleClip ()
{
    remove_edge (_cx);
    remove_edge (_cy);
    remove_edge (_cwidth);
    remove_edge (_cheight);
    delete _cx;
    delete _cy;
    delete _cwidth;
    delete _cheight;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 2) {
        symtable_t::iterator it;

        it = find_child_iterator ("x");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("y");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("width");
        if (it != children_end ())
            delete it->second;

        it = find_child_iterator ("height");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractPropRectangleClip::find_child_impl (const string& name)
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
    } else if (name == "width") {
        coupling    = &_cwidth;
        rawp_Double = &raw_props.width;
        notify_mask = notify_damaged_geometry;
        prop_Double = true;
    } else if (name == "height") {
        coupling    = &_cheight;
        rawp_Double = &raw_props.height;
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
AbstractPropRectangleClip::get_properties_name () const
{
    static const vector<string> res = {
        "x",
        "y",
        "width",
        "height",
    };
    return res;
}

void
AbstractPropRectangleClip::get_properties_values (double& x, double& y, double& width, double& height)
{
    x      = raw_props.x;
    y      = raw_props.y;
    width  = raw_props.width;
    height = raw_props.height;
}

void
AbstractPropRectangleClip::impl_activate ()
{
    AbstractGShape::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_cx, _frame->damaged ());
        enable (_cy, _frame->damaged ());
        enable (_cwidth, _frame->damaged ());
        enable (_cheight, _frame->damaged ());
    } else
        djnn_warning (nullptr, "AbstractPropRectangleClip::impl_activate () - Unable to find frame\n\n");
}

void
AbstractPropRectangleClip::impl_deactivate ()
{
    disable (_cx);
    disable (_cy);
    disable (_cwidth);
    disable (_cheight);
    AbstractGShape::impl_deactivate ();
}

} /* namespace djnn */
