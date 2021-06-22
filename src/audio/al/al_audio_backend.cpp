#include "al_audio_backend.h"

#include "openal.h"
#include "audio/al/al_debug.h"

#include "core/utils/error.h"
#include "core/utils/utils-dev.h"

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
	        error (nullptr, "cannot open sound card");
	 		return;
	    }

		if (alcIsExtensionPresent (device, "ALC_EXT_EFX") == AL_FALSE)
		{
#ifdef DJNN_USE_OPENAL_EFX
	        error (nullptr, "no OpenAL EFX");
#endif
	    }

		// ALint attribs[4] = { 0 };
		// attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		// attribs[1] = 4;

		context = alcCreateContext (device, NULL); //CHKAL; // no context yet
	    if (context == NULL)
	    {
	        error (nullptr, "cannot open context");
	        return;
	    }

	    alcMakeContextCurrent (context); CHKAL;

	    loginfo (string("Default device:") + alcGetString (NULL, ALC_DEFAULT_DEVICE_SPECIFIER)); CHKAL;
		auto * v = alcGetString (0, AL_VENDOR); CHKAL;
		if(v) {
			loginfo ( string("AL vendor: ") + v);
		}

	    int major, minor;
		alcGetIntegerv (NULL, ALC_MAJOR_VERSION, 1, &major); CHKAL;
		alcGetIntegerv (NULL, ALC_MINOR_VERSION, 1, &minor); CHKAL;

		loginfo ("ALC version: " + __to_string(major) + " " + __to_string (minor));
	}

	void clear_audio_openal ()
	{
		alcDestroyContext (context); //CHKAL;
    	alcCloseDevice (device); //CHKAL;
	}
}
