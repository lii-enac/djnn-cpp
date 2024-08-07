/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 */

#pragma once

#include "audio/audio_backend.h"
#include "core/ontology/process.h"
#include "gen/abstract_volume.h"

namespace djnn {
class Volume : public AbstractVolume {
  public:
    Volume (CoreProcess* parent, const string& name, double volume)
        : AbstractVolume (parent, name, volume) {
        finalize_construction (parent, name);
    }

    void sound () {
        if (somehow_activating ()) {
            AudioBackend::instance ().load (this);
        }
    }
};

} // namespace djnn