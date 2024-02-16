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
