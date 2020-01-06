#pragma once

#include "sound.h"

namespace djnn {

	class Sample : public Sound
	{
	public:
		Sample (Process* parent, const string& name, const string& path);
		virtual ~Sample ();
	protected:
		// Process
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;

    	unsigned int bufferid, format;
	    int channel, sampleRate, bps;
	    unsigned int sourceid;
	    int size;
	    char * data;

	private:
	};

}