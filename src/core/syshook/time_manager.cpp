#include "time_manager.h"

#include <iostream>
// #include "utils/debug.h"
// #include <boost/core/demangle.hpp>
// #include <typeinfo>

namespace djnn_internal {
  
  namespace Time {
    
    Manager::Manager(float precision)
    :_precision(precision), _dontCallTimerHasChanged(0)
    {     
    }
    
    void
    Manager::after(Timer* timer, Unit t) //throw (TimerAlreadyScheduled)
    {
      //DBG;
      Unit dt=t;
      Timers::iterator i = _timers.begin();
      bool firstchanged=false;
      
      // find the timer position
      while( i!=_timers.end() && dt > (*i)->getDelta() ) {
        dt -= (*i)->getDelta();
        ++i;
      }
      
      // check if it's not here already
      {
        Timers::iterator j = _timers.begin();
        while(j!=_timers.end()) {
          if((*j)==timer) {
            std::cerr << "Timer " << std::hex << timer << std::dec << "already scheduled" << std::endl;
            throw TimerAlreadyScheduled();
          }
          ++j;
        }
      }
      
      timer->setDelta(dt);
      timer->setTime(t);
      i = _timers.insert(i, timer);
      
      if(i==_timers.begin()) {
        firstchanged=true;
      }
      
      ++i;
      if(i!=_timers.end()) {
        // update next timers' delta
        Unit nextdt = (*i)->getDelta();
#if 0
        while(i!=_timers.end() && (*i)->getDelta()==nextdt) {
          (*i)->setDelta(nextdt-dt);
        }
#else
        (*i)->setDelta(nextdt-dt);
#endif
      }
      if(firstchanged && !_dontCallTimerHasChanged) {
        firstTimerHasChanged();
      }
      //std::cerr << "<-- Time::Manager::after" << __FL__;
    }
    
    void
    Manager::cancel(Timer* timer)
    {
      Timers::iterator i = _timers.begin();
      
      // find the position of the timer
      while( i!=_timers.end() && (*i)!=timer)
        ++i;
      
      if( i==_timers.end() )
        return;
      
      bool firstchanged=false;
      Unit dt=0;
      if( i==_timers.begin() ) {
        firstchanged=true;
        dt = (*i)->getDelta();
        _timers.erase(i);
        i = _timers.begin();
      }
      else {
        Timers::iterator j = i;
        --j;
        _timers.erase(i);
        i=j;
        ++i;
      }
      
      if(i!=_timers.end()) {
        // update next timers' delta
        Unit nextdt = (*i)->getDelta();
#if 0
        while(i!=_timers.end() && (*i)->getDelta()==nextdt) {
          (*i)->setDelta(nextdt+dt);
        }
#else
        (*i)->setDelta(nextdt-dt);
#endif
        
      }
      
      if(firstchanged) {
        firstTimerHasChanged();
      }
  }
    
    
    void
    Manager::timeElapsed(Unit dt)
    {
      //DBGMETH
      if(_timers.empty())
        return;
      
      Timers::iterator i = _timers.begin();
      Unit delta = (*i)->getDelta();
      Unit newdelta = delta-dt;
      
      //std::cerr << DBGVAR(dt) << DBGVAR(delta) << DBGVAR(newdelta) << DBGVAR(_timers.size()) << DBGVAR(_precision) << __FL__;
      
      (*i)->setDelta(newdelta);
      
      if(newdelta>_precision) {
        reschedule();
        return;
      }

      Timers toCall;
#if 0
      while( i!=_timers.end() && (*i)->getDelta()==delta) {
        (*i)->setDelta(newdelta);
        toCall.push_back(*i);
        ++i;
      }
#else
      toCall.push_back(*i);
      ++i;
      while( i!=_timers.end() && ((*i)->getDelta()==0)) {
        toCall.push_back(*i);
        ++i;
      }
#endif

      _timers.erase(_timers.begin(), i);
      if(!_timers.empty()) {
        Timers::iterator i = _timers.begin();
        Unit delta = (*i)->getDelta();
        Unit d = delta+newdelta;
        if(d<0)
          d=0;
        (*i)->setDelta(d);
        //std::cerr << DBGVAR(delta) << DBGVAR(newdelta) << DBGVAR(_timers.size()) << __FL__;
      }
      
      _dontCallTimerHasChanged=1;
      for(Timers::iterator j=toCall.begin(); j!=toCall.end(); ++j) {
        Unit t = (*j)->getTime();
        Unit actualtime = t-newdelta;
        //std::cerr << DBGVAR(t) << " " << DBGVAR(actualtime) << " " << boost::core::demangle(typeid(*j).name()) << __FL__;
        (*j)->doit(actualtime);
      }

      _dontCallTimerHasChanged=0;
      // dont call firstTimerHasChanged if it was call by after during doit...
      firstTimerHasChanged();
      //std::cerr << DBGVAR(_timers.size()) << __FL__;
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
