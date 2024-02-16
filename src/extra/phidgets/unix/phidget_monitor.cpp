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

#include "core/core-dev.h" // graph add/remove edge
#include "core/utils/error.h"
#include "core/utils/iostream.h"
#include "core/utils/to_string.h"
#include "exec_env/global_mutex.h"
#include "unix_phidgets.h"

namespace djnn {
void CCONV
on_sensor_change_handler (PhidgetVoltageInputHandle ch, void* ctx, double sensorValue)
{
    DoubleProperty* p = (DoubleProperty*)ctx;
    djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
    p->set_value (sensorValue, true);
    GRAPH_EXEC                                    // executing
        djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
}

static void CCONV
on_attach_handler (PhidgetManagerHandle man, void* ctx, PhidgetHandle channel)
{
    /* WARNING: this handler is called within the initialization sequence
     * if a Phidget is already plugged-in we probably should avoid to execute the graph */

    Phidget_DeviceID id;
    int              deviceLabel;
    Phidget_getDeviceSerialNumber (channel, &deviceLabel);
    Phidget_getDeviceID (channel, &id);
    switch (id) {
    case PHIDID_1010_1013_1018_1019: {
        IFaceKit888* dev = (IFaceKit888*)InterfaceKits->find_child_impl (djnnstl::to_string (deviceLabel));
        if (dev == nullptr) {

            djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
            dev = new IFaceKit888 (InterfaceKits, djnnstl::to_string (deviceLabel));
            dev->activate ();
            if (!in_init) {
                GRAPH_EXEC // executing
            }
            djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!
        }
        Phidget_ChannelClass channelClass;
        int                  num_channel;
        Phidget_getChannelClass (channel, &channelClass);
        Phidget_getChannel (channel, &num_channel);

        switch (channelClass) {
        case PHIDCHCLASS_DIGITALINPUT: {
            break;
        }
        case PHIDCHCLASS_DIGITALOUTPUT: {
            break;
        }
        case PHIDCHCLASS_VOLTAGEINPUT: {
            PhidgetVoltageInputHandle* sw_ch = (dev->get_sensors ()[num_channel])->get_channel ();
            PhidgetVoltageInput_create (sw_ch);

            Phidget_setDeviceSerialNumber ((PhidgetHandle)*sw_ch, deviceLabel);
            Phidget_setChannel ((PhidgetHandle)*sw_ch, num_channel);
            Phidget_openWaitForAttachment ((PhidgetHandle)*sw_ch, 1000);
            PhidgetVoltageInput_setVoltageChangeTrigger (*sw_ch, 0.0);
            Phidget_setDataInterval ((PhidgetHandle)*sw_ch, 250);
            PhidgetVoltageInput_setOnVoltageChangeHandler (
                (PhidgetVoltageInputHandle)*sw_ch, on_sensor_change_handler,
                (void*)(dev->get_sensors ()[num_channel]->get_value ()));

            djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
            dev->get_sensors ()[num_channel]->end_init ();
            GRAPH_EXEC
            djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!

            break;
        }
        default:
            return;
        }
        break;
    }
    default:
        std::cout << "Phidget not yet supported\n";
    }
}

static void CCONV
on_detach_handler (PhidgetManagerHandle man, void* ctx, PhidgetHandle channel)
{
    int              deviceLabel;
    Phidget_DeviceID id;
    Phidget_getDeviceSerialNumber (channel, &deviceLabel);
    Phidget_getDeviceID (channel, &id);
    switch (id) {
    case PHIDID_1010_1013_1018_1019: {
        IFaceKit888* dev = (IFaceKit888*)InterfaceKits->find_child_impl (djnnstl::to_string (deviceLabel));
        if (dev != nullptr) {

            djnn::get_exclusive_access (DBG_GET); // no break after this call without release !!
            InterfaceKits->remove_child (dev);
            GRAPH_EXEC
            djnn::release_exclusive_access (DBG_REL); // no break before this call without release !!

            delete dev;
        }
        break;
    }
    default: {
        return;
    }
    }
}

void
PhidgetsMonitor::impl_activate ()
{
    PhidgetReturnCode prc;
    PhidgetReturnCode res;
    PhidgetManager_create (&_handle);
    res = PhidgetManager_setOnAttachHandler (_handle, on_attach_handler, _handle);
    if (res != EPHIDGET_OK) {
        warning (Phidgets, "Unable to initialize Phidget attach handler");
        return;
    }
    res = PhidgetManager_setOnDetachHandler (_handle, on_detach_handler, _handle);
    if (res != EPHIDGET_OK) {
        warning (Phidgets, "Unable to initialize Phidget detach handler");
        return;
    }
    res = PhidgetManager_open (_handle);
    if (res != EPHIDGET_OK) {
        warning (Phidgets, "Unable to open Phidget manager");
        return;
    }
}

void
PhidgetsMonitor::impl_deactivate ()
{
    PhidgetManager_delete (&_handle);
}

IFaceKit888::IFaceKit888 (CoreProcess* parent, const string& name)
    : FatProcess (name)
{
    _inputs  = new List (this, "inputs");
    _outputs = new List (this, "outputs");
    _sensors = new List (this, "sensors");
    for (int i = 0; i < 8; i++) {
        _digitalIn.push_back (new DoubleProperty (_inputs, "", 0));
        _digitalOut.push_back (new DoubleProperty (_outputs, "", 0));
        _analogIn.push_back (new VoltageInputChannel (_sensors, ""));
    }
    finalize_construction (parent, name, nullptr);
}

IFaceKit888::~IFaceKit888 ()
{
    for (auto dp : _digitalIn) {
        _inputs->remove_child (dp);
        delete dp;
    }
    for (auto dp : _digitalOut) {
        _outputs->remove_child (dp);
        delete dp;
    }
    for (auto ai : _analogIn) {
        _sensors->remove_child (ai);
        delete ai;
    }
    delete _sensors;
    delete _outputs;
    delete _inputs;
}

void
IFaceKit888::impl_activate ()
{
    _inputs->activate ();
    _outputs->activate ();
    _sensors->activate ();
}

void
IFaceKit888::impl_deactivate ()
{
    _inputs->deactivate ();
    _outputs->deactivate ();
    _sensors->deactivate ();
}

VoltageInputChannel::VoltageInputChannel (CoreProcess* parent, const string& name)
    : FatProcess (name)
{
    _end_init       = new Spike (this, "end_init");
    _value          = new DoubleProperty (this, "value", 0.0);
    _rate           = new IntProperty (this, "rate", 250);
    _trigger        = new DoubleProperty (this, "trigger", 0.0);
    _rate_action    = new ChangeRateAction (this, "rate_action");
    _trigger_action = new ChangeTriggerAction (this, "trigger_action");
    _c_rate         = new Coupling (_rate, ACTIVATION, _rate_action, ACTIVATION);
    _c_trigger      = new Coupling (_trigger, ACTIVATION, _trigger_action, ACTIVATION);
    _sw_ch          = nullptr;
    finalize_construction (parent, name, nullptr);
}

VoltageInputChannel::~VoltageInputChannel ()
{
    if (_sw_ch != nullptr) {
        PhidgetVoltageInput_delete (&_sw_ch);
    }
    delete _c_trigger;
    delete _c_rate;
    delete _trigger_action;
    delete _rate_action;
    delete _trigger;
    delete _rate;
    delete _value;
    delete _end_init;
}

void
VoltageInputChannel::update_rate ()
{
    PhidgetReturnCode rec = PhidgetVoltageInput_setDataInterval (_sw_ch, _rate->get_value ());
    if (rec != EPHIDGET_OK) {
        warning (this, "Phidgets: failed to update data interval. Phidget error code: " + djnnstl::to_string (rec));
    }
}

void
VoltageInputChannel::update_trigger ()
{
    PhidgetReturnCode rec = PhidgetVoltageInput_setVoltageChangeTrigger (_sw_ch, _trigger->get_value ());
    if (rec != EPHIDGET_OK) {
        warning (this, "Phidgets: failed to update trigger. Phidget error code: " + djnnstl::to_string (rec));
    }
}

void
VoltageInputChannel::impl_activate ()
{
    _c_rate->enable ();
    _c_trigger->enable ();
}

void
VoltageInputChannel::impl_deactivate ()
{
    _c_rate->disable ();
    _c_trigger->disable ();
}
} // namespace djnn
