#include "external_source.h"

#include <boost/thread/thread.hpp>

//#include <thread>
//#include <atomic>

#include <pthread.h>

#include <iostream>
#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

#if defined(__WIN32__)
#include <windows.h>
#endif

namespace djnn {

	typedef boost::thread thread_t;

	class ExternalSource::Impl {
	public:
    	thread_t _thread;
	};

	ExternalSource::ExternalSource ()
    : _impl(new ExternalSource::Impl), _please_stop (false)
    {}

    ExternalSource::~ExternalSource () {
      //please_stop ();
      //if ( _thread.joinable() ) _thread.join();
    	delete _impl;
    }

	void
	ExternalSource::please_stop ()
	{
		_please_stop = true;
		_impl->_thread.interrupt();
	}

	void
	ExternalSource::start_thread ()
	{
		//DBG;
		_impl->_thread =    
    	//std::thread (&Clock::run, this);
    	//interruptible_thread (&Clock::run, this);
    	thread_t (&ExternalSource::private_run, this);
#if 0
    	auto native_thread = _impl->_thread.native_handle();
#if defined(__WIN32__)
    	//DBG;
    	auto b = SetThreadPriority(native_thread, THREAD_PRIORITY_NORMAL);
    	if(!b) {
    		std::cerr << "fail to SetPriorityClass " << GetLastError() << " " << __FILE__ << ":" << __LINE__ << std::endl;
    	}
#else
    	sched_param sch;
    	int policy; 
    	pthread_getschedparam(native_thread, &policy, &sch);
    	sch.sched_priority = 20;
    	if (pthread_setschedparam(native_thread, SCHED_FIFO, &sch)) {
        	std::cout << "Failed to setschedparam: " << std::strerror(errno) << '\n';
    	}
#endif
#endif
    	
	}

	void
	ExternalSource::private_run ()
	{	
		run();
	}
	
}
