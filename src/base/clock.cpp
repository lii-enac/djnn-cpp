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
#include "../core/syshook/syshook.h"
#include "../core/tree/int_property.h"
#include "../core/execution/graph.h"

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>


#include <iostream>

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  Clock::Clock (std::chrono::milliseconds period)
  {
    _period = shared_ptr<IntProperty> (new IntProperty (this, "period", period.count ()));
    _tick = shared_ptr<Process> (new Spike (this, "tick"));
  }

  Clock::Clock (Process *p, const std::string& n, std::chrono::milliseconds period) :
      Process (p, n)
  {
    _period = shared_ptr<IntProperty> (new IntProperty (this, "period", period.count ()));
    _tick = shared_ptr<Process> (new Spike (this, "tick"));
    Process::finalize ();
  }

  Clock::Clock (int period)
  {
    _period = shared_ptr<IntProperty> (new IntProperty (this, "period", period));
    _tick = shared_ptr<Process> (new Spike (this, "tick"));
  }

  Clock::Clock (Process *p, const std::string& n, int period) :
      Process (p, n)
  {
    _period = shared_ptr<IntProperty> (new IntProperty (this, "period", period));
    _tick = shared_ptr<Process> (new Spike (this, "tick"));
    Process::finalize ();
  }

  void
  Clock::activate ()
  {
    //djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
    //set_please_stop (true);
    //if ( _thread.joinable() ) _thread.join();
    please_stop();
    start_thread();

    //djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
  }

  void
  Clock::deactivate ()
  {
    please_stop ();
  }

  void
  Clock::run ()
  {
    set_please_stop (false);
    try {
      //std::cerr << this << " >> run" << std::endl;
      while (!get_please_stop ()) {
        //std
        boost
        ::chrono::milliseconds duration (_period->get_value ());
        //std::cerr << this << "  >> sleep " << duration.count() << std::endl;
        //std
        boost
        ::this_thread::sleep_for (duration); // blocking call
        //std::cerr << this << "  << sleep end" << std::endl;
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!get_please_stop ()) {
          _tick.get()->activation (); // propagating
          Graph::instance ().exec (); // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
      }
      //std::cerr << this << " << stop" << std::endl;

    } catch (exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

}

