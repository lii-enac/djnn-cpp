/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#ifndef _WIN32
#if DJNN_USE_QT_THREAD
#include <QtGlobal>
#if (QT_VERSION < QT_VERSION_CHECK(5,10,0))
#undef DJNN_USE_QT_THREAD
#define DJNN_USE_STD_THREAD 1
#endif
#endif
#endif

#if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER
	#include <boost/thread/mutex.hpp>
	namespace djnn {
		typedef boost::mutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 0
#endif

#if DJNN_USE_STD_THREAD
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
	#include <SDL_mutex.h>
	namespace djnn {
		typedef SDL_mutex djnn_mutex_t;
	}
	#define DJNN_MUTEX_IS_POINTER 1
#endif

