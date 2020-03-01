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
  {
  }

  DjnnTimeManager& DjnnTimeManager::instance () {
    return (_instance);
  }

  DjnnTimeManager DjnnTimeManager::_instance;


  void
  DjnnTimeManager::firstTimerHasChanged()
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


  djnn_internal::Time::time_point
  before_emscripten;

  void
  DjnnTimeManager::run_for_emscripten ()
  {
    //djnn_internal::Time::duration duration = getFirstDelta ();

    //struct timespec after;
    //djnn::get_monotonic_time(&after);
    //double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before_emscripten.tv_sec * 1000 + before_emscripten.tv_nsec * 1e-6);

    UNIMPL;
    /*
    if(elapsedTime >= duration) {
      before_emscripten = after;
      timeElapsed (elapsedTime);
      GRAPH_EXEC;
    }
    */
  }

  void
  DjnnTimeManager::run ()
  {
    set_please_stop (false);

    djnn::get_exclusive_access (DBG_GET);
    
    bool empty;
    djnn_internal::Time::time_point next_time;

    empty = getTimers().empty();;
    if( !empty ) {
      next_time = get_next()->getEndTime();
    }

    update_ref_now();
    
    djnn::release_exclusive_access (DBG_REL);

    try {
      while (!get_please_stop ()) {
        
        //bool timer_cancelled =false;
        //auto before_lock = djnn_internal::Time::time_point_cast(djnn_internal::Time::clock::now());

        if (empty) {
          cancel_mutex.lock(); // first lock, get it
          //std::cerr << ">> djnntimemanager entering sleep forever" << __FL__;
          cancel_mutex.lock(); // second lock, blocks until another thread calls please_stop of firstTimerHasChanged
          //std::cerr << "<< djnntimemanager exit sleep forever" << __FL__;
          cancel_mutex.unlock(); // unlock first lock
        } else {
          cancel_mutex.lock(); // first lock, get it
          //std::cerr << ">> djnntimemanager entering sleep " << (next_time - before_lock).count() << "us " << __FL__;
          //timer_cancelled =
          cancel_mutex.try_lock_until(next_time);
          //std::cerr << "<< djnntimemanager exited sleep " << DBGVAR(timer_cancelled) << __FL__;
          cancel_mutex.unlock(); // unlock first time
        }

        //auto after_unlock = djnn_internal::Time::time_point_cast(djnn_internal::Time::clock::now());

        if(thread_local_cancelled || get_please_stop ()) {
          djnn::get_exclusive_access (DBG_GET); // since we release it at exit
          break;
        }

        /*if(timer_cancelled) {
          // either 'infinite duration' has run out or there is a new timer
          djnn::get_exclusive_access (DBG_GET); if(thread_local_cancelled || get_please_stop ()) break;
          duration = getFirstDelta ();
          djnn::release_exclusive_access (DBG_REL);
          continue;
        }*/

        djnn::get_exclusive_access (DBG_GET); if(thread_local_cancelled || get_please_stop ()) break;
        //auto after_get_exclusive_access = djnn_internal::Time::time_point_cast(djnn_internal::Time::clock::now());

        //std::cerr << (after_unlock - next).count() << "us " << (after_get_exclusive_access-next).count () << "us " << __FL__;

        update_ref_now();
        djnn_internal::Time::time_point now = djnn_internal::Time::time_point_cast(get_ref_now());
        //std::cerr << "executing after TimeManager::Timer " <<  DBGVAR(elapsedTime) << __FL__;
        timeElapsed(now);

        if(thread_local_cancelled || get_please_stop ()) break;

        GRAPH_EXEC; // executing

        if(thread_local_cancelled || get_please_stop ()) break;

        empty = getTimers().empty();
        if( !empty ) {
          next_time = get_next()->getEndTime();
        }
        djnn::release_exclusive_access (DBG_REL);
      }
    } catch (djnn::exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }

    djnn::release_exclusive_access (DBG_REL);
  }

}
