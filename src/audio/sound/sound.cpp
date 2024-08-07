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