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

#include "djnn_time_manager.h"

#include "exec_env/global_mutex.h"
#include "core/tree/int_property.h"
#include "exec_env/cpp-thread.h"
#include "core/execution/graph.h"

#include <mutex>
#include <condition_variable>

#include <cassert>

// #include <iostream>
// #include "utils/debug.h"

namespace djnn {

  DjnnTimeManager::DjnnTimeManager()
  : ExternalSource("TimeManager")
  {
  }

  DjnnTimeManager::~DjnnTimeManager()
  {
    if (is_global_mutex_inited()) {
      djnn::get_exclusive_access (DBG_GET);
      djnn::release_exclusive_access (DBG_REL);
    }
  }

  //DjnnTimeManager DjnnTimeManager::_instance;
  //DjnnTimeManager _instance;
  static DjnnTimeManager * _instance;

  // here since DjnnTimeManager is a singleton: this prevents unneeded stuff in djnn_time_manager.h
  //static std::timed_mutex * cancel_mutex;
  static std::mutex * cancel_mutex;
        
  // we need a condition variable, a mutex is not enough, see https://stackoverflow.com/questions/12551341/when-is-a-condition-variable-needed-isnt-a-mutex-enough
  // The mutex must be locked by the current thread of execution, otherwise, the behavior is undefined. https://en.cppreference.com/w/cpp/thread/timed_mutex/unlock
  // If lock is called by a thread that already owns the mutex, the behavior is undefined: for example, the program may deadlock. https://en.cppreference.com/w/cpp/thread/timed_mutex/lock
  //static std::condition_variable_any * cv;
  static std::condition_variable * cv;

  DjnnTimeManager&
  DjnnTimeManager::instance ()
  {
    //return (_instance);
    if (!_instance) {
      _instance = new DjnnTimeManager;
      //cancel_mutex = new std::timed_mutex;
      cancel_mutex = new std::mutex;
      //cv = new std::condition_variable_any;
      cv = new std::condition_variable;
    }
    return (*_instance);
  }

  void
  DjnnTimeManager::delete_ ()
  {
    delete _instance;
    _instance = nullptr;
    delete cancel_mutex;
    delete cv;
  }


  djnn_internal::Time::time_point
  before_emscripten;

  void
  DjnnTimeManager::run_for_emscripten ()
  {
    //djnn_internal::Time::duration duration = getFirstDelta ();

    if (!empty ()) {

      //djnn_internal::Time::duration duration = getFirstDelta ();

      //struct timespec after;
      //djnn::get_monotonic_time(&after);
      //double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before_emscripten.tv_sec * 1000 + before_emscripten.tv_nsec * 1e-6);
      djnn_internal::Time::time_point now = djnn_internal::Time::time_point_cast(djnn_internal::Time::clock::now());

      //UNIMPL;
      
      if (now >= get_next () -> get_end_time ()) {
        update_ref_now ();
        before_emscripten = now;
        time_has_elapsed (now);
        GRAPH_EXEC;
      }
    }
  }

  void
  DjnnTimeManager::update_ref_now_in_scheduled_timers ()
  {
    update_ref_now ();
    for (auto it: _timers) {
      auto d = (*it).get_duration ();
      (*it).set_start_time (get_ref_now ());
      (*it).set_end_time (get_ref_now ()+d);
    }
  }

  void
  DjnnTimeManager::firstTimerHasChanged()
  {
    //cancel_mutex.unlock (); // FIXME1 The mutex must be locked by the current thread of execution, otherwise, the behavior is undefined. https://en.cppreference.com/w/cpp/thread/timed_mutex/unlock
    cv->notify_one();
  }

  void
  DjnnTimeManager::please_stop ()
  {
    if(get_please_stop ()) return;
    set_please_stop (true);
    cv->notify_one();
  }

  void
  DjnnTimeManager::run ()
  {
    set_please_stop (false);
    djnn::get_exclusive_access (DBG_GET);
    update_ref_now_in_scheduled_timers (); // we are about to begin but we took some time to init, so pretend that "now" is now.
    // try {
      while (!get_please_stop ()) {        
        { // scope for mutex
          std::unique_lock<std::mutex> l (*cancel_mutex); // first lock, get it //std::cerr << ">> djnntimemanager entering sleep forever" << __FL__;
          if (empty ()) {
            djnn::release_exclusive_access (DBG_REL);
            cv->wait(l // second lock, blocks until another thread calls please_stop of firstTimerHasChanged //std::cerr << "<< djnntimemanager exit sleep forever" << __FL__;
              //, [this]{return has_time_elapsed();} // should prevent spurious wakeup, but there is a race condition
            ); // FIXME cv should use global mutex
          } else {
            auto next_time = get_next()->get_end_time();
            djnn::release_exclusive_access (DBG_REL);
            cv->wait_until(l, next_time  // second lock, blocks until another thread calls please_stop of firstTimerHasChanged //std::cerr << "<< djnntimemanager exit sleep forever" << __FL__;
              //, [this]{return has_time_elapsed();} // should prevent spurious wakeup, but there is a race condition
            ); // FIXME cv should use global mutex
          }
          djnn::get_exclusive_access (DBG_GET);
        }
        if(should_i_stop ()) break;

        update_ref_now(); // set the default 'now' -- FIXME useless in a time manager external source?
        djnn_internal::Time::time_point now = djnn_internal::Time::time_point_cast(get_ref_now());
        time_has_elapsed(now);
        if(should_i_stop ()) break;

        GRAPH_EXEC;

        if(should_i_stop ()) break;
      }
    // } catch (djnn::exception& e) {
    //   std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    // }

    djnn::release_exclusive_access (DBG_REL);
  }

}
