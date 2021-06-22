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

#pragma once

#include <vector> // loaded modules

#include "abstract_display.h"

namespace djnn {

  extern int mouse_tracking;
  extern int full_screen;
  extern int hide_pointer;

  class FatProcess;

  extern FatProcess *GPUs;
  extern FatProcess *Displays;
  void init_display ();
  void clear_display ();

  class AbstractDisplay;
  extern FatProcess* DrawingRefreshManager;

  typedef enum mouse_button {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT
  } mouse_button;

  class FatProcess;

  class DisplayBackend
  {
  public:
    static AbstractDisplay* instance ();
    static void init ();
    static void clear ();
  private:
    //class Impl;
    class Impl
    {
      public:
      //SDLDisplayBackend* backend;
      AbstractDisplay* backend;
    };
    static Impl* _instance;
  };

  extern FatProcess *GPUs;
  extern FatProcess* DrawingRefreshManager;

}

#define MACRO(lowkey,capkey) extern const int DJN_Key_ ## lowkey;

namespace djnn {
  extern vector<djnn::string> loadedModules;

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
