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
#pragma once

#include "audio/abstract_sobj.h"
#include "audio/sound/gen/abstract_prop_sound.h"

namespace djnn {

class Sound : public AbstractPropSound {
  public:
    Sound (CoreProcess* parent, const string& name);
    virtual ~Sound ();

  protected:
};

} // namespace djnn