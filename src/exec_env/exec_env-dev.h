#pragma once

namespace djnn {
	void get_monotonic_time (struct timespec *ts);
 	void t1 ();
	double t2 (const std::string &msg = "", bool display_stderr=false);
}

