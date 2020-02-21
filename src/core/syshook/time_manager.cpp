#include "time_manager.h"

#include <iostream>
#include "utils/debug.h"
// #include <boost/core/demangle.hpp>
// #include <typeinfo>

#define DBGTIMERS for(Timers::iterator iii = _timers.begin(); iii!=_timers.end(); ++iii) { std::cerr << *iii << "(" << (*iii)->getDelta() << "ms) ";} std::cerr << std::endl;

namespace djnn_internal {
  
  namespace Time {
    
    Manager::Manager(float precision)
    :_precision(precision), _dontCallTimerHasChanged(0)
    {     
    }

    bool
    Manager::already_scheduled(Timer * timer)
    {
      Timers::iterator j = _timers.begin();
      while(j!=_timers.end()) {
        if((*j)==timer) {
          //std::cerr << "Timer " << std::hex << timer << std::dec << " already scheduled" << std::endl;
          return true;
        }
        ++j;
      }
      return false;
    }
    
    void
    Manager::after(Timer* timer, Unit t) //throw (TimerAlreadyScheduled)
    { //std::cerr << "after  " << DBGVAR(timer) << " " << DBGVAR(t) << __FL__;
      //std::cerr << "  >> "; // << __FL__;
      //debug();

      Unit dt=t;
      Timers::iterator i = _timers.begin();
      
      // find the timer position and accumulate its dt
      while( i!=_timers.end() && dt > (*i)->getDelta() ) {
        if(*i==timer) {
          std::cerr << "Timer " << std::hex << timer << std::dec << " already scheduled" << std::endl;
          //throw TimerAlreadyScheduled();
        }
        dt -= (*i)->getDelta();
        ++i;
      }
      
      // check if it's not here already
      for(auto j = i; j!=_timers.end(); ++j) {
        if(*j==timer) {
          std::cerr << "Timer " << std::hex << timer << std::dec << " already scheduled" << std::endl;
          //throw TimerAlreadyScheduled();
        }
      }
      
      timer->setDelta(dt);
      timer->setTime(t);
      i = _timers.insert(i, timer);
      
      bool firstchanged=false;
      if(i==_timers.begin()) {
        firstchanged=true;
      }
      
      ++i;
      if(i!=_timers.end()) {
        // update next timer's delta
        Unit nextdt = (*i)->getDelta();
        Unit newdt = nextdt-dt;
        //std::cerr << DBGVAR(newdt) << __FL__;
        (*i)->setDelta(newdt);
      }

      if(firstchanged && !_dontCallTimerHasChanged) {
        firstTimerHasChanged();
      }
      //std::cerr << "  << ";
      //debug();
    }

    
    void
    Manager::cancel(Timer* timer)
    { //std::cerr << "cancel " << DBGVAR(timer) << __FL__;
      //std::cerr << "  >> ";
      //debug();

      Timers::iterator i = _timers.begin();
      
      // find the position of the timer
      while( i!=_timers.end() && (*i)!=timer)
        ++i;
      
      if( i==_timers.end() ) {
        //std::cerr << "  << ";
        //debug();
        return;
      }
      
      bool firstchanged=false;
      Unit dt=0;
      if( i==_timers.begin() ) {
        firstchanged=true;
        dt = (*i)->getDelta();
        //std::cerr << "previous dt: " << dt << __FL__;
        _timers.erase(i);
        i = _timers.begin();
        //std::cerr << _timers.size() << __FL__;
        //++i;
      }
      else {
        dt = (*i)->getDelta();
        Timers::iterator j = i;
        --j;
        _timers.erase(i);
        i=j;
        ++i;
      }
      
      if(i!=_timers.end()) {
        // update next timers' delta
        Unit nextdt = (*i)->getDelta();
        //std::cerr << "previous next dt: " << nextdt << __FL__;
        Unit newdt = nextdt+dt;
        //std::cerr << DBGVAR(newdt) << __FL__;
        (*i)->setDelta(newdt);  
      }// else DBG;
      
      if(firstchanged) {
        firstTimerHasChanged();
      }

      //std::cerr << "  << ";
      //debug();
    }

    void
    Manager::reset(Timer* timer)
    {
      cancel(timer);
      after(timer, timer->getTime());
    }
    
    
    void
    Manager::timeElapsed(Unit dt)
    { //std::cerr << "timeElapsed " << DBGVAR(dt) << " " << DBGVAR(_timers.size()) << __FL__;
      //std::cerr << ">> "; DBGTIMERS;
      
      if(_timers.empty())
        return;
      
      Timers::iterator i = _timers.begin();
      Unit requested_delta = (*i)->getDelta();
      Unit actual_delta = requested_delta-dt;
      
      //std::cerr << DBGVAR(dt) << DBGVAR(delta) << DBGVAR(newdelta) << DBGVAR(_timers.size()) << DBGVAR(_precision) << __FL__;

      if(actual_delta<0) actual_delta=0;

      if(actual_delta>_precision) { //DBG;
        //std::cerr << DBGVAR(actual_delta) << " " << DBGVAR(_precision) << __FL__;
        (*i)->setDelta(actual_delta);
        reschedule();
        return;
      }

      Timers toCall;
      toCall.push_back(*i);
      ++i;

      dt -= requested_delta;
      while( i!=_timers.end() && ( ((*i)->getDelta()-dt)<_precision) ) {
        toCall.push_back(*i);
        dt -= (*i)->getDelta();
        ++i;
      }

      _timers.erase(_timers.begin(), i);

      if(!_timers.empty()) {
        Timers::iterator i = _timers.begin();
        Unit next_requested_delta = (*i)->getDelta()-dt;
        Unit d = next_requested_delta; // + actual_delta;
        if(d<0) d=0;
        (*i)->setDelta(d);
        //std::cerr << DBGVAR(delta) << DBGVAR(newdelta) << DBGVAR(_timers.size()) << __FL__;
      }
      
      // dont call firstTimerHasChanged if it is called by 'after' during 'doit'...
      _dontCallTimerHasChanged=1;
      for(Timers::iterator j=toCall.begin(); j!=toCall.end(); ++j) {
        Unit t = (*j)->getTime();
        Unit actual_time = t-actual_delta;
        //DBG;
        (*j)->doit(actual_time);
      }
      _dontCallTimerHasChanged=0;

      firstTimerHasChanged();

      //std::cerr << DBGVAR(_timers.size()) << __FL__;
      //debug();
    }
    
    void
    Manager::cleanup ()
    { // DBG;
      _timers.clear ();
    }

    void
    Manager::reschedule()
    {
      firstTimerHasChanged();
    }
    
    Unit
    Manager::getFirstDelta() const// throw(TimerListEmpty)
    {
      if(!_timers.empty()) {
        Timers::const_iterator i = _timers.begin();
        Unit delta = (*i)->getDelta();
        return delta;
      }
      return -1;
      //throw TimerListEmpty();
    }

    void
    Manager::debug() const
    {
      std::cerr << "timer queue: ";
      for (auto it = _timers.begin(); it != _timers.end(); ++it ) {
        std::cerr << (*it)->getDelta() << "ms ";
      }
      std::cerr << std::endl; //__FL__;
    }

    
} // time
} // djnn_internal

namespace djnn {
  class MyTimer : public djnn_internal::Time::Timer {
  public:
    MyTimer(Timer* djnnt) /*: _djnn_timer(djnnt)*/ {}
    virtual void doit(const djnn_internal::Time::Unit& actualtime) {
    }
  private:
    //Timer * _djnn_timer;
  };
}
