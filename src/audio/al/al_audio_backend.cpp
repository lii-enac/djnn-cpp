#include "al_audio_backend.h"

#include "openal.h"

#include <iostream>
#include "audio/al/al_debug.h"

#ifdef DJNN_USE_OPENAL_EFX
#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#endif

namespace djnn {

	static ALAudioBackend backend;

	AudioBackend &
	AudioBackend::instance ()
	{
		return backend;
	}

	// OpenAL is retained : it does not make sense to imitate the drawing backend
	// instead each sound has a gain, pitch_mul property etc.
	// and a timer updates the OpenAL parameters
	// there is not yet a Volume process, and connectors between a e.g. Volume process and the sources it applies to.

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
		device = alcOpenDevice (NULL); //CHKAL // no context yet
	    if (device == NULL)
	    {
	        std::cerr << "cannot open sound card" << std::endl;
	 		return;
	    }

		if (alcIsExtensionPresent (device, "ALC_EXT_EFX") == AL_FALSE)
		{
#ifdef DJNN_USE_OPENAL_EFX
	        std::cerr << "no OpenAL EFX" << std::endl;
#endif
	    }

		// ALint attribs[4] = { 0 };
		// attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		// attribs[1] = 4;

		context = alcCreateContext (device, NULL); //CHKAL; // no context yet
	    if (context == NULL)
	    {
	        std::cerr << "cannot open context" << std::endl;
	        return;
	    }

	    alcMakeContextCurrent (context); CHKAL;

	    std::cout << "Default device:" << alcGetString (NULL, ALC_DEFAULT_DEVICE_SPECIFIER) << std::endl; CHKAL;
		auto * v = alcGetString (0, AL_VENDOR); CHKAL;
		if(v)
			std::cout << "AL vendor: " << v << __FL__;

	    int major, minor;
		alcGetIntegerv (NULL, ALC_MAJOR_VERSION, 1, &major); CHKAL;
		alcGetIntegerv (NULL, ALC_MINOR_VERSION, 1, &minor); CHKAL;

		std::cout << "ALC version: " << major << " " << minor << std::endl;
		std::cout << std::flush;
	}

	void clear_audio_openal ()
	{
		alcDestroyContext (context); //CHKAL;
    	alcCloseDevice (device); //CHKAL;
	}
}
