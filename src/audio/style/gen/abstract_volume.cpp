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

#include "audio/style/audio_abstract_style.h"

#include "abstract_volume.h"

namespace djnn {
AbstractVolume::AbstractVolume (CoreProcess* parent, const string& name, double volume)
    : AbstractAudioStyle (parent, name),
      raw_props{.volume = volume},
      _cvolume (nullptr)
{
}

AbstractVolume::~AbstractVolume ()
{
    remove_edge (_cvolume);
    delete _cvolume;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("volume");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
AbstractVolume::find_child_impl (const string& name)
{
    auto* res = AbstractAudioStyle::find_child_impl (name);
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

    if (name == "volume") {
        coupling    = &_cvolume;
        rawp_Double = &raw_props.volume;
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
AbstractVolume::get_properties_name () const
{
    static const vector<string> res = {
        "volume",
    };
    return res;
}

void
AbstractVolume::get_properties_values (double& volume)
{
    volume = raw_props.volume;
}

void
AbstractVolume::impl_activate ()
{
    AbstractAudioStyle::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_cvolume, _frame->damaged ());
    } else
        djnn_warning (nullptr, "AbstractVolume::impl_activate () - Unable to find frame\n\n");
}

void
AbstractVolume::impl_deactivate ()
{
    disable (_cvolume);
    AbstractAudioStyle::impl_deactivate ();
}

} /* namespace djnn */
