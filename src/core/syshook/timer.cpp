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

#include "cpp-thread.h"
#include "cpp-chrono.h"

#include <iostream>
#define DBG std::cerr << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;

namespace djnn
{
  Timer::Timer (chrono::milliseconds period)
  : Timer(period.count())
  {
    //_delay = new IntProperty (this, "delay", period.count ());
    //_end = new Blank (this, "end");
  }

  Timer::Timer (Process *p, const std::string& n, chrono::milliseconds period) :
      //Process (n)
    Timer(p, n, period.count())
  {
    //_delay = new IntProperty (this, "delay", period.count ());
    //_end = new Blank (this, "end");
    //Process::finalize_construction (p);
  }

  Timer::Timer (int period)
  {
    _delay = new IntProperty (this, "delay", period);
    _end = new Blank (this, "end");
  }

  Timer::Timer (Process *p, const std::string& n, int period) :
      Process (n)
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
    _delay = new IntProperty (this, "delay", period);
    _end = new Blank (this, "end");
    Process::finalize_construction (p);
  }

  Timer::~Timer ()
  {
    //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << this->get_name() << std::endl;
    delete _end;
    delete _delay;
  }

  void
  Timer::impl_activate ()
  {
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
         chrono::milliseconds duration (_delay->get_value ());
        //this_thread::sleep_for (duration); // blocking call
        // std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name () << " before sleep " << _delay->get_value () << std::endl; 
        #if DJNN_USE_SDL_THREAD
        SDL_Delay(_delay->get_value ()); // blocking call
        #elif DJNN_USE_QT_THREAD && (QT_VERSION < QT_VERSION_CHECK(5,10,0))
        QThread::currentThread()->wait(duration.count());
        #else
        this_thread::sleep_for (duration); // blocking call
        #endif

        //launch_mutex_lock();
        //launch_mutex_unlock();
        
        //std::cerr << __PRETTY_FUNCTION__ << " " << this << " " << get_name () << " " << thread_local_cancelled << " " << &thread_local_cancelled << std::endl;
        if(thread_local_cancelled) {
          //std::cerr << this << " " << get_name () << " cancelled" << std::endl;
          return;
        }
        djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
        if(thread_local_cancelled) {
            //std::cerr << this << " " << get_name () << " cancelled" << std::endl;
            djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
            return;
        }
        if (!get_please_stop ()) {
          set_activation_state (DEACTIVATED);
          //std::cerr << this << " " << get_name () << " end notify_activation" << std::endl;
          _end->notify_activation (); // propagating
          
          thread_terminated ();
          GRAPH_EXEC; // executing
        }
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
    } catch (exception& e) {
      std::cerr << e.what() << __FILE__<< " " << __LINE__ << std::endl;
    }
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
