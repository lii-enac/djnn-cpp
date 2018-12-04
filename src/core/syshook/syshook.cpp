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

#include "main_loop.h"
#include "syshook.h"


#if defined(__WIN32__)
#define DJNN_USE_QTHREAD 1
#else
#define DJNN_USE_BOOST_THREAD 1
#endif

#if DJNN_USE_BOOST_THREAD
#include <boost/thread/mutex.hpp>
typedef boost::mutex djnn_mutex_t;
#endif

#if DJNN_USE_CPP_THREAD
#include <mutex>
typedef std::thread djnn_mutex_t;
#endif

#if DJNN_USE_PTHREAD
#include <pthread.h>
#endif

#if DJNN_USE_QTHREAD
#include <QMutex>
typedef QMutex djnn_mutex_t;
#endif


#include <chrono>
#include <iostream>

#define DBG_MUTEX 0

namespace djnn
{

  static djnn_mutex_t* global_mutex;
  //thread_local bool _please_stop;
  
  void
  get_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    //std::cerr << debug << " priority:" << QThread::currentThread()->priority() << std::flush;
#endif
    global_mutex->lock ();
#if DBG_MUTEX
    std::cerr << " GOT " << debug << " priority:" << QThread::currentThread()->priority() << std::endl << std::flush;
#endif
  }

  void
  release_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    //std::cerr << debug << std::flush;
#endif
    global_mutex->unlock ();
#if DBG_MUTEX
    //std::cerr << " ROL " << debug << std::endl << std::flush;
#endif
  }

  ExternalSource * MainLoop::another_source_wants_to_be_mainloop = nullptr;

  MainLoop* MainLoop::_instance;
  std::once_flag MainLoop::onceFlag;
  MainLoop&
  MainLoop::instance ()
  {
    std::call_once (MainLoop::onceFlag, [] () {
      global_mutex = new djnn_mutex_t ();      
      _instance = new MainLoop();
    });

    return *(_instance);
  }
}
