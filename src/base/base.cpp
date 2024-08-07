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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "core/ontology/process.h" // vector etc.

namespace djnn {
using namespace djnnstl;

static bool           __module_initialized = false;
extern vector<string> loadedModules;

void
init_base ()
{

    if (__module_initialized == false) {

        __module_initialized = true;

        djnn::loadedModules.push_back ("base");
    }
}

void
clear_base ()
{
}

} // namespace djnn
