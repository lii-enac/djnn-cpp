/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "external_source.h"
#include "cpp-thread.h"
#include "cpp-mutex.h"
#include <mutex>

//#include "core/utils/iostream.h"
//#include "utils/debug.h"
//#include <thread>


namespace djnn {

    //static djnn_mutex_t * launch_mutex;
    static std::mutex * launch_mutex;

#ifndef __EMSCRIPTEN__
    thread_local
#endif
    std::atomic<bool>
    //ExternalSource:: // not in class ExternalSource as static anymore, because of mingw ld or binutils bug
    thread_local_cancelled;

    #if DJNN_USE_SDL_THREAD
    static int SDL_ThreadFunction(void* data)
    {
       ExternalSource * es = (ExternalSource*) data;
        es->private_run ();
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
        #if DJNN_USE_QT_THREAD
        if(_thread) {
            delete _thread;
            _thread = nullptr;
        }
        #endif
        #if DJNN_USE_STD_THREAD
        //if(_thread.joinable()) _thread.detach();
        #endif
      }

      void start() {
        #if DJNN_USE_STD_THREAD
        //if(_thread.joinable()) _thread.detach();
        // if(_thread.joinable())
        //     _thread.join();
        #endif

        _thread = 

        #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
        //djnn_thread_t (&ExternalSource::private_run, this);
        djnn_thread_t ([this]() {this->_es->ExternalSource::private_run();});
        //DBG;
        _thread.detach();
        #endif

        #if DJNN_USE_QT_THREAD
            #if (QT_VERSION>= QT_VERSION_CHECK(5,10,0))
        //DBG;
        QThread::create([this]() { this->_es->ExternalSource::private_run(); });
        //QObject::connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
        (*_thread).start();
        //DBG;
            #else
        #error dont remember
            #endif
        #endif
        #if DJNN_USE_SDL_THREAD
        SDL_CreateThread(SDL_ThreadFunction, "djnn thread", _es);
        #endif
      }

      void join () {
        #if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER || DJNN_USE_STD_THREAD
        _thread.join();
        #endif

        #if DJNN_USE_QT_THREAD
        (*_thread).wait();
        #endif

        #if DJNN_USE_SDL_THREAD
        SDL_WaitThread(_thread, nullptr);
        #endif
      }

      void please_stop() {
        #if DJNN_USE_BOOST_THREAD
        _thread.interrupt();
        #endif

        #if DJNN_USE_QT_THREAD
        if(_thread) {
           (*_thread).requestInterruption();
        }
        #endif

        terminate_thread ();
      }

      void terminate_thread() {
       #if DJNN_USE_SDL_THREAD
        //int threadReturnValue;
        //SDL_WaitThread(_impl->_thread, &threadReturnValue);
        SDL_DetachThread(_thread);
        #endif

        #if DJNN_USE_BOOST_THREAD
        //std::cerr << _thread.joinable() << std::endl;
        _thread.detach();
        //std::cerr << _thread.joinable() << std::endl;
        #endif

        #if DJNN_THREAD_IS_POINTER
        //_thread.detach ();
        _thread = nullptr;
        #endif
      }

    private:
        #if DJNN_THREAD_IS_POINTER
        std::atomic<djnn_thread_t*> _thread;
        //djnn_thread_t* _thread;
        #else
        djnn_thread_t _thread;
        #endif
	};

	ExternalSource::ExternalSource (const string& name)
    : cancelled(nullptr), _impl(new ExternalSource::Impl(this)), _name(name), _please_stop (false)
    {
    }

    ExternalSource::~ExternalSource ()
    {
        thread_local_cancelled = true;
        please_stop ();
        delete _impl;
    }

    void
    ExternalSource::init()
    {
        //std::cerr << __PRETTY_FUNCTION__<< std::endl;
        #if 1
        launch_mutex = new std::mutex;
        #else
        #if DJNN_USE_SDL_THREAD
        launch_mutex = SDL_CreateMutex();
        #else
        launch_mutex = new djnn_mutex_t();
        //std::cerr << launch_mutex << std::endl;
        #endif
        #endif
    }

    void
    ExternalSource::launch_mutex_lock()
    {
        //std::cerr << __PRETTY_FUNCTION__ << " " << &launch_mutex << std::flush;
        #if 0 //DJNN_USE_SDL_THREAD
        SDL_LockMutex(launch_mutex);
        #else
        //std::cerr << launch_mutex << std::endl;
        launch_mutex->lock();
        #endif
        //std::cerr << __PRETTY_FUNCTION__ << "got" << std::endl;
    }
    void
    ExternalSource::launch_mutex_unlock()
    {
        //std::cerr << __PRETTY_FUNCTION__ << " " << &launch_mutex << std::flush;
        #if 0 //DJNN_USE_SDL_THREAD
        SDL_UnlockMutex(launch_mutex);
        #else
        //std::cerr << launch_mutex << std::endl;
        launch_mutex->unlock();
        #endif
        //std::cerr << __PRETTY_FUNCTION__ << "rel" << std::endl;
    }

    void
    ExternalSource::start()
    {
        set_please_stop (false);
        start_thread ();
    }

    void
    ExternalSource::join()
    {
        _impl->join ();
    }

	void
	ExternalSource::please_stop ()
	{
        if(get_please_stop ()) return;

        if(cancelled) *cancelled = true;
		set_please_stop (true);

        _impl->please_stop ();
	}

    bool
    ExternalSource::should_i_stop () const
    {
        return thread_local_cancelled || get_please_stop ();
    }

	void
	ExternalSource::start_thread ()
	{
        _impl->start();	
	}

	void
	ExternalSource::private_run ()
	{
        //std::cerr << "thread created " << _name << " " << __FL__; //<< std::this_thread::get_id() << __FL__;
        launch_mutex_lock();
        launch_mutex_unlock();

        cancelled = &thread_local_cancelled;
        *cancelled = false;

        run();

        cancelled = nullptr;
	}
	
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
#if 0 //defined(_WIN32)
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

#if 0
        auto native_thread = _impl->_thread.native_handle();
#if defined(_WIN32)
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
