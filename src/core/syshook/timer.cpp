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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "timer.h"
#include "syshook.h"
#include "core/tree/int_property.h"
#include "core/execution/graph.h"
#include "core/serializer/serializer.h"
#include "core/syshook/djnn_time_manager.h"

#include "cpp-thread.h"
#include "cpp-chrono.h"

// #include <iostream>
// #include "utils/debug.h"

namespace djnn
{

  Timer::Timer (Process *parent, const std::string& name, int period)
  : Process (name),
  _delay (this, "delay", period),
  _end (this, "end"),
  _action (this, "action"),
  _c_update (&_delay, ACTIVATION, &_action, ACTIVATION, true)
  {
    Process::finalize_construction (parent, name);
  }

  Timer::Timer (Process *parent, const std::string& name, std::chrono::milliseconds period)
  : Timer (parent, name, period.count())
  {
  }

#if DJNN_USE_BOOST_CHRONO
  Timer::Timer (Process* parent, const std::string& name, boost::chrono::milliseconds period)
  : Timer(parent, name, period.count())
  {
  }
#endif

  Timer::~Timer ()
  {
  }

  
  void
  Timer::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    //start_thread();
    DjnnTimeManager::instance().after(this, _delay.get_value ());
  }

  void
  Timer::impl_deactivate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    //please_stop ();
    DjnnTimeManager::instance().cancel(this);
  }

  void
  Timer::update_period()
  {
    DjnnTimeManager::instance().cancel(this);
    if(somehow_activating())
      impl_activate (); // reschedule
  }

  void
  Timer::doit(const djnn_internal::Time::Unit& actualtime)
  {
    //DBG;
    //_elapsed.set_value (actualtime, true);
    set_activation_state (DEACTIVATED);
    _end.notify_activation (); // propagating
    //_end.activate (); // propagating
  }

#if 0
  void
  Timer::run ()
  {
    //DBG;
    int duration;
    djnn::get_exclusive_access (DBG_GET);
    if(thread_local_cancelled) {
      djnn::release_exclusive_access (DBG_REL);
      return;
    } else {
      duration = _delay.get_value ();
      djnn::release_exclusive_access (DBG_REL);
    }

    set_please_stop (false);
    try {
      djnn::sleep(duration);

      if(thread_local_cancelled) {
        //thread_terminated ();
        //return;
      } else {
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        // the previous call my take some time, check if we have been cancelled meanwhile
        if(!thread_local_cancelled && !get_please_stop ()) {
          set_activation_state (DEACTIVATED);
          _end.notify_activation (); // propagating
          GRAPH_EXEC; // executing
          if(!thread_local_cancelled) { // let's finish properly if we were not cancelled
            cancelled = nullptr; // make our external source aware that we are finised
          }
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
    } catch (exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }
#if DJNN_USE_BOOST_THREAD
    catch(boost::thread_interrupted const& ) {
        //std::cout << "thread interrupted" << std::endl;
    }
#endif
  }
#endif

  void
  Timer::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:timer");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("delay", _delay.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }


#if 0
#ifdef __cplusplus // C++

#if 1 // plain objects

#define BEG_CAI :
#define ALLOC_AND_INIT(type, name, ...) name (__VA_ARGS__)
#define END_CAI {
#define UNINIT_AND_DEALLOC(type, name, ...)

#else // pointers

#define BEG_CAI {
#define ALLOC_AND_INIT(type, name, ...) name = ne_w type (__VA_ARGS__)
#define END_CAI
#define UNINIT_AND_DEALLOC (type, name, ...) delete name;

#endif

#else // C // not tested

#define BEG_CAI {
#define ALLOC_AND_INIT(type, name, ...) name = (type*)malloc(sizeof(type)); type#_init(__VA_ARGS__);
#define END_CAI
#define UNINIT_AND_DEALLOC (type, name, ...) free(name);

#endif
  /*
  // old pointer-based
  Timer::Timer (int period)
  {
    _delay = ne_w IntProperty (this, "delay", period);
    _end = ne_w Blank (this, "end");
  }
  */

  /*
  // configurable
  Timer::Timer (int period)
  BEG_CAI
  ALLOC_AND_INIT(IntProperty, _delay, this, "delay", period),
  ALLOC_AND_INIT(Blank, _end, this, "end")
  END_CAI
  }
  */
  /*
  // old destructor, pointer based
  Timer::~Timer ()
  {
    delete _end;
    delete _delay;
  }
  */
  
  /*
  // configurable
  Timer::~Timer ()
  {
    UNINIT_AND_DEALLOC(IntProperty, _delay);
    UNINIT_AND_DEALLOC(Blank, _end);
  }
  */
#endif

#if 0

// PLEASE DO NOT REMOVE, FOR FUTURE INCLUSION

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

      void after(Timer* timer, Unit) throw(TimerAlreadyScheduled);
      void cancel(Timer* timer);
      void timeElapsed(Unit);

    //protected:
      virtual void firstTimerHasChanged()=0;
      virtual void reschedule();

      typedef std::list<Timer*> Timers;
      const Timers& getTimers() const { return _timers; }
      Unit getFirstDelta() const;// throw(TimerListEmpty);

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

namespace djnn_internal {
  
  namespace Time {
    
    Manager::Manager(float precision)
    :_precision(precision), _dontCallTimerHasChanged(0)
    {     
    }
    
    void
    Manager::after(Timer* timer, Unit t) throw (TimerAlreadyScheduled)
    {
      //DBGMETH
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
        //std::cerr << DBGVAR(t) << DBGVAR(actualtime) << __FL__;
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

    class MyManager : public djnn_internal::Time::Manager, public djnn::ExternalSource/*, public djnn::Process*/ {
    public:
        static MyManager* instance () {
          std::call_once (MyManager::onceFlag, [] () {
            _instance = new MyManager ();
          });

          return (_instance);
        }
        static std::once_flag onceFlag;
        static MyManager* _instance;
      
        // djnn_internal::Time::Manager
        virtual void firstTimerHasChanged() override
        {

        }

        // djnn::process
        //void
        //impl_activate () override
        //{
        //  set_please_stop (false);
        //  start_thread();
        //}

        //void
        //impl_deactivate () override
        //{
        //  please_stop ();
        //}

        // ExternalSource
        void
        run () override
        {
          //DBG;
          try {
              //chrono::milliseconds duration (_delay->get_value ());
              //djnn::chrono::milliseconds duration ( (int) (getFirstDelta () * 1000));
              //this_thread::sleep_for (duration); // blocking call
              djnn::sleep(getFirstDelta ());
              djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
              if (!get_please_stop ()) {
                //set_deactivated ();
                //_end->notify_activation (); // propagating
                using namespace djnn;
                GRAPH_EXEC; // executing
              }
              djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
          } catch (djnn::exception& e) {
            std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
          }
        }
      };
      std::once_flag MyManager::onceFlag;
      MyManager* MyManager::_instance;
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
#endif



}
