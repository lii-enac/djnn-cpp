#include "audio-priv.h"

#ifdef DJNN_USE_OPENAL
#include "audio/al/openal.h"

#include "audio/al/al_debug.h"
#include "utils/debug.h"
#endif

#include "abstract_sobj.h"

#include "core/execution/graph.h" // for enable

#include <iostream>

namespace djnn {

	FatProcess * default_audio_listener;

	void init_audio ()
	{
		#ifdef DJNN_USE_OPENAL
		void init_audio_openal ();
		init_audio_openal();

		default_audio_listener = new AudioListener (nullptr, "default_audio_listener");
		default_audio_listener->activate ();

		#endif
	}

	void clear_audio ()
	{
		#ifdef DJNN_USE_OPENAL
		
		default_audio_listener->deactivate ();
		delete default_audio_listener;

		void clear_audio_openal ();
		clear_audio_openal();
		#endif
	}

	AudioListener::AudioListener (ParentProcess* parent, const std::string& name)
	: FatProcess(name), _gain(this, "gain", 1.0),
	_gain_action (this, "gain_action"),
  	_c_gain_action (&_gain, ACTIVATION, &_gain_action, ACTIVATION, true)
	{
		finalize_construction (parent, name);
	}

	void
	AudioListener::update_gain ()
	{
		double gain = _gain.get_value();
		#ifdef DJNN_USE_OPENAL
		alListenerf(AL_GAIN, gain); CHKAL;
		#endif
	}

	AbstractSObjImpl::~AbstractSObjImpl() {}

}
