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

  DjnnTimeManager::~DjnnTimeManager()
  {
    djnn::get_exclusive_access (DBG_GET);
    djnn::release_exclusive_access (DBG_REL);
  }

  DjnnTimeManager& DjnnTimeManager::instance () {
    return (_instance);
  }

  DjnnTimeManager DjnnTimeManager::_instance;


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
  DjnnTimeManager::run ()
  {
    set_please_stop (false);

    djnn::get_exclusive_access (DBG_GET);

    update_ref_now_in_scheduled_timers (); // we are about to begin but we took some time to init, so pretend that "now" is now.
    try {
      while (!get_please_stop ()) {        
        //bool timer_cancelled =false;
        cancel_mutex.lock(); // first lock, get it //std::cerr << ">> djnntimemanager entering sleep forever" << __FL__;
        if (empty()) {
          djnn::release_exclusive_access (DBG_REL);
          cancel_mutex.lock(); // second lock, blocks until another thread calls please_stop of firstTimerHasChanged //std::cerr << "<< djnntimemanager exit sleep forever" << __FL__;
        } else {
          auto next_time = get_next()->get_end_time();
          djnn::release_exclusive_access (DBG_REL);
          //timer_cancelled =
          cancel_mutex.try_lock_until(next_time); //std::cerr << "<< djnntimemanager exited sleep " << DBGVAR(timer_cancelled) << __FL__;
        }
        djnn::get_exclusive_access (DBG_GET);
        cancel_mutex.unlock(); // unlock first lock
        if(thread_local_cancelled || get_please_stop ()) break;
        //if(timer_cancelled) {} // either 'infinite duration' has run out or there is a new timer

        update_ref_now(); // set the default 'now' -- FIXME useless in a time manager external source?
        djnn_internal::Time::time_point now = djnn_internal::Time::time_point_cast(get_ref_now());
        timeElapsed(now);
        if(thread_local_cancelled || get_please_stop ()) break;

        GRAPH_EXEC;

        if(thread_local_cancelled || get_please_stop ()) break;
      }
    } catch (djnn::exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }

    djnn::release_exclusive_access (DBG_REL);
  }

}













