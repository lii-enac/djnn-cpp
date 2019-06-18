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

#include "input-priv.h"
#include "../core/tree/set.h"
#include "../core/utils/uri.h"

namespace djnn {

  static bool __module_initialized = false;

  Process *InputDevices = nullptr;
  Process *Mice = nullptr;
  Process *TouchPanels = nullptr;

  void
  init_input () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("input");

      InputDevices = new Set (nullptr, "InputDevices");
      InputDevices->set_activation_state (ACTIVATED);
      Mice = new Set (InputDevices, "Mice");
      Mice->set_activation_state (ACTIVATED);
      TouchPanels = new Set (InputDevices, "TouchPanels");
      TouchPanels->set_activation_state (ACTIVATED);
      URI::add_uri ("input", InputDevices);
      p_init_input ();
    }

  }
}
