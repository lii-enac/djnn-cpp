#pragma once

#include <list>

namespace djnn_internal {

  namespace Time {

    typedef float Unit; // in seconds

    class Manager;
    class Timer;

    class Manager {
    public:
      Manager(float precision=0);
      virtual ~Manager() {}

      struct exc {};
      struct TimerAlreadyScheduled : exc {};
      struct TimerListEmpty : exc {};

      void after(Timer* timer, Unit) ; //throw(TimerAlreadyScheduled);
      void cancel(Timer* timer);
      bool already_scheduled(Timer* timer);
      void timeElapsed(Unit);
      void cleanup ();

    //protected:
      virtual void firstTimerHasChanged()=0;
      virtual void reschedule();

      typedef std::list<Timer*> Timers;
      const Timers& getTimers() const { return _timers; }
      Unit getFirstDelta() const;// throw(TimerListEmpty);
      void debug () const;

    private:

      Timers _timers;
      Unit _precision;
      bool _dontCallTimerHasChanged;
    };

    class Timer {
    public:
      virtual ~Timer() {}
      virtual void doit(const Unit& actualtime)=0;
      Unit getTime() const { return _time; }
      Unit getDelta() const { return _delta; }

    private:
      Unit _time;
      Unit _delta;
      void setTime(Unit d) { _time=d; }
      void setDelta(Unit d) { _delta=d; }

      friend class Manager;
    };
  }
}
