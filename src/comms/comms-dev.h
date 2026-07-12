/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Magnaudet Mathieu <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
#pragma once

#include "core/utils/containers.h"

namespace djnn {

extern djnnstl::vector<djnnstl::string> loadedModules;

void init_comms ();
void clear_comms ();

} // namespace djnn
