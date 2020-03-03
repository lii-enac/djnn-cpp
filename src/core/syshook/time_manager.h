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

/*

Requirements: the time manager should
- schedule actions associated to timers and clocks in the future
- schedule multiple timers and clocks even with the same execution time
- schedule actions in a sync manner when multiple timers and clocks are scheduled in a chunk/slice of time in the callback execution of a single external source
- schedule actions with a delay that does not depend on the time taken to execute the associated actions
- execute actions when timers and clocks have elapsed
- deactivate timers once they completed
- reset timers as long as they have not completed
- reschedule clocks so as to repeat the associated action regularly
- cancel timers and clocks when requested externally, presumably inside another external source handling
- modify timers and clocks delays/periods during runtime
- allow for delays/periods modification and activation/deactivation, inside associated actions
- sleep as long as no timer/clock has elapsed
- awake only when the next timer has elapsed

- adapt schedule time according to execution time measurements (with a PID?)

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
        return
            t1->getEndTime()  < t2->getEndTime()
        || (t1->getEndTime() == t2->getEndTime() ? t1<t2 : false); // need to distinguish between Timer*'s
      }
    };


    class Manager {
    public:
      Manager (duration precision=std::chrono::microseconds(100));
      virtual ~Manager() {}

      //struct exc {};
      //struct TimerAlreadyScheduled : exc {};
      //struct TimerListEmpty : exc {};

      void update_ref_now ();
      void update_ref_now (time_point now);
      const time_point& get_ref_now () const { return _ref_now; } 

      void schedule(Timer* timer, duration) ; //throw(TimerAlreadyScheduled);
      void schedule(Timer* timer, duration, time_point start_time) ; //throw(TimerAlreadyScheduled);
      void cancel  (Timer* timer);
      void reset   (Timer* timer);
      void cleanup ();
      bool empty   () const { return _timers.empty(); }
      Timer*  get_next ();
      void    pop_next ();
      void timeElapsed (time_point now);
      virtual void firstTimerHasChanged()=0;

      // debug
      const Timer* find (Timer *) const;
      bool already_scheduled (Timer* timer) const;
      void debug () const;

    private:
      typedef std::set<Timer*, lesser> Timers;
      Timers _timers;
      duration _precision;
      bool _dontCallTimerHasChanged;
      time_point _ref_now;
    };

    
  }
}
