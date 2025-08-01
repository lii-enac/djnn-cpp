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

#include "audio/abstract_sobj.h"
#include "audio/audio_backend.h"
#include "core/ontology/process.h"

namespace djnn {
class AbstractAudioStyle : public AbstractSObj {
  public:
    AbstractAudioStyle (CoreProcess* parent, const string& name)
        : AbstractSObj (parent, name) {}
    virtual ~AbstractAudioStyle () {};
    virtual void impl_activate () override {}
    virtual void impl_deactivate () override {}
};
} // namespace djnn