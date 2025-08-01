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

#include "outline_cap_style.h"

namespace djnn {
OutlineCapStyle::OutlineCapStyle (CoreProcess* parent, const string& name, int cap)
    : AbstractStyle (parent, name),
      raw_props{.cap = cap},
      _ccap (nullptr)
{

    finalize_construction (parent, name);
}

OutlineCapStyle::~OutlineCapStyle ()
{
    remove_edge (_ccap);
    delete _ccap;

    /* origin_x and origin_y are always in _symtable for AbstractGShape */
    if (children_size () > 0) {
        symtable_t::iterator it;

        it = find_child_iterator ("cap");
        if (it != children_end ())
            delete it->second;
    }
}

CoreProcess*
OutlineCapStyle::find_child_impl (const string& name)
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

    if (name == "cap") {
        coupling    = &_ccap;
        rawp_Int    = &raw_props.cap;
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
OutlineCapStyle::get_properties_name () const
{
    static const vector<string> res = {
        "cap",
    };
    return res;
}

void
OutlineCapStyle::get_properties_values (int& cap)
{
    cap = raw_props.cap;
}

void
OutlineCapStyle::impl_activate ()
{
    AbstractStyle::impl_activate ();
    auto _frame = get_frame ();
    if (_frame) {
        enable (_ccap, _frame->damaged ());
    } else
        djnn_warning (nullptr, "OutlineCapStyle::impl_activate () - Unable to find frame\n\n");
}

void
OutlineCapStyle::impl_deactivate ()
{
    disable (_ccap);
    AbstractStyle::impl_deactivate ();
}

void
OutlineCapStyle::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_outline_cap_style (this);
    }
}

OutlineCapStyle*
OutlineCapStyle::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new OutlineCapStyle (nullptr, name, raw_props.cap);
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

} /* namespace djnn */
