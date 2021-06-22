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

#include <vector>

#include "window.h"
#include "display/display-dev.h"

#define MACRO(lowkey,capkey) extern const int DJN_Key_ ## lowkey;

namespace djnn {
  extern vector<string> loadedModules;

  #include "const_keys.h"
  MACRO (Print, PRINT)
  MACRO (Shift, SHIFT)
  MACRO (Control, CONTROL)
  MACRO (Meta, META)
  MACRO (Alt, ALT)
  MACRO (AltGr, ALTGR)
  MACRO (NumLock, NUMLOCK)
  MACRO (Exclam, EXCLAM)
  MACRO (Apostrophe, APOSTROPHE)
  MACRO (ParenLeft, PARENLEFT)
  MACRO (ParenRight, PARENRIGHT)
  MACRO (Equal, EQUAL)

}

