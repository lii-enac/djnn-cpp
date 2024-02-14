#pragma once

#include "core/utils/containers.h"

namespace djnn {

extern djnnstl::vector<djnnstl::string> loadedModules;

void init_comms ();
void clear_comms ();

} // namespace djnn
