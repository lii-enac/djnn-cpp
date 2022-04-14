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
#include "core/control/blank.h"

namespace djnn {

	class Sample : public Sound {
	public:
		Sample (ParentProcess* parent, const string& name, const string& path);
		virtual ~Sample ();
	protected:
		// FatProcess
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;

    private:
	    Blank _end;

		// end_timer necessitates to recompute it whenever pitch_mul is changed, since duration is affected
		// since we periodically control the sound, we can check that the sound is still playing instead
		// keep the structure for future use anyway

		// struct end_timer : djnn_internal::Time::Timer {
		// 	Sample& s;
		// 	end_timer(Sample&s_) : s(s_){}
		// 	virtual void do_it(const djnn_internal::Time::duration& actualduration) override {
		// 		s.do_end();
		// 	}
		// };
		// end_timer _end_timer;
		struct control_timer : djnn_internal::Time::Timer {
			Sample& s;
			control_timer(Sample&s_) : s(s_){}
			virtual void do_it(const djnn_internal::Time::duration& actualduration) override {
				s.do_control();
			}
		};
		control_timer _control_timer;

		void do_end ();
		void do_control (); // and check if still playing
		void do_control_only ();

    	unsigned int bufferid;
		unsigned int sourceid;
#ifdef DJNN_USE_OPENAL_EFX
		unsigned int lowpassid;
#endif
		unsigned int duration_ms;
		bool looping;
	};

}