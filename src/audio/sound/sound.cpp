/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */
#include "sound.h"

namespace djnn {
Sound::Sound (CoreProcess* parent, const string& name)
    : AbstractPropSound (parent, name,
                         1.0,        // gain
                         1.0, 1.0,   // lowpass gain and freq
                         0., 0., 0., // position
                         1.0,        // pitch multiplier
                         0           // loop
      )
{
}

Sound::~Sound ()
{
}

} // namespace djnn