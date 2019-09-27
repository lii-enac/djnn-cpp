#include "external_source.h"

#include "cpp-thread.h"
#include "cpp-mutex.h"

#include <iostream>
#define DBG std::cerr << __FUNCTION__ << " " << __FILE__ << ":" << __LINE__ << std::endl;

namespace djnn {

    #if DJNN_USE_SDL_THREAD
    static int SDL_ThreadFunction(void* data)
    {
        ExternalSource * es = (ExternalSource*) data;
        es->private_run ();
        //std::cerr << "end of thread " << __PRETTY_FUNCTION__ << " " << ExternalSource::thread_local_cancelled << " " << &ExternalSource::thread_local_cancelled << std::endl;
        return 0;
    }
    #endif

    class ExternalSource::Impl {
    public:
      ExternalSource * _es;

      Impl(ExternalSource* es) : _es(es) {
        #if DJNN_THREAD_IS_POINTER
        _thread = nullptr;
        #endif
      }

      ~Impl() {
        #if DJNN_THREAD_IS_POINTER
        if(_thread) {
        #else
        if(_thread.joinable()) {
        #endif
            
            #if DJNN_USE_QT_THREAD
            //_impl->_thread->wait();

            #elif DJNN_USE_SDL_THREAD
            //int threadReturnValue;
            //SDL_WaitThread(_impl->_thread, &threadReturnValue);
            SDL_DetachThread(_thread);

            #else
            #if DJNN_THREAD_IS_POINTER
            if ( _impl->_thread->joinable() ) _thread->join();
            #else
            if ( (_impl->_thread).joinable() ) (_thread).join();
            #endif
            #endif
        }

      }

      void start() {
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;
        #if DJNN_USE_STD_THREAD
        if(_impl->_thread.joinable()) _impl->_thread.detach();
        #endif

        _thread = 

        #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
        djnn_thread_t (&ExternalSource::private_run, this);
        #endif

        #if DJNN_USE_QT_THREAD //&& (QT_VERSION>= QT_VERSION_CHECK(5,10,0))
        QThread::create([this]() { this->_es->ExternalSource::private_run(); })
        ;
        //QObject::connect(_impl->_thread, SIGNAL(finished()), _impl->_thread, SLOT(deleteLater()));
        _thread->start();
        #endif

        #if DJNN_USE_SDL_THREAD
        //SDL_CreateThread(this->ExternalSource::private_run(), "djnn thread", this);
        SDL_CreateThread(SDL_ThreadFunction, "djnn thread", _es);
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

      void please_stop() {
        #if DJNN_USE_BOOST_THREAD
        _impl->_thread.interrupt();
        #endif

        #if DJNN_USE_QT_THREAD
        if(_thread) {
           _thread->requestInterruption();
        }
        #endif
      }

      void thread_terminated() {
        #if DJNN_THREAD_IS_POINTER
        _thread = nullptr;
        #endif
      }

    private:
        #if DJNN_THREAD_IS_POINTER
        djnn_thread_t * _thread;
        #else
        djnn_thread_t _thread;
        #endif
	};

	ExternalSource::ExternalSource ()
    : cancelled(nullptr), _impl(new ExternalSource::Impl(this)), _please_stop (false)
    {
    }

    ExternalSource::~ExternalSource ()
    {
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;
        thread_local_cancelled = true;
        please_stop ();
        delete _impl;
    }


    static djnn_mutex_t * launch_mutex;

    void
    ExternalSource::init()
    {
        //std::cerr << __PRETTY_FUNCTION__<< std::endl;
        #if DJNN_USE_SDL_THREAD
        launch_mutex = SDL_CreateMutex();
        #else
        launch_mutex = new djnn_mutex_t();
        //std::cerr << launch_mutex << std::endl;
        #endif
    }

    void
    ExternalSource::launch_mutex_lock()
    {
        //std::cerr << __PRETTY_FUNCTION__<< std::endl;
        #if DJNN_USE_SDL_THREAD
        SDL_LockMutex(launch_mutex);
        #else
        //std::cerr << launch_mutex << std::endl;
        launch_mutex->lock();
        #endif
        //std::cerr << __PRETTY_FUNCTION__<< std::endl;
    }
    void
    ExternalSource::launch_mutex_unlock()
    {
        //std::cerr << __PRETTY_FUNCTION__ << std::endl;
        #if DJNN_USE_SDL_THREAD
        SDL_UnlockMutex(launch_mutex);
        #else
        //std::cerr << launch_mutex << std::endl;
        launch_mutex->unlock();
        #endif
        //std::cerr << __PRETTY_FUNCTION__<< std::endl;
    }

    thread_local std::atomic<bool> ExternalSource::thread_local_cancelled;

	void
	ExternalSource::please_stop ()
	{
        if(get_please_stop ()) return;
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;

        if(cancelled) *cancelled = true;
		set_please_stop (true);

        _impl->please_stop ();
	}

    

	void
	ExternalSource::start_thread ()
	{
        _impl->start();
    	
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
    ExternalSource::thread_terminated ()
    {
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;

        _impl->thread_terminated();
    }

	void
	ExternalSource::private_run ()
	{
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;
        //set_thread_priority();
        cancelled = &ExternalSource::thread_local_cancelled;
        *cancelled = false;
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << cancelled << " " << *cancelled << std::endl;
        //DBG;
        launch_mutex_lock();
        launch_mutex_unlock();
		run();
	}
	
}
