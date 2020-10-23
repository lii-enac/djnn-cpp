#include "al_audio_backend.h"

#include "openal.h"

#include <iostream>
#include "audio/al/al_debug.h"

namespace djnn {

	static ALAudioBackend backend;

	AudioBackend &
	AudioBackend::instance ()
	{
		return backend;
	}

	// OpenAL is retained : it does not make sense to imitate the drawing backend
	// instead each sound should have a volume property etc.
	// or connectors between a Volume process and the sources it applies to.

	void
	ALAudioBackend::sound (Sample * s)
	{
	}

	void
	ALAudioBackend::load (Volume * s)
	{
	}




	ALCdevice* device;
	ALCcontext* context;

	void init_audio_openal ()
	{
		//CHKAL;

		device = alcOpenDevice(NULL); //CHKAL;
	    if (device == NULL)
	    {
	        std::cerr << "cannot open sound card" << std::endl;
	 		return;
	    }

		if (alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_FALSE)
		{
	        std::cerr << "no OpenAL EFX" << std::endl;
	    }

		context = alcCreateContext(device, NULL); //CHKAL;
	    if (context == NULL)
	    {
	        std::cerr << "cannot open context" << std::endl;
	        return;
	    }

	    alcMakeContextCurrent(context); CHKAL;

	    std::cout << "Default device:" << alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER) << std::endl; CHKAL;
		auto * v = alcGetString(0, AL_VENDOR); CHKAL;
		if(v)
			std::cout << "AL vendor: " << v << __FL__;

	    int major, minor;
		alcGetIntegerv(NULL, ALC_MAJOR_VERSION, 1, &major); CHKAL;
		alcGetIntegerv(NULL, ALC_MINOR_VERSION, 1, &minor); CHKAL;

		std::cout << "ALC version: " << major << " " << minor << std::endl;
		std::cout << std::flush;
	}

	void clear_audio_openal ()
	{
		alcDestroyContext(context); //CHKAL;
    	alcCloseDevice(device); //CHKAL;
	}
}
