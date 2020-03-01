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

#include "time_manager.h"

#include <iostream>
#include "utils/debug.h"
#include "core/utils/error.h"
//#include "utils/ext/debugbreak.h"
// #include <boost/core/demangle.hpp>
// #include <typeinfo>

#define DBGTIMERS for(Timers::iterator iii = _timers.begin(); iii!=_timers.end(); ++iii) { std::cerr << *iii << "(" << (*iii)->getDelta() << "ms) ";} std::cerr << std::endl;

namespace djnn
{
  void
  get_monotonic_time (struct timespec *ts);
}

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
      _ref_now = time_point_cast(clock::now());
    }

    void
    Manager::schedule(Timer* timer, duration t)
    {
      schedule(timer, t, get_ref_now());
    }
    
    void
    Manager::schedule(Timer* timer, duration t, time_point start_time) //throw (TimerAlreadyScheduled)
    {
      if(already_scheduled(timer)) {
        djnn::warning(dynamic_cast<djnn::Process*>(timer), "timer scheduled but has already been scheduled");
        cancel(timer);
      }

      timer->setStartTime(start_time);
      timer->setEndTime(start_time + t);
      Timer * previous_first = nullptr;
      if(!_timers.empty()) {
        previous_first = get_next();
      }
      _timers.emplace(timer);
      if(previous_first != get_next()) {      
        firstTimerHasChanged ();
      }
    }

    void
    Manager::cancel(Timer* timer)
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
    Manager::reset(Timer* timer)
    {
      cancel(timer);
      duration d = timer->getEndTime () - timer->getStartTime ();
      schedule(timer, d, get_ref_now());
    }

    bool
    Manager::already_scheduled(Timer * timer)
    {
      return _timers.find(timer) != _timers.end();
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
      if(_timers.empty())
        return;

      Timer * previous_first = get_next();
      
      _dontCallTimerHasChanged=1;
      while ( _timers.empty()==false && (get_next()->getEndTime() - now) < _precision) {
        auto * t = get_next();
        _timers.erase(_timers.begin());
        t->doit(now-t->getEndTime());
      }
      _dontCallTimerHasChanged=0;

      if (_timers.empty() || previous_first != get_next()) {
        firstTimerHasChanged ();
      }
    }
    
    void
    Manager::cleanup ()
    {
      _timers.clear ();
      assert(_timers.empty());
    }

    /*
    void
    Manager::debug() const
    {
      std::cerr << "timer queue: ";
      for (auto it = _timers.begin(); it != _timers.end(); ++it ) {
        //std::cerr << (*it)->getEndTime () << "ms (" << (*it)->getDuration() << "ms) - ";
        std::cerr << (*it)->getDuration().count() << "us - ";
      }
      //std::cerr << std::endl; //__FL__;
    }
    */
    
} // time
} // djnn_internal

