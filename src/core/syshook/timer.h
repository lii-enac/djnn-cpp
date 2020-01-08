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
#include "core/tree/blank.h"
#include "core/control/action.h"

//#include "external_source.h"
#include "core/syshook/time_manager.h"
#include "cpp-chrono.h"

namespace djnn
{
  class IntProperty;
  class Blank;

  class Timer : public Process, public djnn_internal::Time::Timer //ExternalSource
  {
    class TimerAction : public Action
    {
    public:
      TimerAction (Process* parent, const string &name) :
        Action (parent, name){};
    
      virtual ~TimerAction () {}
      void impl_activate () override { ((Timer*)get_parent())->update_period (); }
      void impl_deactivate () override {}
    };
  public:
    Timer (Process* p, const std::string& n, std::chrono::milliseconds period = std::chrono::milliseconds(1000));
    Timer (Process* p, const std::string& n, int period = 1000);
#if DJNN_USE_BOOST_CHRONO
    Timer (Process* p, const std::string& n, boost::chrono::milliseconds period = boost::chrono::milliseconds(1000));
#endif
    virtual ~Timer ();

    // djnn_internal::Time::Timer
    virtual void doit(const djnn_internal::Time::Unit& actualtime) override;

  protected:
    // Process
    void impl_activate () override;
    void impl_deactivate () override;
    void serialize (const string& type) override;
    void update_period ();

  private:
    // ExternalSource
    //void run() override;

  private:
    IntProperty _delay;
    Blank _end;
    TimerAction _action;
    Coupling _c_update;

  };

}
