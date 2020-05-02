#pragma once

#include "audio/sound/sample.h"
#include "audio/style/audio_style.h"

namespace djnn {
	extern FatProcess * default_audio_listener;

	void init_audio ();
	void clear_audio ();
}