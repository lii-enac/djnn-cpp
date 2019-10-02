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

#include "syshook.h"

#include "cpp-mutex.h"

#include <chrono>
#include <iostream>

#include <thread>

#define DBG_MUTEX 0

namespace djnn
{

  static djnn_mutex_t* global_mutex;
  static djnn_mutex_t* ios_mutex;

  
  inline djnn_mutex_t* create_lock() {
    #if DJNN_USE_BOOST_FIBER
    return nullptr;
    #elif DJNN_USE_SDL_THREAD
    return SDL_CreateMutex();
    #else
    return new djnn_mutex_t();
    #endif
  }

  inline void lock(djnn_mutex_t* m) {
    #if DJNN_USE_BOOST_FIBER
    // do nothing
    #elif DJNN_USE_SDL_THREAD
    SDL_LockMutex(m);
    #else
    m->lock();
    #endif
  }

  inline void release(djnn_mutex_t* m) {
    #if DJNN_USE_BOOST_FIBER
    // do nothing
    #elif DJNN_USE_SDL_THREAD
    SDL_UnlockMutex(m);
    #else
    m->unlock();
    #endif
  }

  void
  init_global_mutex() {
    //std::cerr << __PRETTY_FUNCTION__ << std::endl;
    global_mutex = create_lock();
    ios_mutex = create_lock();
  }
  
  void
  get_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    lock(ios_mutex);
    std::cerr << debug << " thread:" << std::this_thread::get_id() << std::flush;
    release(ios_mutex);
#endif

    lock(global_mutex);
    
#if DBG_MUTEX
    lock(ios_mutex);
    std::cerr << " GOT (" << debug << ")"
    //<< " priority:" << QThread::currentThread()->priority()
    << std::endl << std::flush;
    release(ios_mutex);
#endif
  }

  void
  release_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    lock(ios_mutex);
    std::cerr << debug << std::flush;
    release(ios_mutex);
#endif

    release(global_mutex);

#if DBG_MUTEX
    lock(ios_mutex);
    std::cerr << " ROL (" << debug << ")"
    << std::endl << std::flush;
    release(ios_mutex);
#endif
  }

}
