/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "display/drm/drm_display.h"

namespace djnn {
static DRMUdev* udev;

#define MACRO(lowkey, capkey) const int DJN_Key_##lowkey = 0; // FIXME
#include "display/const_keys.h"
MACRO (Shift, SHIFT); // for multiline
MACRO (Control, CONTROL);
MACRO (Alt, ALT)
#undef MACRO

void
p_init_display ()
{
    udev = new DRMUdev ();
}
} // namespace djnn
