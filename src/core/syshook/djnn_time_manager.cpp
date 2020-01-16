#include "djnn_time_manager.h"

#include "core/syshook/syshook.h"
#include "core/tree/int_property.h"
#include "core/syshook/cpp-thread.h"
#include "core/execution/graph.h"

#include <iostream>
#include <cassert>
#include "utils/debug.h"

namespace djnn {

  DjnnTimeManager::DjnnTimeManager()
  : _acc_duration(0)
  {
  }

  DjnnTimeManager& DjnnTimeManager::instance () {
    return (_instance);
  }
  DjnnTimeManager DjnnTimeManager::_instance;


  struct timespec before;
  void DjnnTimeManager::firstTimerHasChanged()
  {
    //DBG;
    djnn::get_monotonic_time(&before);
    cancel_mutex.unlock ();
  }

  void
  DjnnTimeManager::run_for_emscripten ()
  {
    int duration = getFirstDelta ();

    struct timespec after;
    djnn::get_monotonic_time(&after);
    double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);

    if(elapsedTime >= duration) {
      before = after;
      timeElapsed (elapsedTime);
      GRAPH_EXEC;
    }
  }

  void
  DjnnTimeManager::run ()
  {
    //DBG;
    set_please_stop (false);
    cancel_mutex.lock();
    djnn::get_exclusive_access (DBG_GET);
    int duration = getFirstDelta ();
    djnn::release_exclusive_access (DBG_REL);
    
    try {
      while (!get_please_stop ()) {
        struct timespec before;
        djnn::get_monotonic_time(&before);

        assert(duration>=-1);

        //std::cerr << "entering sleep " << DBGVAR(duration) << std::endl;
        std::chrono::milliseconds ddd(duration);
        if(duration < 0) ddd=std::chrono::milliseconds(2000000); //std::chrono::milliseconds::max();
        bool cancelled = cancel_mutex.try_lock_for(ddd);
        //std::cerr << "exit sleep " << DBGVAR(cancelled) << std::endl;

        if(thread_local_cancelled) {
          break;
        }
        if (get_please_stop ()) {
          break;
        }

        djnn::get_exclusive_access (DBG_GET);

        if(thread_local_cancelled) {
          djnn::release_exclusive_access (DBG_REL);
          break;
        }
        if (get_please_stop ()) {
          djnn::release_exclusive_access (DBG_REL); 
          break;
        }

        if(cancelled) {
          duration = getFirstDelta ();
          djnn::release_exclusive_access (DBG_REL);
          continue;
        }
        if(duration==-1) {
          djnn::release_exclusive_access (DBG_REL);
          continue;
        }


        {
          struct timespec after;
          djnn::get_monotonic_time(&after);
          double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
          timeElapsed(elapsedTime);
          //DBG;
          GRAPH_EXEC; // executing

          if(thread_local_cancelled) {
            djnn::release_exclusive_access (DBG_REL);
            ExternalSource::cancelled = nullptr; // make our external source aware that we are finised
            break;
          }
          if (get_please_stop ()) {
            djnn::release_exclusive_access (DBG_REL); 
            break;
          }
          
          duration = getFirstDelta ();
          djnn::release_exclusive_access (DBG_REL);
        }
          
      }
      //std::cerr << this << " << stop" << std::endl;
    } catch (djnn::exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }
  }

}