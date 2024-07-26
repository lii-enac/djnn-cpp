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

#include "core/control/action.h"
#include "core/ontology/coupling.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"
#include "core/tree/component.h"

namespace djnn {

class SwitchRangeBranch : public Container {
  public:
    SwitchRangeBranch (CoreProcess* parent, const string& name, double lower, bool left_open, double upper, bool right_open);
    ~SwitchRangeBranch ();
    bool is_in_range (double v);
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    bool           _left_open, _right_open;
    DoubleProperty _lower, _upper;
};

class SwitchRange : public Container, public DelegatingProcess {
  private:
    class SwitchRangeAction : public Action {
      public:
        SwitchRangeAction (SwitchRange* parent, const string& name);
        virtual ~SwitchRangeAction (){};
        void impl_activate () override { _sw->change_branch (); };

      private:
        SwitchRange* _sw;
    };

  public:
    SwitchRange (CoreProcess* parent, const string& name, double initial);
    // virtual process_type_e get_process_type () const override { return SWITCH_T; }
    void            impl_activate () override;
    void            impl_deactivate () override;
    void            draw () override;
    void            pick () override;
    AbstractGShape* pick_analytical (PickAnalyticalContext& pac) override;
    CoreProcess*       get_delegate () override { return _cur_branch; }
    const CoreProcess* get_delegate () const override { return _cur_branch; }
    virtual ~SwitchRange ();
#ifndef DJNN_NO_SERIALIZE
    virtual void serialize (const string& format) override;
#endif
  private:
    void              set_parent (CoreProcess* parent) override;
    void              change_branch ();
    friend void       SwitchRangeAction::impl_activate ();
    double            _initial;
    DoubleProperty    _input;
    TextProperty      _branch_name;
    SwitchRangeAction _action;
    Coupling          _c_branch;
    CoreProcess*      _cur_branch;
};

} // namespace djnn
