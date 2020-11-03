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

	class Sample : public Sound {
	public:
		Sample (ParentProcess* parent, const std::string& name, const std::string& path);
		virtual ~Sample ();
	protected:
		// FatProcess
		virtual void impl_activate () override;
    	virtual void impl_deactivate () override;

    private:
	    Blank _end;

		struct end_timer : djnn_internal::Time::Timer {
			Sample& s;
			end_timer(Sample&s_) : s(s_){}
			virtual void do_it(const djnn_internal::Time::duration& actualduration) override {
				s.do_end();
			}
		};
		end_timer _end_timer;
		struct control_timer : djnn_internal::Time::Timer {
			Sample& s;
			control_timer(Sample&s_) : s(s_){}
			virtual void do_it(const djnn_internal::Time::duration& actualduration) override {
				s.do_control();
			}
		};
		control_timer _control_timer;

		void do_end ();
		void do_control ();

    	unsigned int bufferid;
		unsigned int sourceid;
		unsigned int lowpassid;

		unsigned int duration_ms;
		bool looping;
	};

}