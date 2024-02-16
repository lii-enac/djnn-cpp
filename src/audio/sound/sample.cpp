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

#include "audio/al/al_debug.h"
#include "audio/ext/dr_wav.h"
#include "core/utils/error.h"
#include "core/utils/to_string.h"
#include "utils/debug.h"
// #include "audio/ext/dr_mp3.h"

namespace djnn {

Sample::Sample (CoreProcess* parent, const string& name, const string& path)
    : Sound (parent, name),
      _end (this, "end"),
      // _end_timer(*this),
      _control_timer (*this),
#ifdef DJNN_USE_OPENAL_EFX
      lowpassid (0),
#endif
      looping (false)
{
    unsigned int   sampleRate, bps, channels, numFrames, datasize;
    unsigned char* data;

    drwav        wav;
    drwav_result res;
    if ((res = drwav_init_file (&wav, path.c_str (), NULL))) {
        sampleRate = wav.sampleRate;
        bps        = wav.bitsPerSample;
        channels   = wav.channels;
        numFrames  = wav.totalPCMFrameCount;
        datasize   = numFrames * (bps / 8) * channels;
        data       = new unsigned char[datasize];
        /*size = */
        drwav_read_pcm_frames (&wav, numFrames, data);
    } else {
        // drmp3 dr;
        // if (drmp3_init_file(&dr, path.c_str(), NULL)) {
        // 	sampleRate = dr.sampleRate;
        // 	bps = 16; //wav.bitsPerSample;
        // 	channels = dr.channels;
        // 	numFrames = dr.;
        // 	datasize = numFrames * (bps/8) * channels;
        // 	data = new unsigned char[datasize];
        // 	/*size = */
        // 	std::cerr << numFrames << std::endl;
        // 	drmp3_read_pcm_frames_s16 (&dr, numFrames, (drmp3_int16*)data);
        // } else {
        error (this, string ("cannot open audio sample file ") + path + "(dr_wav error " + djnnstl::to_string (res) + ")");
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
        warning (this, string ("audio sample file is not mono and will not be panned") + path);
        if (bps == 8) {
            format = AL_FORMAT_STEREO8;
        } else {
            format = AL_FORMAT_STEREO16;
        }
    }

    // std::cerr << std::hex << format << " " << std::dec << sampleRate << " " << bps << std::endl;
    duration_ms = 1000 * numFrames / sampleRate;

    alGenBuffers (1, &bufferid);
    CHKAL;
    alBufferData (bufferid, format, data, datasize, sampleRate);
    CHKAL;
    alGenSources (1, &sourceid);
    CHKAL;

    finalize_construction (parent, name);
    delete[] data;
}

Sample::~Sample ()
{
    alDeleteSources (1, &sourceid);
    CHKAL;
    alDeleteBuffers (1, &bufferid);
    CHKAL;
#ifdef DJNN_USE_OPENAL_EFX
    if (lowpassid) {
        alDeleteFilters (1, &lowpassid);
        CHKAL;
    }
#endif
}

// Process
void
Sample::impl_activate ()
{
    alSourcei (sourceid, AL_BUFFER, bufferid);
    CHKAL;
    looping = !raw_props.loop; // switch state in do_control
    do_control_only ();
    alSourcePlay (sourceid);
    CHKAL;
}

void
Sample::impl_deactivate ()
{
    alSourceStop (sourceid);
    CHKAL;

    // if(_end_timer.is_already_scheduled ()) {
    // 	DjnnTimeManager::instance ().cancel (&_end_timer);
    // }
    if (_control_timer.is_already_scheduled ()) {
        DjnnTimeManager::instance ().cancel (&_control_timer);
    }
}

void
Sample::do_end ()
{
    if (somehow_activating ()) {
        deactivate ();
        _end.schedule_activation ();
    }
}

// do_control is called every xx ms to get the updated property values in case they were updated
// not very efficient, but this avoids using connectors.
// it's also called once in impl_activate and (re)schedules itself
void
Sample::do_control ()
{
    ALint state;
    alGetSourcei (sourceid, AL_SOURCE_STATE, &state);
    CHKAL;
    if (state != AL_PLAYING) {
        do_end ();
        return;
    }
    do_control_only ();
}

void
Sample::do_control_only ()
{
    double gain, lowpass_gain, lowpass_freq, x, y, z, pitch_mul;
    int    loop;
    get_properties_values (gain, lowpass_gain, lowpass_freq, x, y, z, pitch_mul, loop);
    alSourcef (sourceid, AL_GAIN, gain);
    CHKAL;
    alSource3f (sourceid, AL_POSITION, x, y, z);
    CHKAL;
    alSourcef (sourceid, AL_PITCH, pitch_mul);
    CHKAL;

    // check change of loop, react accordingly if the value is different, do nothing otherwise
    if (raw_props.loop) {
        if (!looping) {
            looping = true;
            // DjnnTimeManager::instance ().cancel (&_end_timer);
            alSourcei (sourceid, AL_LOOPING, AL_TRUE);
            CHKAL;
        }
    } else {
        if (looping) {
            looping = false;
            // djnn_internal::Time::duration d1 = std::chrono::milliseconds (duration_ms);
            // DjnnTimeManager::instance ().schedule (&_end_timer, d1);
            alSourcei (sourceid, AL_LOOPING, AL_FALSE);
            CHKAL;
        }
    }

#ifdef DJNN_USE_OPENAL_EFX
    if ((lowpass_gain < 0.999 || lowpass_freq < 0.999) || lowpassid) {
        if (!lowpassid) {
            alGenFilters (1, &lowpassid);
            CHKAL;
            alFilteri (lowpassid, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
            CHKAL;
            if (alGetError () != AL_NO_ERROR) {
                error (nullptr, "Low Pass Filter not supported\n");
            } else {
                alSourcei (sourceid, AL_DIRECT_FILTER, lowpassid);
                CHKAL;
                alSourcei (sourceid, AL_DIRECT_FILTER_GAINHF_AUTO, AL_FALSE);
                CHKAL;
            }
        }
        // if (lowpassid) {
        // std::cerr << lowpassid << " " << lowpass_freq << " " << lowpass_gain << __FL__;
        alFilterf (lowpassid, AL_LOWPASS_GAIN, lowpass_gain);
        CHKAL;
        alFilterf (lowpassid, AL_LOWPASS_GAINHF, lowpass_freq);
        CHKAL;
        alSourcei (sourceid, AL_DIRECT_FILTER, lowpassid);
        CHKAL;
        //}
    }
#endif

    djnn_internal::Time::duration d2 = std::chrono::milliseconds (10);
    DjnnTimeManager::instance ().schedule (&_control_timer, d2);
}

} // namespace djnn
