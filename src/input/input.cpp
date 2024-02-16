/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/tree/set.h"
#include "core/utils/uri.h"
#include "input-priv.h"

namespace djnn {

static bool __module_initialized = false;

FatProcess* InputDevices = nullptr;
FatProcess* Mice         = nullptr;
FatProcess* TouchPanels  = nullptr;
FatProcess* GPIOs        = nullptr;

void
init_input ()
{

    if (__module_initialized == false) {

        __module_initialized = true;

        djnn::loadedModules.push_back ("input");

        InputDevices = new Set (nullptr, "InputDevices");
        InputDevices->set_activation_state (ACTIVATED);
        Mice = new Set (InputDevices, "Mice");
        Mice->set_activation_state (ACTIVATED);
        TouchPanels = new Set (InputDevices, "TouchPanels");
        TouchPanels->set_activation_state (ACTIVATED);
        GPIOs = new GPIOProxy (nullptr, "GPIO");
        URI::add_uri ("input", InputDevices);
        URI::add_uri ("gpio", GPIOs);
        p_init_input ();
    }
}

void
clear_input ()
{
}

CoreProcess*
GPIOProxy::find_child_impl (const string& path)
{
    int pos = 0;
    if (path.rfind ("in/", 0) == 0) {
        pos = 3;
    } else if (path.rfind ("out/", 0) == 0) {
        pos = 4;
    } else {
        return nullptr;
    }
    // string::size_type sz;
    const string buff = path.substr (pos);
    CoreProcess* c    = nullptr;
    if (pos == 3) {
        c = p_find_gpio (buff, IN);
    } else {
        c = p_find_gpio (buff, OUT);
    }
    return c;
}
} // namespace djnn
