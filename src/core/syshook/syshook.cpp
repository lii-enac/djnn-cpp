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
#include "external_source.h"

#include "cpp-mutex.h"

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
        #if DJNN_USE_SDL_THREAD
        SDL_LockMutex(global_mutex);
        #else
        global_mutex->lock();
        #endif
    //global_mutex->lock ();
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
        #if DJNN_USE_SDL_THREAD
        SDL_UnlockMutex(global_mutex);
        #else
        global_mutex->unlock();
        #endif
    //global_mutex->unlock ();
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
      #if DJNN_USE_SDL_THREAD
      global_mutex = SDL_CreateMutex();
      #else
      global_mutex = new djnn_mutex_t();
      #endif
      //global_mutex = new djnn_mutex_t ();
      ExternalSource::init();
      _instance = new MainLoop();
    });

    return *(_instance);
  }
}
