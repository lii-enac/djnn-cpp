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

#pragma once

#include <list>
#include <chrono>
#include <set>

namespace djnn_internal {

  namespace Time {

    typedef std::chrono::duration<long long, std::micro> duration;
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::time_point <clock, duration> time_point;
    
    template <typename xxx>
    time_point
    time_point_cast(const xxx& tp)  {
      return std::chrono::time_point_cast<std::chrono::microseconds>(tp);
    }

    class Manager;
    class Timer;

    class Timer {
    public:
      virtual ~Timer() {}
      virtual void doit(const duration& actualduration)=0;
      const time_point& getStartTime() const { return _start; }
      const time_point& getEndTime() const { return _end; }
      void setStartTime(time_point d) { _start=d; }
      void setEndTime(time_point d) { _end=d; }
      duration getDuration() const { return _end-_start;}
      
    private:
      time_point _start, _end;
    };

    struct lesser {
      bool
      operator() (const djnn_internal::Time::Timer* t1, const djnn_internal::Time::Timer* t2) const
      {
        return t1->getEndTime() < t2->getEndTime();
      }
    };

    class Manager {
    public:
      Manager(duration precision=std::chrono::microseconds(100));
      virtual ~Manager() {}

      struct exc {};
      struct TimerAlreadyScheduled : exc {};
      struct TimerListEmpty : exc {};

      void update_ref_now();
      const time_point& get_ref_now () const { return _ref_now; } 

      virtual void schedule(Timer* timer, duration) ; //throw(TimerAlreadyScheduled);
      virtual void schedule(Timer* timer, duration, time_point start_time) ; //throw(TimerAlreadyScheduled);
      virtual void cancel(Timer* timer);
      void reset(Timer* timer);      
      bool already_scheduled(Timer* timer);
      Timer* get_next ();
      void pop_next ();
      virtual void firstTimerHasChanged()=0;

      void timeElapsed(time_point now);

      typedef std::multiset<Timer*, lesser> Timers;      
      const Timers& getTimers() const { return _timers; }
      void cleanup ();
      void debug () const;

    private:
      Timers _timers;
      duration _precision;
      bool _dontCallTimerHasChanged;
      time_point _ref_now;
    };

    
  }
}
