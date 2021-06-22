/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

namespace djnn {
	extern FatProcess *mainloop;
	void init_exec_env();
	void clear_exec_env();

	void get_monotonic_time (struct timespec *ts);
 	void t1 ();
	double t2 (const djnn::string& msg = "", bool display_stderr=false);
}

