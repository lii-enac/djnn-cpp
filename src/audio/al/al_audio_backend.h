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
#include "audio/abstract_sobj.h"
#include "audio/audio_backend.h"

namespace djnn {

class ALAudioBackend : public AudioBackend {
  public:
    virtual void sound (Sample* s);
    virtual void load (Volume* s);
};

class ALSoundImpl : public AbstractSObjImpl {
  public:
    virtual ~ALSoundImpl () {}

    /*void load_wav(const string& path);
    unsigned int bufferid, format;
    int channel, sampleRate, bps;
    unsigned int sourceid;

    int size;

    char * data;*/
};

class ALTransformationImpl : public AbstractSObjImpl {
  public:
    virtual ~ALTransformationImpl () {}
    // bool damaged;
};

// using GLStyleImpl = GLTransformationImpl;
} // namespace djnn
