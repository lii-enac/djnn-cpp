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
#include "../tree/int_property.h"
#include "../execution/graph.h"
#include "../serializer/serializer.h"

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>


#include <iostream>

#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  Timer::Timer (std::chrono::milliseconds period)
  {
    _delay = shared_ptr<IntProperty> (new IntProperty (this, "delay", period.count ()));
    _end = shared_ptr<Blank> (new Blank (this, "end"));
  }

  Timer::Timer (Process *p, const std::string& n, std::chrono::milliseconds period) :
      Process (p, n)
  {
    _delay = shared_ptr<IntProperty> (new IntProperty (this, "delay", period.count ()));
    _end = shared_ptr<Blank> (new Blank (this, "end"));
    Process::finalize ();
  }

  Timer::Timer (int period)
  {
    _delay = shared_ptr<IntProperty> (new IntProperty (this, "delay", period));
    _end = shared_ptr<Blank> (new Blank (this, "end"));
  }

  Timer::Timer (Process *p, const std::string& n, int period) :
      Process (p, n)
  {
    _delay = shared_ptr<IntProperty> (new IntProperty (this, "delay", period));
    _end = shared_ptr<Blank> (new Blank (this, "end"));
    Process::finalize ();
  }

  void
  Timer::activate ()
  {
    if (_activation_state == activated)
      return;
    start_thread();
  }

  void
  Timer::deactivate ()
  {
    please_stop ();
  }

  void
  Timer::run ()
  {
    try {
        boost
        ::chrono::milliseconds duration (_delay->get_value ());
        boost
        ::this_thread::sleep_for (duration); // blocking call
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if (!get_please_stop ()) {
          _end->notify_activation (); // propagating
          Graph::instance ().exec (); // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    } catch (exception& e) {
      std::cerr << e.what() << std::endl;
    }
    _activation_state = deactivated;
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
