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

#pragma once

#include <string>
#include <vector>

#include "../core/tree/process.h"

namespace djnn {

  extern std::vector<std::string> loadedModules;

  extern Process *GPUs;
  extern Process *Displays;
  void init_display ();

}