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

#include "style/style_types.h"
#include "core/utils/containers.h"


namespace djnn {

  using namespace djnnstl;

  class FatProcess;

  extern vector<string> loadedModules;
  // extern int mouse_tracking;
  // extern int full_screen;
  // extern int hide_pointer;
  // extern FatProcess* DrawingRefreshManager;
  extern CoreProcess* GenericMouse;
  extern int z_order_enabled;

  void init_gui ();
  void clear_gui ();

  typedef unsigned int named_color_t;


  extern int djn__GrphIsInClip;

  typedef enum
  {
    djnStrokeUndef, djnStrokeNone, djnStrokeColor
  } djnStrokeType;


}
