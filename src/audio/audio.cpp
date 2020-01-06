#include "audio.h"

#include <portaudio.h>

namespace djnn {
	static int djn_PaCallback (const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

	static const unsigned int SAMPLE_RATE = 44100;
	static const unsigned int FRAMES_PER_BUFFER = 128;

	double djn__OutSoundBuffer[FRAMES_PER_BUFFER*2];
	int pos=0;

	static int
	djn_PaCallback (const void *inputBuffer, void *outputBuffer,
                unsigned long framesPerBuffer,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags, void *userData)
	{
		if (1) {
			float *out = (float*) outputBuffer;
			//djn_SoundProc ();
			unsigned int i;
			for (i = 0; i < FRAMES_PER_BUFFER; i++) {
				*out++ = djn__OutSoundBuffer[pos];
				*out++ = djn__OutSoundBuffer[pos + 1];
				pos += 2;
				if (pos >= FRAMES_PER_BUFFER * 2)
					pos = 0;
			}
		}
		return 0;
	}

	AudioOut::AudioOut (Process *parent, const string& name)
	: Process(name), _pa_stream (nullptr)
	{
		finalize_construction (parent, name);
	}

	AudioOut::~AudioOut ()
	{
	}

	// Process
	void
	AudioOut::impl_activate ()
	{
		fprintf (stderr, ">> portaudio activate\n");
		PaError err = Pa_OpenDefaultStream ( &_pa_stream, 0, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, djn_PaCallback, 0);
		if (err != paNoError) {
			fprintf (stderr, "WARNING: unable to open Port Audio Stream: %s\n", Pa_GetErrorText (err));
			Pa_Terminate ();
			return;
		}

		err = Pa_StartStream (_pa_stream);
		if (err != paNoError) {
			fprintf (stderr, "WARNING: unable to start Port Audio Stream: %s\n", Pa_GetErrorText (err));
			Pa_Terminate ();
			return;
		}
		fprintf (stderr, "portaudio ok\n");
	}

	void
	AudioOut::impl_deactivate ()
	{
		PaError err = Pa_StopStream (_pa_stream);
  		if (err != paNoError) {
    		fprintf (stderr, "WARNING: unable to stop Port Audio Stream: %s\n", Pa_GetErrorText (err));
  		} else {
    		err = Pa_CloseStream (_pa_stream);
    		if (err != paNoError) {
      			fprintf (stderr, "WARNING: unable to close Port Audio Stream: %s\n", Pa_GetErrorText (err));
    		}
  		}
	}
    
    // ExternalSource
	// void
	// AudioOut::run ()
	// {

	// }

	void
	init_audio ()
	{
		//fprintf (stderr, ">> portaudio init\n");
		PaError err = Pa_Initialize ();
		if ( err != paNoError) {
		    fprintf (stderr, "WARNING: unable to initialize Port Audio: %s\n", Pa_GetErrorText (err));
		    Pa_Terminate ();
		    return;
	  	}
	  	for(unsigned int i=0; i<FRAMES_PER_BUFFER*2; ++i) djn__OutSoundBuffer[i] = 0;
	  	//fprintf (stderr, "<< portaudio init\n");
	}

	void
	clear_audio ()
	{
		Pa_Terminate ();
	}
}
