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
 *
 */

#pragma once

#include <ctype.h>
#include <phidget22.h>

#include "core/control/action.h"
#include "core/property/double_property.h"
#include "core/tree/list.h"
#include "exec_env/external_source.h"
#include "extra/phidgets/phidgets-priv.h"

namespace djnn {

extern bool in_init;

class PhidgetsMonitor : public FatProcess {
  public:
    PhidgetsMonitor (CoreProcess* parent, const string& n)
        : FatProcess (n), _handle (nullptr) {}
    virtual ~PhidgetsMonitor () {}

  protected:
    virtual void impl_activate () override;
    virtual void impl_deactivate () override;

  private:
    PhidgetManagerHandle _handle;
};

class VoltageInputChannel : public FatProcess {
  private:
    class ChangeRateAction : public Action {
      public:
        ChangeRateAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        virtual ~ChangeRateAction () {}

      protected:
        void impl_activate () override { ((VoltageInputChannel*)get_parent ())->update_rate (); }
    };
    class ChangeTriggerAction : public Action {
      public:
        ChangeTriggerAction (CoreProcess* parent, const string& name)
            : Action (parent, name) {}
        virtual ~ChangeTriggerAction () {}

      protected:
        void impl_activate () override { ((VoltageInputChannel*)get_parent ())->update_trigger (); }
    };

  public:
    VoltageInputChannel (CoreProcess* parent, const string& name);
    virtual ~VoltageInputChannel ();
    void                       update_rate ();
    void                       update_trigger ();
    DoubleProperty*            get_value () { return _value; }
    PhidgetVoltageInputHandle* get_channel () { return &_sw_ch; }
    void                       end_init () { _end_init->activate (); }

  protected:
    void impl_activate () override;
    void impl_deactivate () override;

  private:
    FatProcess*               _end_init;
    DoubleProperty*           _value;
    IntProperty*              _rate;
    DoubleProperty*           _trigger;
    ChangeRateAction*         _rate_action;
    ChangeTriggerAction*      _trigger_action;
    Coupling *                _c_rate, *_c_trigger;
    PhidgetVoltageInputHandle _sw_ch;
};
class IFaceKit888 : public FatProcess {
    friend PhidgetsMonitor;

  public:
    IFaceKit888 (CoreProcess* parent, const string& name);
    virtual ~IFaceKit888 ();
    vector<DoubleProperty*>&      get_inputs () { return _digitalIn; }
    vector<DoubleProperty*>&      get_outputs () { return _digitalOut; }
    vector<VoltageInputChannel*>& get_sensors () { return _analogIn; }

  protected:
    virtual void impl_activate () override;
    virtual void impl_deactivate () override;

  private:
    vector<DoubleProperty*>      _digitalIn;
    vector<DoubleProperty*>      _digitalOut;
    vector<VoltageInputChannel*> _analogIn;
    List*                        _inputs;
    List*                        _outputs;
    List*                        _sensors;
};
} // namespace djnn
