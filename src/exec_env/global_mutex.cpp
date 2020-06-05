#include "global_mutex.h"
#include "cpp-mutex.h"
#include "cpp-mutex-priv.h"
#include "exec_env/djnn_time_manager.h"

// dbg
// #define DBG_MUTEX 1
// #include <iostream>
// #include <thread> // this_thread

namespace djnn
{
  static djnn_mutex_t* global_mutex;
  static djnn_mutex_t* ios_mutex;

  void
  init_global_mutex() {
    global_mutex = create_lock();
    ios_mutex = create_lock();
  }

  bool
  is_global_mutex_inited ()
  {
    return global_mutex != nullptr;
  }

  void
  lock_ios_mutex () {
    lock(ios_mutex);
  }

	void
  release_ios_mutex () {
    release(ios_mutex);
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