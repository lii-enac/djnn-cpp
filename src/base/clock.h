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

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/syshook/external_source.h"
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/spike.h"
#include "core/control/action.h"

#include "core/syshook/cpp-chrono.h"
#include "core/syshook/time_manager.h"


namespace djnn
{
  class IntProperty;
  class DoubleProperty;

  class Clock : public Process, public djnn_internal::Time::Timer
  {
    class ClockAction : public Action
    {
    public:
      ClockAction (Process* parent, const string &name) :
        Action (parent, name){};
    
      virtual ~ClockAction () {}
      void impl_activate () override { ((Clock*)get_parent())->update_period (); }
      void impl_deactivate () override {}
    };
  public:
    Clock (Process* p, const std::string& n, std::chrono::milliseconds period = std::chrono::seconds(1));
    Clock (Process* p, const std::string& n, int period = 1000);
#if DJNN_USE_BOOST_CHRONO
    Clock (Process* p, const std::string& n, boost::chrono::milliseconds period = boost::chrono::milliseconds(1000));
#endif
    virtual ~Clock ();

  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    void update_period ();
    // djnn_internal::Time::Timer
    virtual void doit(const djnn_internal::Time::Unit& actualtime) override;

  private:
    IntProperty _period;
    DoubleProperty _elapsed;
    Spike _tick;
    ClockAction _action;
    Coupling _c_update;
  };

}
