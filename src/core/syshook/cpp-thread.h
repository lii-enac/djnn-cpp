#pragma once

#ifndef __WIN32__
#if DJNN_USE_QT_THREAD
#include <QtGlobal>
#if (QT_VERSION < QT_VERSION_CHECK(5,10,0))
#undef DJNN_USE_QT_THREAD
#define DJNN_USE_STD_THREAD 1
#endif
#endif
#endif

#if DJNN_USE_BOOST_THREAD
	#include <boost/thread/thread.hpp>
	namespace djnn {
		namespace this_thread = boost::this_thread;
		typedef boost::thread djnn_thread_t;
    inline void sleep(int ms) {
      this_thread::sleep_for (boost::chrono::milliseconds(ms));
    }
	}
	#define DJNN_THREAD_IS_POINTER 0

#elif DJNN_USE_BOOST_FIBER
	#include <boost/fiber/all.hpp>
	namespace djnn {
		namespace this_thread = boost::this_fiber;
		typedef boost::fibers::fiber djnn_thread_t;
	}
	#define DJNN_THREAD_IS_POINTER 0

#elif DJNN_USE_STD_THREAD
	#include <thread>
  #include <unistd.h>
	namespace djnn {
		namespace this_thread = std::this_thread;
		typedef std::thread djnn_thread_t;
    inline void sleep(int ms) {
      ::usleep(1000*ms);
    }
	}
	#define DJNN_THREAD_IS_POINTER 0

#elif DJNN_USE_PTHREAD
	#include <pthread.h>

#elif DJNN_USE_QT_THREAD
	#include <QThread>
	namespace djnn {
		typedef QThread djnn_thread_t;
    inline void sleep(int ms) {
      QThread::currentThread()->usleep(1000*ms);
    }
	}
	#define DJNN_THREAD_IS_POINTER 1

#elif DJNN_USE_SDL_THREAD
	#include <SDL.h>
  #define DJNN_THREAD_IS_POINTER 1

	namespace djnn {
    typedef SDL_Thread djnn_thread_t;
    inline void sleep(int ms) {
      SDL_Delay(ms);
    }
	}

#endif

#if 0
  // form https://stackoverflow.com/questions/13893060/i-want-to-kill-a-stdthread-using-its-thread-object
  // Synopsis
  class interrupt_thread_exception;
  class interruptible_thread;
  void check_for_interrupt();

  // Interrupt exception
  class interrupt_thread_exception: public virtual std::exception {
  public:
      virtual char const* what() const noexcept(true) override { return "interrupt"; }
  }; // class interrupt_thread_exception

  // Interruptible thread
  class interruptible_thread {
  public:
      friend void check_for_interrupt();

      template <typename Function, typename... Args>
      interruptible_thread(Function&& fun, Args&&... args):
      _flag(false),
          _thread([](std::atomic_bool& f, Function&& fun, Args&&... args) {
                      _flag_ref = &f; fun(std::forward<Args>(args)...);
                  },
                  _flag,
                  std::forward<Function>(fun),
                  std::forward<Args>(args)...)
      {}

      bool stopping() const { return _flag.load(); }

      void stop() { _flag.store(true); }

  private:
      static thread_local std::atomic_bool* _flag_ref;

      std::atomic_bool _flag;
      std::thread _thread;
  }; // class interruptible_thread

  // Interruption checker
  inline void check_for_interrupt() noexcept(false) {
      if (not interruptible_thread::_flag_ref) { return; }
      if (not interruptible_thread::_flag_ref->load()) { return; }

      throw interrupt_thread_exception();
  } // check_for_interrupt
#endif
