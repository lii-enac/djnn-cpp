#pragma once

// useless for now

namespace djnn {

	class Sample;

	class AudioBackend {
	public:
		virtual void play (Sample* s) = 0;

		static AudioBackend &instance ();
	};
}
