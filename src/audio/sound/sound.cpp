#include "sound.h"

namespace djnn {
	Sound::Sound(ParentProcess* parent, const std::string& name)
	: AbstractPropSound(parent, name,
		1.0, // gain
		1.0, // lowpass_gain
		1.0, //lowpass_freq
		0., 0., 0., // position
		1.0 // pitch multiplier
		)
	{
	    
	}

	Sound::~Sound ()
	{
	}

	
}