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

#include "core/ontology/coupling.h"
#include "core/control/action.h"
#include "core/tree/component.h"
#include "core/tree/text_property.h"

namespace djnn {

  class Switch : public Container
  {
  private:
    class SwitchAction : public Action
    {
    public:
      SwitchAction (Switch * parent, const std::string& name);
      virtual ~SwitchAction () {};
      void impl_activate () override { _sw->change_branch(); };
    private:
      Switch* _sw;
    };
  public:
    Switch (ParentProcess* parent, const std::string& name, const std::string& initial);
    //virtual process_type_e get_process_type () const override { return SWITCH_T; }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    virtual ~Switch ();
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const std::string& format) override;
#endif
  private:
    void set_parent (ParentProcess* parent) override;
    void change_branch ();
    friend void SwitchAction::impl_activate ();
    std::string _initial;
    TextProperty _branch_name;
    SwitchAction _action;
    Coupling _c_branch;
    CoreProcess* _cur_branch;
  };

}
