#pragma once

#if DJNN_USE_BOOST_THREAD
	#include <boost/thread/thread.hpp>
	namespace djnn {
		namespace this_thread = boost::this_thread;
		typedef boost::thread djnn_thread_t;
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
	namespace djnn {
		namespace this_thread = std::this_thread;
		typedef std::thread djnn_thread_t;
	}
	#define DJNN_THREAD_IS_POINTER 0

#elif DJNN_USE_PTHREAD
	#include <pthread.h>

#elif DJNN_USE_QT_THREAD
	#include <QThread>
	namespace djnn {
		typedef QThread djnn_thread_t;
	}
	#define DJNN_THREAD_IS_POINTER 1

#elif DJNN_USE_SDL_THREAD
	#include <SDL2/SDL.h>
	namespace djnn {
		typedef SDL_Thread djnn_thread_t;
	}
	#define DJNN_THREAD_IS_POINTER 1

#endif
