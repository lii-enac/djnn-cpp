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

		device = alcOpenDevice(NULL); //CHKAL;
	    if (device == NULL)
	    {
	        std::cerr << "cannot open sound card" << std::endl;
	 		return;
	    }

		context = alcCreateContext(device, NULL); //CHKAL;
	    if (context == NULL)
	    {
	        std::cerr << "cannot open context" << std::endl;
	        return;
	    }

	    alcMakeContextCurrent(context); CHKAL;

	    std::cout << "Default device:" << alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER) << std::endl; CHKAL;

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
