/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2020)
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
#include "core/serializer/serializer.h"
#include "exec_env/djnn_time_manager.h"

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
  _c_update (&_delay, ACTIVATION, &_action, ACTIVATION, true),
  _reset (this, "reset"),
  _reset_action (this, "reset_action"),
  _c_reset (&_reset, ACTIVATION, &_reset_action, ACTIVATION, true)
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

  
  // Process
  void
  Timer::impl_activate ()
  {
    _c_update.enable ();
    _c_reset.enable ();
    djnn_internal::Time::duration d = std::chrono::milliseconds(_delay.get_value ());
    DjnnTimeManager::instance().schedule(this, d);
  }

  void
  Timer::impl_deactivate ()
  {
    if(is_already_scheduled()) {
      DjnnTimeManager::instance().cancel(this);
    }
    _c_update.disable ();
    _c_reset.disable ();
    
  }

  // djnn_internal::Time::Timer
  void
  Timer::do_it(const djnn_internal::Time::duration & actualduration)
  {
    if(somehow_activating()) {
      set_activation_state (DEACTIVATED);
      _end.notify_activation ();
    }
  }

  void
  Timer::update_period()
  {
    DjnnTimeManager::instance().cancel(this);
    if(somehow_activating()) {
      djnn_internal::Time::duration d = std::chrono::milliseconds(_delay.get_value ());
      DjnnTimeManager::instance().schedule(this, d);
    }
  }


  void
  Timer::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("exec_env:timer");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("delay", _delay.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

}
