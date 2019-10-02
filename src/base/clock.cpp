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

#include <sys/time.h>

#include <iostream>
#define __FL__ __FILE__ ":" << __LINE__
#define DBG std::cerr << " " __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  Clock::Clock (milliseconds period)
  : Clock(period.count ())
  {
  }

  Clock::Clock (Process *p, const std::string& n, milliseconds period)
  : Clock(p, n, period.count ())
  {
  }

  Clock::Clock (int period)
  {
    _period = new IntProperty (this, "period", period);
    _elapsed = new DoubleProperty (this, "elapsed", 0);
    _tick = new Spike (this, "tick");
  }

  Clock::Clock (Process *p, const std::string& n, int period) :
      Process (n)
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << (p ? p->get_name() : "") << "/" << get_name() << std::endl; 
    _period = new IntProperty (this, "period", period);
    _elapsed = new DoubleProperty (this, "elapsed", 0);
    _tick = new Spike (this, "tick");
    Process::finalize_construction (p);
  }

  Clock::~Clock ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    delete _tick;
    delete _elapsed;
    delete _period;
  }

  void
  Clock::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    start_thread();
  }

  void
  Clock::impl_deactivate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    please_stop ();
  }

  void
  Clock::run ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    struct timespec before;
    struct timespec after;
    set_please_stop (false);

    try {
      while (!get_please_stop ()) {
        djnn::get_exclusive_access (DBG_GET);
        if(thread_local_cancelled) break;
        int duration = _period->get_value ();
        djnn::release_exclusive_access (DBG_REL);

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

        if (!get_please_stop ()) {
          get_monotonic_time(&after);
          double elapsedTime = (after.tv_sec * 1000 + after.tv_nsec * 1e-6) - (before.tv_sec * 1000 + before.tv_nsec * 1e-6);
          _elapsed->set_value (elapsedTime, true);
          _tick->activate (); // propagating
          
          GRAPH_EXEC; // executing
          if(thread_local_cancelled) {
            djnn::release_exclusive_access (DBG_REL); 
            //std::cerr << " cancelled" << __FL__ << std::endl;
            break;
          }
        } else {
          djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
          break;
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
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

  void
  Clock::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:clock");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("period", _period->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}





