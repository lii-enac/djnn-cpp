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

#include "input/input-priv.h"
#include "core/utils/error.h"

namespace djnn {
  void
  p_init_input () {
  }
  CoreProcess* p_find_gpio (const djnn::string& path, direction_e dir)
  {
    warning (nullptr, "GPIOs not supported on this platform");
    return nullptr;
  }
}
