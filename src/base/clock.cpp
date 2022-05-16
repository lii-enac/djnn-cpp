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

#include "clock.h"
#include "core/serializer/serializer.h"
#include "exec_env/djnn_time_manager.h"

// dbg
//#include "core/utils/iostream.h"
//#include "utils/debug.h"

namespace djnn
{

  Clock::Clock (ParentProcess* parent, const string& name, std::chrono::milliseconds period)
  : Clock(parent, name, period.count ())
  {
  }

  Clock::Clock (ParentProcess* parent, const string& name, int period)
  :
    FatProcess (name),
    _period (this, "period", period),
    _elapsed (this, "elapsed", 0),
    _tick (this, "tick"),
    _updateaction (this, "action"),
    _c_update (&_period, ACTIVATION, &_updateaction, ACTIVATION, true)
  {
    finalize_construction (parent, name);
  }


#if DJNN_USE_BOOST_CHRONO
  Clock::Clock (ParentProcess* parent, const string& name, boost::chrono::milliseconds period)
  : Clock(parent, name, period.count())
  {
  }
#endif
  Clock::~Clock ()
  {
  }

  // FatProcess
  void
  Clock::impl_activate ()
  {
    //std::cerr << DBGVAR(_period.get_value ()) << __FL__;
    //std::cerr << "activate " << get_name () << __FL__;
    _c_update.enable ();
    djnn_internal::Time::duration d = std::chrono::milliseconds(_period.get_value ());
    DjnnTimeManager::instance().schedule(this, d);
  }

  void
  Clock::impl_deactivate ()
  {
     _c_update.disable ();
    if(is_already_scheduled ())
      DjnnTimeManager::instance().cancel(this);
  }

  // djnn_internal::Time::Timer
  void
  Clock::do_it(const djnn_internal::Time::duration& actualduration)
  {
    _elapsed.set_value ((double)actualduration.count()/1000, true);
    //auto sav_period = _period.get_value ();
    _tick.schedule_activation ();
    if(somehow_activating()
      //&& !DjnnTimeManager::instance().already_scheduled(this)) // activate could have updated period // FIXME costly
      //&& sav_period == _period.get_value ()
    )
    {
      if(!is_already_scheduled ()) {// _tick.activate() below might have alread scheduled it
        djnn_internal::Time::duration d = std::chrono::milliseconds(_period.get_value ());
        DjnnTimeManager::instance().schedule(this, d, get_end_time());
      }
    }
  }

  void
  Clock::update_period()
  { //DBG
    if(is_already_scheduled ())
      DjnnTimeManager::instance().cancel(this);
    if(somehow_activating()) {
      djnn_internal::Time::duration d = std::chrono::milliseconds(_period.get_value ());
      DjnnTimeManager::instance().schedule(this, d);
    }
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Clock::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:clock");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("period", _period.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

}
