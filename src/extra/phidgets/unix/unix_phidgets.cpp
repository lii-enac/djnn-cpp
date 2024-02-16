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

#include "unix_phidgets.h"

namespace djnn {
static PhidgetsMonitor* mon;
bool                    in_init;
void
p_init_phidgets ()
{
    in_init = true;
    mon     = new PhidgetsMonitor (nullptr, "PhidgetMonitor");
    mon->activate ();
    in_init = false;
}
} // namespace djnn
