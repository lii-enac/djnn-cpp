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

#include "cpp-thread.h"
#include "cpp-chrono.h"

#include <iostream>
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{

  Timer::Timer (int period)
  {
    _delay = new IntProperty (this, "delay", period);
    _end = new Blank (this, "end");
  }

  Timer::Timer (Process *p, const std::string& n, int period)
  : Process (n)
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
    _delay = new IntProperty (this, "delay", period);
    _end = new Blank (this, "end");
    Process::finalize_construction (p);
  }

  Timer::Timer (chrono::milliseconds period)
  : Timer(period.count())
  {
  }

  Timer::Timer (Process *p, const std::string& n, chrono::milliseconds period)
  : Timer(p, n, period.count())
  {
  }

#if !DJNN_USE_STD_CHRONO
  Timer::Timer (std::chrono::milliseconds period)
  : Timer(period.count())
  {
  }

  Timer::Timer (Process *p, const std::string& n, std::chrono::milliseconds period)
  : Timer(p, n, period.count())
  {
  }
#endif

  Timer::~Timer ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
    delete _end;
    delete _delay;
  }

  void
  Timer::impl_activate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    start_thread();
  }

  void
  Timer::impl_deactivate ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name() << std::endl;
    please_stop ();
  }

  void
  Timer::run ()
  {
    //DBG;
    set_please_stop (false);
    try {
        djnn::get_exclusive_access (DBG_GET);
        if(thread_local_cancelled) {
          thread_terminated ();
          return;
        }
        int duration = _delay->get_value ();
        djnn::release_exclusive_access (DBG_REL);

        //std::cerr << "entering sleep" << std::endl;
        djnn::sleep(duration);
        //std::cerr << "exit sleep" << std::endl;

        if(thread_local_cancelled) {
          //thread_terminated ();
          //return;
        } else {
          djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
          // the previous call my take some time, check if we have been cancelled meanwhile
          if(!thread_local_cancelled && !get_please_stop ()) {
            set_activation_state (DEACTIVATED);
            _end->notify_activation (); // propagating
            GRAPH_EXEC; // executing
          }
          djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
        }
    } catch (exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }
#if DJNN_USE_BOOST_THREAD
    catch(boost::thread_interrupted const& ) {
        //clean resources
        //std::cout << "thread interrupted" << std::endl;
    }
#endif
    thread_terminated ();
    //deactivate ();
  }

  void
  Timer::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("core:timer");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("delay", _delay->get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}
