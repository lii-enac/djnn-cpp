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

#include <iostream>
#include "utils/debug.h"
#include "core/utils/error.h"


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
      update_ref_now(time_point_cast(clock::now()));
    }

    void
    Manager::update_ref_now (time_point now)
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
      //debug(); std::cerr << __FL__;
      if(already_scheduled(timer)) {
        djnn::warning(dynamic_cast<djnn::Process*>(timer), "timer has already been scheduled");
        //std::cerr << timer << __FL__;
        cancel(timer);
      }

      timer->setStartTime(start_time);
      timer->setEndTime(start_time + t);
      Timer * previous_first = nullptr;
      if(!empty()) {
        previous_first = get_next();
      }
      _timers.emplace(timer);
      if(previous_first != get_next()) {      
        firstTimerHasChanged ();
      }
    }

    void
    Manager::cancel (Timer* timer)
    { 
      if(!already_scheduled(timer)) {
        djnn::warning(dynamic_cast<djnn::Process*>(timer), "timer cancelled but not previously scheduled");
        return;
      }

      auto it = _timers.find(timer);
      if(it != _timers.end()) {
        bool firsthaschanged = false;
        if (it==_timers.begin()) firsthaschanged = true;
        _timers.erase(it);
        if(firsthaschanged) firstTimerHasChanged();
      }
    }

    void
    Manager::reset (Timer* timer)
    {
      cancel(timer);
      duration d = timer->getEndTime () - timer->getStartTime ();
      schedule(timer, d, get_ref_now());
    }

    void
    Manager::cleanup ()
    {
      _timers.clear ();
      assert(_timers.empty());
    }

    Timer*
    Manager::get_next()
    {
      return *(_timers.begin ());
    }

    void
    Manager::pop_next()
    {
      _timers.erase(_timers.begin());
    }
        
    void
    Manager::timeElapsed(time_point now)
    { 
      if(empty())
        return;

      Timer * previous_first = get_next();
      
      _dontCallTimerHasChanged=1;
      //std::cerr << (now - (get_next()->getEndTime() - _precision)).count()/1000 << __FL__;
      while ( empty()==false && (now > (get_next()->getEndTime() - _precision))) {
        auto * t = get_next();
        pop_next();
        t->doit(now-t->getEndTime());
      }
      _dontCallTimerHasChanged=0;

      if (empty() || previous_first != get_next()) {
        firstTimerHasChanged ();
      }
    }
    
    
    // debug

    const Timer*
    Manager::find(Timer * timer) const
    {
      return *(_timers.find(timer));
    }


    bool
    Manager::already_scheduled(Timer * timer) const
    {
      return _timers.find(timer) != _timers.end();
    }
    
    void
    Manager::debug() const
    {
      std::cerr << "timer queue: ";
      for (auto it = _timers.begin(); it != _timers.end(); ++it ) {
        //std::cerr << (*it)->getEndTime () << "ms (" << (*it)->getDuration() << "ms) - ";
        std::cerr << *it << " " << (*it)->getDuration().count() << "us - ";
      }
      //std::cerr << std::endl; //__FL__;
    }
    
    
} // time
} // djnn_internal

