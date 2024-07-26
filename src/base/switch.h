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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#pragma once

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/property/text_property.h"
#include "core/tree/component.h"

namespace djnn {

class Switch : public Container, public DelegatingProcess {
  private:
    class SwitchAction : public Action {
      public:
        SwitchAction (Switch* parent, const string& name);
        virtual ~SwitchAction (){};
        void impl_activate () override { _sw->change_branch (); };

      private:
        Switch* _sw;
    };

  public:
    Switch (CoreProcess* parent, const string& name, const string& initial);
    // virtual process_type_e get_process_type () const override { return SWITCH_T; }
    void            impl_activate () override;
    void            impl_deactivate () override;
    void            draw () override;
    void            pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    CoreProcess*       get_delegate () override { return _cur_branch; }
    const CoreProcess* get_delegate () const override { return _cur_branch; }
    virtual ~Switch ();
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    void         set_parent (CoreProcess* parent) override;
    void         change_branch ();
    friend void  SwitchAction::impl_activate ();
    string       _initial;
    TextProperty _branch_name;
    SwitchAction _action;
    Coupling     _c_branch;
    CoreProcess* _cur_branch;
    bool         _has_been_initialized_with_parent;
};

} // namespace djnn
