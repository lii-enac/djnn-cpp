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

#include "../core/core.h"
//#include "window.h"
#include "interaction/interaction.h"
#include "style/style.h"
#include "shapes/shapes.h"
#include "transformation/transformations.h"

namespace djnn {

  extern std::vector<string> loadedModules;
  // extern int mouse_tracking;
  // extern int full_screen;
  // extern int hide_pointer;
  extern Process* DrawingRefreshManager;
  extern Process* GenericMouse;

  void init_gui ();
  void init_svg_parser ();
  void clear_gui ();
}
