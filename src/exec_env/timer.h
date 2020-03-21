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

#pragma once

#include "core/ontology/process.h"
#include "core/ontology/coupling.h"
#include "core/tree/int_property.h"
#include "core/tree/blank.h"
#include "core/tree/spike.h"
#include "core/control/action.h"

#include "exec_env/time_manager.h"
#include "cpp-chrono.h"

namespace djnn
{
  class Timer : public Process, public djnn_internal::Time::Timer
  {
    class TimerAction : public Action
    {
    public:
      TimerAction (Process* parent, const std::string& name) :
        Action (parent, name) {};
      virtual ~TimerAction () {}
      void impl_activate () override { ((Timer*)get_parent())->update_period (); }
      void impl_deactivate () override {}
    };

    class ResetAction : public Action
    {
    public:
      ResetAction (Process* parent, const std::string& name) :
        Action (parent, name) {};
      virtual ~ResetAction () {}
      void impl_activate () override { ((Timer*)get_parent())->update_period (); }
      void impl_deactivate () override {}
    };

  public:
    Timer (Process* p, const std::string& n, std::chrono::milliseconds delay = std::chrono::milliseconds(1000));
    Timer (Process* p, const std::string& n, int period = 1000);
#if DJNN_USE_BOOST_CHRONO
    Timer (Process* p, const std::string& n, boost::chrono::milliseconds delay = boost::chrono::milliseconds(1000));
#endif
    virtual ~Timer ();

    // djnn_internal::Time::Timer
    virtual void do_it(const djnn_internal::Time::duration& actualduration) override;

  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;

    void serialize (const std::string& type) override;
    void update_period ();

  private:
    IntProperty _delay;
    Blank _end;
    TimerAction _action;
    Coupling _c_update;
    Spike _reset;
    ResetAction _reset_action;
    Coupling _c_reset;
  };

}
