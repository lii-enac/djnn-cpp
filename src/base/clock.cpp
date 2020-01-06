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

#include "clock.h"
#include "core/syshook/syshook.h"
#include "core/tree/int_property.h"
#include "core/execution/graph.h"
#include "core/serializer/serializer.h"
#include "core/utils/utils-dev.h"
#include "core/syshook/cpp-thread.h"
#include "core/syshook/djnn_time_manager.h"

#include <sys/time.h>

// #include <iostream>
// #define __FL__ __FILE__ ":" << __LINE__
// #define DBG std::cerr << " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{

  Clock::Clock (Process *parent, const std::string& name, std::chrono::milliseconds period)
  : Clock(parent, name, period.count ())
  {
  }

  Clock::Clock (Process *parent, const std::string& name, int period)
  :
    Process (name),
    _period (this, "period", period),
    _elapsed (this, "elapsed", 0),
    _tick (this, "tick")
  {
    Process::finalize_construction (parent, name);
  }


#if DJNN_USE_BOOST_CHRONO
  Clock::Clock (Process* parent, const std::string& name, boost::chrono::milliseconds period)
  : Clock(parent, name, period.count())
  {
  }
#endif
  Clock::~Clock ()
  {
  }

  void
  Clock::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    //start_thread();
    DjnnTimeManager::instance().after(this, _period.get_value ());
  }

  void
  Clock::impl_deactivate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    //please_stop ();
    DjnnTimeManager::instance().cancel(this);
  }

  void
  Clock::doit(const djnn_internal::Time::Unit& actualtime)
  {
    //DBG;
    _elapsed.set_value (actualtime, true);
    _tick.activate (); // propagating
    DjnnTimeManager::instance().after(this, _period.get_value ());
  }

#if 0
  void
  Clock::run ()
  {
    std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    set_please_stop (false);

    djnn::get_exclusive_access (DBG_GET);
    if(thread_local_cancelled) {
      djnn::release_exclusive_access (DBG_REL);
      return;
    }
    int duration = _period.get_value ();
    djnn::release_exclusive_access (DBG_REL);

    try {
      while (!get_please_stop ()) {
        struct timespec before;
        get_monotonic_time(&before);

        //std::cerr << "entering sleep" << std::endl;
        djnn::sleep(duration);
        //std::cerr << "exit sleep" << std::endl;
        
        if(thread_local_cancelled) {
          //std::cerr << " cancelled" << __FL__ << std::endl;
          break;
        }

        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if(thread_local_cancelled) {
          djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
          //std::cerr << " cancelled" << __FL__ << std::endl;
          break;
        }
        if (get_please_stop ()) {
          djnn::release_exclusive_access (DBG_REL); 
          break;
        } else {
          struct timespec after;
          get_monotonic_time(&after);
          double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
          _elapsed.set_value (elapsedTime, true);
          _tick.activate (); // propagating
          
          GRAPH_EXEC; // executing
          if(thread_local_cancelled) {
            djnn::release_exclusive_access (DBG_REL); 
            //std::cerr << " cancelled" << __FL__ << std::endl;
            break;
          }
          duration = _period.get_value ();
          djnn::release_exclusive_access (DBG_REL); 
        }
          
      }
      //std::cerr << this << " << stop" << std::endl;

    } catch (exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }
#if DJNN_USE_BOOST_THREAD
    catch(boost::thread_interrupted const& ) {
        //clean resources
        //std::cout << "thread interrupted" << std::endl;
    }
#endif
    //std::cout << "clock thread terminating..." << std::endl;
    //thread_terminated ();
    //cancelled = nullptr;
  }
#endif

  void
  Clock::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:clock");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("period", _period.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}





