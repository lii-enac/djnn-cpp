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

#include "abstract_prop_font_style.h"

#include "audio/style/audio_style.h"
#include "display/abstract_display.h"
#include "display/display-dev.h" // DisplayBackend::instance
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/abstract_gshape.h"
#include "gui/style/abstract_style.h"
#include "physics/abstract_pobj.h"
#include "physics/physics.h"

namespace djnn {
AbstractPropFontStyle::AbstractPropFontStyle (CoreProcess* parent, const string& name, int style)
    : AbstractStyle (parent, name),
      raw_props{.style = style},
      _cstyle (nullptr)
{
}

AbstractPropFontStyle::~AbstractPropFontStyle ()
{
    remove_edge (_cstyle);
    delete _cstyle;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("style");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractPropFontStyle::find_child_impl (const string& name)
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

    if (name == "style") {
        coupling    = &_cstyle;
        rawp_Int    = &raw_props.style;
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

const djnnstl::vector<djnnstl::string>&
AbstractPropFontStyle::get_properties_name () const
{
    static const vector<string> res = {
        "style",
    };
    return res;
}

void
AbstractPropFontStyle::get_properties_values (int& style)
{
    style = raw_props.style;
}

void
AbstractPropFontStyle::impl_activate ()
{
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    enable (_cstyle, _frame->damaged ());
}

void
AbstractPropFontStyle::impl_deactivate ()
{
    disable (_cstyle);
    AbstractStyle::impl_deactivate ();
}

} /* namespace djnn */
