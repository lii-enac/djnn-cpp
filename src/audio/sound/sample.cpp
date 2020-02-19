#include "sample.h"

#include "audio/al/openal.h"

#include <iostream>
#include "audio/al/al_debug.h"
#include "utils/debug.h"

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);


namespace djnn {

	Sample::Sample(Process* parent, const string& name, const string& path)
	: Sound(parent, name),
	_end (this, "end")
	{
		char* data = loadWAV(path.c_str(), channel, sampleRate, bps, size);
		if (channel == 1)
	    {
	        if (bps == 8)
	        {
	            format = AL_FORMAT_MONO8;
	        }
	        else {
	            format = AL_FORMAT_MONO16;
	        }
	    } else {
	        if (bps == 8)
	        {
	            format = AL_FORMAT_STEREO8;
	        }
	        else {
	            format = AL_FORMAT_STEREO16;
	        }
	    }
	    alGenBuffers(1, &bufferid); CHKAL;
	    alBufferData(bufferid, format, data, size, sampleRate); CHKAL;
	    alGenSources(1, &sourceid); CHKAL;

	    finalize_construction(parent, name);
	    delete [] data;
	}

	Sample::~Sample ()
	{
		alDeleteSources(1, &sourceid); CHKAL;
	    alDeleteBuffers(1, &bufferid); CHKAL;
	}

	void
	Sample::impl_activate ()
	{
		alSourcei(sourceid, AL_BUFFER, bufferid); CHKAL;
    	alSourcePlay(sourceid); CHKAL;
    	auto duration = 1000.*((size/channel)/2)/sampleRate;
    	DjnnTimeManager::instance().after(this, duration);
	}

	void
	Sample::impl_deactivate ()
	{
		alSourceStop(sourceid); CHKAL;
	}

	// djnn_internal::Time::Timer
	void
  	Sample::doit(const djnn_internal::Time::Unit& actualtime)
  	{
    	if(somehow_activating()) {
      		set_activation_state (DEACTIVATED);
      		Sample::impl_deactivate ();
      		_end.notify_activation (); // propagating
    	}
  	}

}
