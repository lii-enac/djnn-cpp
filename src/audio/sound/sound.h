#pragma once

#include "audio/abstract_sobj.h"
#include "audio/sound/gen/abstract_prop_sound.h"

namespace djnn {


	class Sound : public AbstractPropSound
	{
	public:
		Sound (CoreProcess* parent, const string& name);
		virtual ~Sound ();
	protected:

	};

}