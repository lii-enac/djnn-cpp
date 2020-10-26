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

#ifdef DJNN_USE_OPENAL_EFX
#define AL_ALEXT_PROTOTYPES
#include <AL/efx.h>
#endif

#include <iostream>
#include "audio/al/al_debug.h"
#include "utils/debug.h"
#include "core/utils/error.h"

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_CONVERSION_API
#include "audio/ext/dr_wav.h"

// #define DR_MP3_IMPLEMENTATION
// #define DR_MP3_NO_CONVERSION_API
// #include "audio/ext/dr_mp3.h"


namespace djnn {

	Sample::Sample(ParentProcess* parent, const std::string& name, const std::string& path)
	: Sound(parent, name),
	_end (this, "end"),
	_end_timer(*this),
	_control_timer(*this),
	lowpassid(0)
	{
		unsigned int sampleRate, bps, channels, numFrames, datasize;
		unsigned char * data;

		drwav wav;
    	if (drwav_init_file(&wav, path.c_str(), NULL)) {
			sampleRate = wav.sampleRate;
			bps = wav.bitsPerSample;
			channels = wav.channels;
			numFrames = wav.totalPCMFrameCount;
			datasize = numFrames * (bps/8) * channels;
			data = new unsigned char[datasize];
			/*size = */
			drwav_read_pcm_frames(&wav, numFrames, data);
		} else {
			// drmp3 wav;
    		// if (drmp3_init_file(&wav, path.c_str(), NULL)) {
			// 	sampleRate = wav.sampleRate;
			// 	bps = 16; //wav.bitsPerSample;
			// 	channels = wav.channels;
			// 	numFrames = wav.pcmFramesRemainingInMP3Frame;
			// 	datasize = numFrames * (bps/8) * channels;
			// 	data = new unsigned char[datasize];
			// 	/*size = */
			// 	drmp3_read_pcm_frames_s16(&wav, numFrames, (drmp3_int16*)data);
			// } else {
				error (this, std::string("Error opening audio sample file ") + path);
				return;
			// }
		}

		unsigned int format;
		if (channels == 1) {
	        if (bps == 8) {
	            format = AL_FORMAT_MONO8;
	        } else {
	            format = AL_FORMAT_MONO16;
	        }
	    } else {
	        if (bps == 8) {
	            format = AL_FORMAT_STEREO8;
	        } else {
	            format = AL_FORMAT_STEREO16;
	        }
	    }
	
		//std::cerr << std::hex << format << " " << std::dec << sampleRate << " " << bps << " " << size << std::endl;
		duration = numFrames / sampleRate;

	    alGenBuffers (1, &bufferid); CHKAL;
	    alBufferData (bufferid, format, data, datasize, sampleRate); CHKAL;
	    alGenSources (1, &sourceid); CHKAL;

	    finalize_construction(parent, name);
	    delete [] data;
	}

	Sample::~Sample ()
	{
		alDeleteSources(1, &sourceid); CHKAL;
	    alDeleteBuffers(1, &bufferid); CHKAL;
#ifdef DJNN_USE_OPENAL_EFX
		if (lowpassid) {
			alDeleteFilters(1,&lowpassid); CHKAL;
		}
#endif
	}

	// FatProcess
	void
	Sample::impl_activate ()
	{
		alSourcei(sourceid, AL_BUFFER, bufferid); CHKAL;
		do_control ();
    	alSourcePlay(sourceid); CHKAL;
		
    	djnn_internal::Time::duration d1 = std::chrono::milliseconds(1000*duration);
		DjnnTimeManager::instance ().schedule (&_end_timer, d1);
	}

	void
  	Sample::do_control ()
	{
		double gain, lowpass_gain, lowpass_freq, x, y, z, pitch_mul;
		get_properties_values (gain, lowpass_gain, lowpass_freq, x, y, z, pitch_mul);
		alSourcef(sourceid, AL_GAIN, gain); CHKAL;
		alSource3f(sourceid, AL_POSITION, x,y,z); CHKAL;
		alSourcef(sourceid, AL_PITCH, pitch_mul); CHKAL;

#ifdef DJNN_USE_OPENAL_EFX
		if( (lowpass_gain<0.999 || lowpass_freq<0.999) || lowpassid) {
			if (!lowpassid) {
				alGenFilters (1, &lowpassid); CHKAL;
				alFilteri (lowpassid, AL_FILTER_TYPE, AL_FILTER_LOWPASS); CHKAL;
				if (alGetError () != AL_NO_ERROR) printf("Low Pass Filter not supported\n");
				else {
					alSourcei (sourceid, AL_DIRECT_FILTER, lowpassid); CHKAL;
				}
			}
			if (lowpassid) {
				//std::cerr << lowpassid << " " << lowpass_freq << " " << lowpass_gain << __FL__;
				alFilterf (lowpassid, AL_LOWPASS_GAIN, lowpass_gain); CHKAL;
				alFilterf (lowpassid, AL_LOWPASS_GAINHF, lowpass_freq); CHKAL;
			}
		}
#endif
		djnn_internal::Time::duration d2 = std::chrono::milliseconds (10);
		DjnnTimeManager::instance ().schedule (&_control_timer, d2);
	}

	void
	Sample::impl_deactivate ()
	{
		alSourceStop(sourceid); CHKAL;

		if(_end_timer.is_already_scheduled ()) {
      		DjnnTimeManager::instance ().cancel (&_end_timer);
    	}
		if(_control_timer.is_already_scheduled ()) {
      		DjnnTimeManager::instance ().cancel (&_control_timer);
    	}
	}

	void
  	Sample::do_end()
  	{
    	if(somehow_activating()) {
      		deactivate ();
      		_end.notify_activation ();
    	}
  	}

}
