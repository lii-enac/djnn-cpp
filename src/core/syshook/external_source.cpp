#include "external_source.h"

#include "cpp-thread.h"


//#include <atomic>

//#if defined(__WIN32__)
//#include <windows.h>
//#endif

//typedef QThread *my_thread_t;

#include <iostream>
#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;


namespace djnn {


	class ExternalSource::Impl {
	public:
    	djnn_thread_t _thread;
	};

	ExternalSource::ExternalSource ()
    : _impl(new ExternalSource::Impl), _please_stop (false)
    {
        #if DJNN_USE_QTHREAD
        _impl->_thread = nullptr;
        #endif
    }

    ExternalSource::~ExternalSource () {
      //please_stop ();
      //if ( _thread.joinable() ) _thread.join();
      //delete _impl;
    }

	void
	ExternalSource::please_stop ()
	{
		_please_stop = true;

        #if DJNN_USE_BOOST_THREAD
		_impl->_thread.interrupt();
        #endif

        #if DJNN_USE_QTHREAD
        if(_impl->_thread) {
            _impl->_thread->requestInterruption();
        }
        #endif
        
	}

	void
	ExternalSource::start_thread ()
	{
		//DBG;
        //delete _impl;
        //auto prev_thread = _impl->_thread;
		_impl->_thread =    
    	//std::thread (&Clock::run, this);
    	//interruptible_thread (&Clock::run, this);
        #if DJNN_USE_BOOST_THREAD
    	djnn_thread_t (&ExternalSource::private_run, this);
        #endif

        #if DJNN_USE_CPP_THREAD
        djnn_thread_t (&ExternalSource::private_run, this);
        #endif

        #if DJNN_USE_QTHREAD
        //QThread (
        QThread::create([this]() { this->ExternalSource::private_run(); })
        //)
        ;
        QObject::connect(_impl->_thread, SIGNAL(finished()), _impl->_thread, SLOT(deleteLater()));
        _impl->_thread->start();
        #endif


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

#if 0
#if 0
static const char* th_err(int errmsg)
{
    switch(errmsg) {
        case ESRCH: return "No thread with the ID thread could be found"; break;
        case EINVAL: return "policy is not a recognized policy, or param does not make sense for the policy"; break;
        case EPERM: return ""; break;
        default: return "unknown pthread error"; break;
    }
}
#endif

    void
    ExternalSource::set_thread_priority ()
    {
#if 0 //defined(__WIN32__)
        auto native_thread = _impl->_thread.native_handle();
        std::cerr << native_thread << __EFL__;
        //if(native_thread==-1) native_thread = 
        //DBG;
        int priority = GetThreadPriority(native_thread);
        if(priority==THREAD_PRIORITY_ERROR_RETURN) {
            std::cerr << "fail to GetThreadPriority " << GetLastError() << " " << __FILE__ << ":" << __LINE__ << std::endl;
        } else {
            std::cerr << "thread priority: " << priority << __EFL__;
            #if 1
           auto b = SetThreadPriority(native_thread, THREAD_PRIORITY_NORMAL);
           if(!b) {
              std::cerr << "fail to SetThreadPriority " << GetLastError() << " " << __FILE__ << ":" << __LINE__ << std::endl;
           }
        }
        #endif
#endif
#if 0
        sched_param sch;
        int policy;
        int err;
        auto native_thread = pthread_self();
        err=pthread_getschedparam(native_thread, &policy, &sch);
        if (err) {
            std::cerr << "Failed to getschedparam: " << th_err(err) << __EFL__;
        } else {
            std::cout << "thread priority: " << sch.sched_priority << __EFL__;
            #if 0
            sch.sched_priority = 20;
            err=pthread_setschedparam(native_thread, SCHED_FIFO, &sch);
            if (err) {
                std::cerr << "Failed to setschedparam: " << th_err(err) << __FILE__ << ":" << __LINE__ << std::endl;;
            }
            #endif
        }
#endif
#if 0
    std::cerr << "qt thread priority " << QThread::currentThread()->priority () << __EFL__;
    QThread::currentThread()->setPriority(QThread::HighestPriority); 
    std::cerr << "qt thread priority " << QThread::currentThread()->priority () << __EFL__;
#endif
#endif

	void
	ExternalSource::private_run ()
	{	
        //set_thread_priority();
        //std::cerr << "run " << name << std::endl; 
        DBG;
		run();
	}
	
}
