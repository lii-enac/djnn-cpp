#pragma once

#include "sound.h"
#include "core/syshook/djnn_time_manager.h"
#include "core/tree/blank.h"

namespace djnn {

	class Sample : public Sound, public djnn_internal::Time::Timer
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

	    // djnn_internal::Time::Timer
    	virtual void doit(const djnn_internal::Time::Unit& actualtime) override;

	    Blank _end;
    	
	private:
	};

}