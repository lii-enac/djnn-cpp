/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "main_loop.h"
#include "syshook.h"

#include "cpp-thread.h"

#if DJNN_USE_BOOST_THREAD || DJNN_USE_BOOST_FIBER
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
    std::cerr << debug << std::flush;
#endif

#if !DJNN_USE_BOOST_FIBER
    global_mutex->lock ();
#endif
    
#if DBG_MUTEX
    std::cerr << " GOT (" << debug << ")"
    //<< " priority:" << QThread::currentThread()->priority()
    << std::endl << std::flush;
#endif
  }

  void
  release_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    std::cerr << debug << std::flush;
#endif

#if !DJNN_USE_BOOST_FIBER
    global_mutex->unlock ();
#endif

#if DBG_MUTEX
    std::cerr << " ROL (" << debug << ")"
    << std::endl << std::flush;
#endif
  }

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
