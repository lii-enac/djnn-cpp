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

#include "core/control/action.h"
#include "core/control/blank.h"
#include "core/control/spike.h"
#include "core/ontology/coupling.h"
#include "core/ontology/process.h"
#include "core/property/int_property.h"
#include "cpp-chrono.h"
#include "exec_env/time_internal.h"

namespace djnn {
class Timer : public FatProcess, public djnn_internal::Time::Timer {
    class TimerAction : public Action {
      public:
        TimerAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {};
        virtual ~TimerAction () {}
        void impl_activate () override { ((Timer*)get_parent ())->update_period (); }
    };

    class ResetAction : public Action {
      public:
        ResetAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {};
        virtual ~ResetAction () {}
        void impl_activate () override { ((Timer*)get_parent ())->update_period (); }
    };

  public:
    Timer (CoreProcess* parent, const string& n, std::chrono::milliseconds delay = std::chrono::milliseconds (1000), bool model = false);
    Timer (CoreProcess* parent, const string& n, int period = 1000, bool lazy = false);
#if DJNN_USE_BOOST_CHRONO
    Timer (CoreProcess* parent, const string& n, boost::chrono::milliseconds delay = boost::chrono::milliseconds (1000));
#endif
    virtual ~Timer ();
    CoreProcess* get_delay () { return &_delay; }
    CoreProcess* get_end () { return &_end; }
    CoreProcess* impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const override;

    // djnn_internal::Time::Timer
    virtual void do_it (const djnn_internal::Time::duration& actualduration) override;

  protected:
    // FatProcess
    void impl_activate () override;
    void impl_deactivate () override;

    void update_period ();

  private:
    IntProperty _delay;
    Blank       _end;
    TimerAction _action;
    Coupling    _c_update;
    Spike       _reset;
    ResetAction _reset_action;
    Coupling    _c_reset;

#ifndef DJNN_NO_SERIALIZE
    void serialize (const string& type) override;
#endif
};

} // namespace djnn
