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

// useless for now

namespace djnn {

class Sample;
class Volume;

class AudioBackend {
  public:
    virtual void sound (Sample* s) = 0;
    virtual void load (Volume* v)  = 0;

    static AudioBackend& instance ();
};
} // namespace djnn
