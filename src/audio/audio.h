#pragma once

#include "core/ontology/process.h"

#include <portaudio.h>

namespace djnn {
	void init_audio ();
	void clear_audio ();


	class AudioOut : public Process//, public ExternalSource
	{
	public:
		AudioOut (Process* parent, const string& name);
		virtual ~AudioOut ();
	protected:
		// Process
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;

    	// ExternalSource
		//virtual void run() ();

	private:
		PaStream *_pa_stream;
	};

}