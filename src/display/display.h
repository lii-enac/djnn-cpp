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

#include "../core/ontology/process.h"
#include "window.h"

#define MACRO(key) extern const int DJN_Key_ ## key;

namespace djnn {

  #include "const_keys.h"

  extern int mouse_tracking;
  extern int full_screen;
  extern int hide_pointer;

  extern std::vector<std::string> loadedModules;

  extern Process *GPUs;
  extern Process *Displays;
  void init_display ();
  void clear_display ();

  class AbstractDisplay;

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

}