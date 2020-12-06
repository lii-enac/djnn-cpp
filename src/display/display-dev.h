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

//#include "display.h"

#include "core/ontology/process.h"
#include "abstract_display.h"

namespace djnn {
  typedef enum mouse_button {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT
  } mouse_button;

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

  extern FatProcess* DrawingRefreshManager;

}
