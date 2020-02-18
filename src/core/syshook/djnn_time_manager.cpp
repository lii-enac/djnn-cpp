#include "djnn_time_manager.h"

#include "core/syshook/syshook.h"
#include "core/tree/int_property.h"
#include "core/syshook/cpp-thread.h"
#include "core/execution/graph.h"

#include <cassert>

#include <iostream>
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


  struct timespec before_emscripten;
  void DjnnTimeManager::firstTimerHasChanged()
  {
    cancel_mutex.unlock ();
  }

  void
  DjnnTimeManager::please_stop ()
  {
    if(get_please_stop ()) return;
    set_please_stop (true);
    cancel_mutex.unlock ();
  }


  void
  DjnnTimeManager::run_for_emscripten ()
  {
    int duration = getFirstDelta ();

    struct timespec after;
    djnn::get_monotonic_time(&after);
    double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before_emscripten.tv_sec * 1000 + before_emscripten.tv_nsec * 1e-6);

    if(elapsedTime >= duration) {
      before_emscripten = after;
      timeElapsed (elapsedTime);
      GRAPH_EXEC;
    }
  }

  void
  DjnnTimeManager::run ()
  {
    set_please_stop (false);
    djnn::get_exclusive_access (DBG_GET);
    int duration = getFirstDelta ();
    djnn::release_exclusive_access (DBG_REL);

    try {
      while (!get_please_stop ()) {
        struct timespec before;
        djnn::get_monotonic_time(&before);

        assert(duration>=-1);

        //djnn::get_exclusive_access (DBG_GET);
        //debug();
        //djnn::release_exclusive_access (DBG_REL);

        bool timer_cancelled =false;

        if (duration<0) {
          cancel_mutex.lock(); // first lock, get it
          //std::cerr << ">> djnntimemanager entering sleep forever" << __FL__;
          cancel_mutex.lock(); // second lock, blocks until another thread calls please_stop of firstTimerHasChanged
          //std::cerr << "<< djnntimemanager exit sleep forever" << __FL__;
          cancel_mutex.unlock(); // unlock first lock
        } else {
          cancel_mutex.lock(); // first lock, get it
          //std::cerr << ">> djnntimemanager entering sleep " << DBGVAR(duration) << __FL__;
          timer_cancelled =
          cancel_mutex.try_lock_for(std::chrono::milliseconds(duration)); // second lock, blocks for ddd ms unless it's unlocked elsewhere
          //std::cerr << "<< djnntimemanager exited sleep " << DBGVAR(timer_cancelled) << __FL__;
          cancel_mutex.unlock(); // unlock first time
        }

        if(thread_local_cancelled || get_please_stop ()) {
          djnn::get_exclusive_access (DBG_GET); // since we release it at exit
          break;
        }

        if(timer_cancelled) {
          // either 'infinite duration' has run out or there is a new timer
          djnn::get_exclusive_access (DBG_GET); if(thread_local_cancelled || get_please_stop ()) break;
          duration = getFirstDelta ();
          djnn::release_exclusive_access (DBG_REL);
          continue;
        }

        djnn::get_exclusive_access (DBG_GET); if(thread_local_cancelled || get_please_stop ()) break;

        struct timespec after;
        djnn::get_monotonic_time(&after);
        double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
        //std::cerr << "executing after TimeManager::Timer " <<  DBGVAR(elapsedTime) << __FL__;
        timeElapsed(elapsedTime);

        GRAPH_EXEC; // executing

        if(thread_local_cancelled || get_please_stop ()) break;
        
        duration = getFirstDelta ();
        djnn::release_exclusive_access (DBG_REL);
      }
    } catch (djnn::exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }

    //ExternalSource::cancelled = nullptr; // make our external source aware that we are finished
    djnn::release_exclusive_access (DBG_REL);
    //DBG;
  }

}
