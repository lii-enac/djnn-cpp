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
#include "../core/uri.h"

namespace djnn {
  Process *InputDevices = nullptr;
  Process *Mice = nullptr;
  Process *TouchPanels = nullptr;

  void
  init_input () {
    InputDevices = new Set (nullptr, "InputDevices");
    InputDevices->set_state (activated);
    Mice = new Set (InputDevices, "Mice");
    Mice->set_state (activated);
    TouchPanels = new Set (InputDevices, "TouchPanels");
    TouchPanels->set_state (activated);
    URI::add_uri ("input", InputDevices);
    p_init_input ();
  }
}
