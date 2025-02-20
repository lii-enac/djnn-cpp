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

#include "abstract_opacity.h"

namespace djnn {
AbstractOpacity::AbstractOpacity (CoreProcess* parent, const string& name, double a)
    : AbstractStyle (parent, name),
      raw_props{.a = a},
      _ca (nullptr)
{
}

AbstractOpacity::~AbstractOpacity ()
{
    remove_edge (_ca);
    delete _ca;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("a");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractOpacity::find_child_impl (const string& name)
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

    if (name == "a") {
        coupling    = &_ca;
        rawp_Double = &raw_props.a;
        notify_mask = notify_damaged_style;
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
AbstractOpacity::get_properties_name () const
{
    static const vector<string> res = {
        "a",
    };
    return res;
}

void
AbstractOpacity::get_properties_values (double& a)
{
    a = raw_props.a;
}

void
AbstractOpacity::impl_activate ()
{
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_ca, _frame->damaged ());
    } else
        djnn_error (nullptr, "AbstractOpacity::impl_activate () - Unable to find frame");
}

void
AbstractOpacity::impl_deactivate ()
{
    disable (_ca);
    AbstractStyle::impl_deactivate ();
}

} /* namespace djnn */
