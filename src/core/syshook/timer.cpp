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

//#include <iostream>
//#include "utils/debug.h"

namespace djnn
{

  Timer::Timer (Process *parent, const std::string& name, int delay)
  : Process (name),
  _delay (this, "delay", delay),
  _end (this, "end"),
  _action (this, "action"),
  _c_update (&_delay, ACTIVATION, &_action, ACTIVATION, true)
  {
    Process::finalize_construction (parent, name);
  }

  Timer::Timer (Process *parent, const std::string& name, std::chrono::milliseconds delay)
  : Timer (parent, name, delay.count())
  {
  }

#if DJNN_USE_BOOST_CHRONO
  Timer::Timer (Process* parent, const std::string& name, boost::chrono::milliseconds delay)
  : Timer(parent, name, delay.count())
  {
  }
#endif

  Timer::~Timer ()
  {
  }

  
  void
  Timer::impl_activate ()
  {
    //DBG;
    _c_update.enable ();
    DjnnTimeManager::instance().after(this, _delay.get_value ());
  }

  void
  Timer::impl_deactivate ()
  {
    //DBG;
    _c_update.disable ();
    DjnnTimeManager::instance().cancel(this);
  }

  void
  Timer::update_period()
  {
    //DBG;
    DjnnTimeManager::instance().cancel(this);
    if(somehow_activating())
      //impl_activate (); // reschedule
      DjnnTimeManager::instance().after(this, _delay.get_value ());
  }

  void
  Timer::doit(const djnn_internal::Time::Unit& actualtime)
  {
    //DBG;
    set_activation_state (DEACTIVATED);
    _end.notify_activation (); // propagating
  }



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



}
