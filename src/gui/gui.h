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

#pragma once

#include "widgets/text_field.h"
#include "interaction/interaction.h"
#include "style/style.h"
#include "shape/shapes.h"
#include "transformation/transformations.h"
#include "layer.h"

namespace djnn {

  class FatProcess;

  extern std::vector<std::string> loadedModules;
  // extern int mouse_tracking;
  // extern int full_screen;
  // extern int hide_pointer;
  // extern FatProcess* DrawingRefreshManager;
  extern FatProcess* GenericMouse;

  void init_gui ();
  void clear_gui ();
}
