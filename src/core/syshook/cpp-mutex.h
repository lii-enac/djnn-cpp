#pragma once

#if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER
	#include <boost/thread/mutex.hpp>
	namespace djnn {
		typedef boost::mutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 0
#endif

#if DJNN_USE_CPP_THREAD
	#include <mutex>
	namespace djnn {
		typedef std::mutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 0
#endif

#if DJNN_USE_PTHREAD
	#include <pthread.h>
#endif

#if DJNN_USE_QT_THREAD
	#include <QMutex>
	namespace djnn {
		typedef QMutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 0
#endif

#if DJNN_USE_SDL_THREAD
	#include <SDL2/SDL.h>
	namespace djnn {
		typedef SDL_mutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 1
#endif
