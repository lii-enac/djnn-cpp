#include "external_source.h"

#include <boost/thread/thread.hpp>

//#include <thread>
//#include <atomic>

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
		_impl->_thread =    
    	//std::thread (&Clock::run, this);
    	//interruptible_thread (&Clock::run, this);
    	thread_t (&ExternalSource::run, this);
	}
	
}