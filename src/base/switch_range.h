/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
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
#include "core/tree/double_property.h"

namespace djnn {
  using namespace std;

  class SwitchRangeBranch : public Container
  {
  public:
    SwitchRangeBranch (Process *parent, const string& name, double lower, bool left_open, double upper, bool right_open);
    ~SwitchRangeBranch ();
    bool is_in_range (double v);
    void serialize (const string& type) override;
  private:
    bool _left_open, _right_open;
    DoubleProperty _lower, _upper;
  };

  class SwitchRange : public Container
  {
  private:
    class SwitchRangeAction : public Action
    {
    public:
      SwitchRangeAction (SwitchRange *parent, const string &name);
      virtual ~SwitchRangeAction () {};
      void impl_activate () override { _sw->change_branch(); };
      void impl_deactivate () override {};
      void serialize (const string& type) override {};
    private:
      SwitchRange* _sw;
    };
  public:
    SwitchRange (Process *parent, const string &name, double initial);
    SwitchRange (double initial);
    virtual process_type_e get_cpnt_type () const override { return SWITCH_T; }
    void impl_activate () override;
    void impl_deactivate () override;
    void draw () override;
    void pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    virtual ~SwitchRange ();
    void serialize (const string& type) override;
  private:
    void set_parent (Process* p) override;
    void change_branch ();
    friend void SwitchRangeAction::impl_activate ();
    double _initial;
    DoubleProperty _branch_range;
    SwitchRangeAction _action;
    Coupling _c_branch;
    Process* _cur_branch;
  };

}
