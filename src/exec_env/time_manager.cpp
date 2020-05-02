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

#include "time_manager.h"

#include "core/utils/error.h"

//#include <iostream>
//#include "utils/debug.h"

#include <cassert>


namespace djnn_internal {
  
  namespace Time {
    
    Manager::Manager(duration precision)
    : _precision(precision), _dontCallTimerHasChanged(0)
    {
      update_ref_now ();
    }

    void
    Manager::update_ref_now ()
    {
      set_ref_now(time_point_cast(clock::now()));
    }

    void
    Manager::set_ref_now (time_point now)
    {
      _ref_now = now;
    }

    void
    Manager::schedule (Timer* timer, duration t)
    {
      schedule(timer, t, get_ref_now());
    }
    
    void
    Manager::schedule (Timer* timer, duration t, time_point start_time) //throw (TimerAlreadyScheduled)
    {
//#if !DJNN_NO_DEBUG // should be assert
      if(is_already_scheduled(timer)) {
        djnn::warning(dynamic_cast<djnn::FatProcess*>(timer), "timer has already been scheduled");
        cancel(timer);
      }
//#endif

      timer->set_start_time(start_time);
      timer->set_end_time(start_time + t);
      Timer * previous_first = nullptr;
      if(!empty()) {
        previous_first = get_next();
      }
      _timers.emplace(timer);
      timer->set_scheduled(true);
      if(previous_first != get_next()) {      
        firstTimerHasChanged ();
      }
    }

    void
    Manager::cancel (Timer* timer)
    { 
//#if !DJNN_NO_DEBUG // should be assert
      if(!is_already_scheduled(timer)) {
        djnn::warning(dynamic_cast<djnn::FatProcess*>(timer), "timer cancelled but not previously scheduled");
        return;
      }
//#endif

      auto it = _timers.find(timer);
      if(it != _timers.end()) {
        bool firsthaschanged = false;
        if (it==_timers.begin()) firsthaschanged = true;
        _timers.erase(it);
        (*it)->set_scheduled(false);
        if(firsthaschanged) firstTimerHasChanged();
      }
    }

    void
    Manager::reset (Timer* timer)
    {
      cancel(timer);
      duration d = timer->get_end_time () - timer->get_start_time ();
      schedule(timer, d, get_ref_now());
    }

    void
    Manager::cleanup ()
    {
      for (auto * t: _timers) t->set_scheduled(false);
      _timers.clear ();
      assert(_timers.empty());
    }

    Timer*
    Manager::get_next()
    {
      return *(_timers.begin ());
    }

    const Timer*
    Manager::get_next() const
    {
      return *(_timers.begin ());
    }

    void
    Manager::pop_next()
    {
      _timers.erase(_timers.begin());
    }
        
    void
    Manager::time_has_elapsed(time_point now)
    { 
      if(empty())
        return;

      const Timer* previous_first = get_next();
      
      _dontCallTimerHasChanged=1;
      while ( empty()==false && (now > (get_next()->get_end_time() - _precision))) {
        Timer* t = get_next();
        pop_next();

        // req : sync between timers, do not loose time because of delays
        auto sav_now = get_ref_now (); // save default 'now'
        set_ref_now (t->get_end_time()); // use the expected End Time and pretend it's the default 'now' for the next activations
        t->set_scheduled(false);
        t->do_it(now-t->get_end_time());
        set_ref_now (sav_now); // restore default 'now'
      }
      _dontCallTimerHasChanged=0;

      if (empty() || previous_first != get_next()) {
        firstTimerHasChanged ();
      }
    }

    bool
    Manager::has_time_elapsed() const
    {
      return !empty() && (djnn_internal::Time::clock::now() + get_precision ()) < get_next()->get_end_time();
    }
    
    // debug

    const Timer*
    Manager::find(Timer * timer) const
    {
      return *(_timers.find(timer));
    }


    bool
    Manager::is_already_scheduled(Timer * timer) const
    {
      //return _timers.find(timer) != _timers.end();
      return timer->is_already_scheduled ();
    }
    
    void
    Manager::debug() const
    {
      // needs <iostream>
      /*std::cerr << "timer queue: ";
      for (auto it = _timers.begin(); it != _timers.end(); ++it ) {
        //std::cerr << (*it)->get_end_time () << "ms (" << (*it)->get_duration() << "ms) - ";
        std::cerr << *it << " " << (*it)->get_duration().count() << "us - ";
      }
      //std::cerr << std::endl; //__FL__;
      */
    }
    
    
} // time
} // djnn_internal

