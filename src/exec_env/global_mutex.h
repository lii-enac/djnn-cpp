#pragma once

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define DBG_GET (">> GET " __FILE__ ":" LINE_STRING)
#define DBG_REL ("<< REL " __FILE__ ":" LINE_STRING)

namespace djnn
{

	class FatProcess;

	void init_global_mutex ();
	bool is_global_mutex_inited ();

	void get_exclusive_access(const char* debug);
	void release_exclusive_access(const char* debug);

	void lock_ios_mutex ();
	void release_ios_mutex ();

	void start (FatProcess *c);
	void stop (FatProcess *c);
}
