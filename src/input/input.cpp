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

namespace djnn {
  Process *Mice = nullptr;

  void
  init_input () {
    Mice = new Set (nullptr, "Mice");
    p_init_input ();
  }
}
