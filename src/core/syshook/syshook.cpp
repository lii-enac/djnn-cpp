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
#include "cpp-mutex-priv.h"

#include "core/syshook/djnn_time_manager.h"

#include <chrono>
#include <thread>

#define DBG_MUTEX 0
#include <iostream>
#include "utils/debug.h"

namespace djnn
{

  static djnn_mutex_t* global_mutex;
  static djnn_mutex_t* ios_mutex;

  void
  init_global_mutex() {
    global_mutex = create_lock();
    ios_mutex = create_lock();
  }
  
  void
  get_exclusive_access (const char * debug)
  {
#if DBG_MUTEX
    lock(ios_mutex);
    std::cerr << debug << " thread:" << std::this_thread::get_id() << " global mutex: " << global_mutex << std::flush;
    release(ios_mutex);
#endif

    lock(global_mutex);

    DjnnTimeManager::instance().update_ref_now();
    
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
    std::cerr << debug << " thread:" << std::this_thread::get_id() << " global mutex: " << global_mutex << std::flush;
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
