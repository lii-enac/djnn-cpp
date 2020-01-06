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

	void
	ALAudioBackend::play(Sample * s)
	{
	}

	ALCdevice* device;
	ALCcontext* context;

	void init_audio_openal ()
	{
		//CHKAL;
		int major, minor;
		alcGetIntegerv(NULL, ALC_MAJOR_VERSION, 1, &major);
		alcGetIntegerv(NULL, ALC_MINOR_VERSION, 1, &minor);

		//assert(major == 1);

		printf("ALC version: %i.%i\n", major, minor);
		printf("Default device: %s\n", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));

		device = alcOpenDevice(NULL); CHKAL;
	    if (device == NULL)
	    {
	        std::cout << "cannot open sound card" << std::endl;
	 		return;
	    }
	    context = alcCreateContext(device, NULL); CHKAL;
	    if (context == NULL)
	    {
	        std::cout << "cannot open context" << std::endl;
	        return;
	    }
	    alcMakeContextCurrent(context); CHKAL;
	}

	void clear_audio_openal ()
	{
		alcDestroyContext(context); CHKAL;
    	alcCloseDevice(device); CHKAL;
	}
}
