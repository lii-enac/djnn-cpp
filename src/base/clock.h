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
#include "core/tree/int_property.h"
#include "core/tree/double_property.h"
#include "core/tree/spike.h"
#include "core/control/action.h"

#include "exec_env/time_manager.h"


namespace djnn
{
  class IntProperty;
  class DoubleProperty;

  class Clock : public Process, public djnn_internal::Time::Timer
  {
    class ClockAction : public Action
    {
    public:
      ClockAction (Process* parent, const std::string& name) :
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
 #ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format);
#endif
    // djnn_internal::Time::Timer
    virtual void do_it(const djnn_internal::Time::duration& actualduration) override;

    void update_period ();

  private:
    IntProperty _period;
    DoubleProperty _elapsed;
    Spike _tick;
    ClockAction _action;
    Coupling _c_update;
  };

}
