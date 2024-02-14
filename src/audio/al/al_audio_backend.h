#include "audio/audio_backend.h"
#include "audio/abstract_sobj.h"

namespace djnn {

  class ALAudioBackend : public AudioBackend {
  public:
    virtual void sound (Sample* s);
    virtual void load (Volume* s);
  };

  class ALSoundImpl : public AbstractSObjImpl {
  public:
    virtual ~ALSoundImpl() {}

    /*void load_wav(const string& path);
    unsigned int bufferid, format;
    int channel, sampleRate, bps;
    unsigned int sourceid;

    int size;

    char * data;*/

  };

  class ALTransformationImpl : public AbstractSObjImpl {
  public:
    virtual ~ALTransformationImpl() {}
    //bool damaged;
  };

  //using GLStyleImpl = GLTransformationImpl;
}
