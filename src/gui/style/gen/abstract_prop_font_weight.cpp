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

#include "gui/style/abstract_style.h"

#include "abstract_prop_font_weight.h"

namespace djnn {
AbstractPropFontWeight::AbstractPropFontWeight (CoreProcess* parent, const string& name, int weight)
    : AbstractStyle (parent, name),
      raw_props{.weight = weight},
      _cweight (nullptr)
{
}

AbstractPropFontWeight::~AbstractPropFontWeight ()
{
    remove_edge (_cweight);
    delete _cweight;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("weight");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractPropFontWeight::find_child_impl (const string& name)
{
    auto* res = AbstractStyle::find_child_impl (name);
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

    if (name == "weight") {
        coupling    = &_cweight;
        rawp_Int    = &raw_props.weight;
        notify_mask = notify_damaged_geometry;
        prop_Int    = true;
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
AbstractPropFontWeight::get_properties_name () const
{
    static const vector<string> res = {
        "weight",
    };
    return res;
}

void
AbstractPropFontWeight::get_properties_values (int& weight)
{
    weight = raw_props.weight;
}

void
AbstractPropFontWeight::impl_activate ()
{
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_cweight, _frame->damaged ());
    } else
        djnn_warning (nullptr, "AbstractPropFontWeight::impl_activate () - Unable to find frame\n\n");
}

void
AbstractPropFontWeight::impl_deactivate ()
{
    disable (_cweight);
    AbstractStyle::impl_deactivate ();
}

} /* namespace djnn */
