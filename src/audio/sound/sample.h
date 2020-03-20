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

#pragma once

#include "sound.h"
#include "exec_env/djnn_time_manager.h"
#include "core/tree/blank.h"

namespace djnn {

	class Sample : public Sound, public djnn_internal::Time::Timer
	{
	public:
		Sample (Process* parent, const std::string& name, const std::string& path);
		virtual ~Sample ();
	protected:
		// Process
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;

	    // djnn_internal::Time::Timer
    	virtual void do_it(const djnn_internal::Time::duration& actualduration) override;

    private:
	    Blank _end;

    	unsigned int bufferid, format;
	    int channel, sampleRate, bps;
	    unsigned int sourceid;
	    int size;
	};

}