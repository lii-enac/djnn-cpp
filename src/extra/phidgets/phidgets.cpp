/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
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
#include "phidgets-priv.h"

namespace djnn {

static bool __module_initialized = false;

FatProcess* Phidgets      = nullptr;
FatProcess* InterfaceKits = nullptr;

void
init_phidgets ()
{

    if (__module_initialized == false) {

        __module_initialized = true;

        djnn::loadedModules.push_back ("phidgets");

        Phidgets = new Set (nullptr, "Phidgets");
        Phidgets->activate ();
        InterfaceKits = new Set (Phidgets, "InterfaceKits");
        InterfaceKits->activate ();
        URI::add_uri ("phidgets", Phidgets);
        p_init_phidgets ();
    }
}
} // namespace djnn
