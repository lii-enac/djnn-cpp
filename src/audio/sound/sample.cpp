/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "sample.h"
#include "audio/al/openal.h"

#include <iostream>
#include "audio/al/al_debug.h"
#include "utils/debug.h"

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);


namespace djnn {

	Sample::Sample(ParentProcess* parent, const std::string& name, const std::string& path)
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

	// FatProcess
	void
	Sample::impl_activate ()
	{
		alSourcei(sourceid, AL_BUFFER, bufferid); CHKAL;
    	alSourcePlay(sourceid); CHKAL;
    	djnn_internal::Time::duration d = std::chrono::milliseconds(1000*((size/channel)/2)/sampleRate);
    	DjnnTimeManager::instance().schedule(this, d);
	}

	void
	Sample::impl_deactivate ()
	{
		alSourceStop(sourceid); CHKAL;
		if(is_already_scheduled ()) {
      		DjnnTimeManager::instance().cancel(this);
    	}
	}

	// djnn_internal::Time::Timer
	void
  	Sample::do_it(const djnn_internal::Time::duration& actualduration)
  	{
    	if(somehow_activating()) {
      		set_activation_state (DEACTIVATED);
      		alSourceStop(sourceid); CHKAL;
      		_end.notify_activation ();
    	}
  	}

}
