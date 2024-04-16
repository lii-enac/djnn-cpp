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

#include "arithmetic.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"
#include "incr.h"

// #include "utils/debug.h"
// #include "core/utils/utils-dev.h"
// #include "core/utils/iostream.h"
// #include "core/utils/error.h"

namespace djnn {

template <>
const char name_info<std::plus<double>>::left[] = "left";
template <>
const char name_info<std::plus<double>>::right[] = "right";
template <>
const char name_info<std::plus<double>>::serialize[] = "adder";

template <>
const char name_info<std::minus<double>>::left[] = "left";
template <>
const char name_info<std::minus<double>>::right[] = "right";
template <>
const char name_info<std::minus<double>>::serialize[] = "subtractor";

template <>
const char name_info<std::multiplies<double>>::left[] = "left";
template <>
const char name_info<std::multiplies<double>>::right[] = "right";
template <>
const char name_info<std::multiplies<double>>::serialize[] = "multiplier";

template <>
const char name_info<std::divides<double>>::left[] = "left";
template <>
const char name_info<std::divides<double>>::right[] = "right";
template <>
const char name_info<std::divides<double>>::serialize[] = "divider";

template <>
const char name_info<std::modulus<int>>::left[] = "left";
template <>
const char name_info<std::modulus<int>>::right[] = "right";
template <>
const char name_info<std::modulus<int>>::serialize[] = "modulo";

template <>
const char name_info<std::greater<double>>::left[] = "left";
template <>
const char name_info<std::greater<double>>::right[] = "right";
template <>
const char name_info<std::greater<double>>::serialize[] = "ascendingcomparator";

template <>
const char name_info<std::greater_equal<double>>::left[] = "left";
template <>
const char name_info<std::greater_equal<double>>::right[] = "right";
template <>
const char name_info<std::greater_equal<double>>::serialize[] = "strictascendingcomparator";

template <>
const char name_info<std::equal_to<double>>::left[] = "left";
template <>
const char name_info<std::equal_to<double>>::right[] = "right";
template <>
const char name_info<std::equal_to<double>>::serialize[] = "equalitycomparator";

template <>
const char name_info<std::negate<double>>::left[] = "left";
template <>
const char name_info<std::negate<double>>::right[] = "right";
template <>
const char name_info<std::negate<double>>::serialize[] = "signinverter";

int
Incr::init_incr (bool isModel)
{
    graph_add_edge (this, &_state);
    set_is_model (isModel);
    return 1;
}

Incr::Incr (CoreProcess* parent, const string& name, bool isModel)
    : FatProcess (name),
      _delta (this, "delta", 1),
      _state (this, "state", 0),
      _step (this, "step"),
      _action (this, "action"),
      _c_step (&_step, ACTIVATION, &_action, ACTIVATION),
      _is_model (isModel)
{
    init_incr (0);
    graph_add_edge (&_step, &_state);
    finalize_construction (parent, name);
}

Incr::~Incr ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_state);
    }
    graph_remove_edge (&_step, &_state);
    graph_remove_edge (this, &_state);
}

void
Incr::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_state);
    }

    add_state_dependency (parent, &_state);
    FatProcess::set_parent (parent);
}

bool
Incr::pre_activate ()
{
    /* no activation if :
     * 1 - the parent exists and is stopped
     *
     * WARNING - In order to keep the old behavior (step on activation) we relieve
     * the usual condition that the process cannot be activated if it is already
     * active.
     */
    if (get_parent () != nullptr && !get_parent ()->somehow_activating ()) {
        return false;
    }
    if (!is_activated ())
        set_activation_state (ACTIVATING);
    return true;
}

void
Incr::impl_activate ()
{
    _c_step.enable ();
    if (!_is_model || is_activated ())
        step ();
}

void
Incr::impl_deactivate ()
{
    _c_step.disable ();
}

void
Incr::step ()
{
    _state.set_value (_state.get_value () + _delta.get_value (), true);
}

#ifndef DJNN_NO_SERIALIZE
void
Incr::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:incr");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

OperationConnector::OperationConnector (CoreProcess* parent, const string& name, CoreProcess* dst, bool copy_on_activation)
    : FatProcess (name),
      _dst (dynamic_cast<AbstractSimpleProperty*> (dst)),
      _delta (this, "delta", 0),
      _action (this, "action"),
      _c_delta (&_delta, ACTIVATION, &_action, ACTIVATION), _copy_on_activation (copy_on_activation)
{
    graph_add_edge (&_delta, _dst);
    graph_add_edge (this, _dst);
    finalize_construction (parent, name);
}

OperationConnector::~OperationConnector ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (this, _dst);
    graph_remove_edge (&_delta, _dst);
}

void
OperationConnector::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);
    FatProcess::set_parent (parent);
}

// bool
// OperationConnector::pre_activate ()
// {
//     /* no activation if :
//      * 1 - the parent exists and is stopped
//      *
//      * WARNING - In order to keep the old behavior (step on activation) we relieve
//      * the usual condition that the process cannot be activated if it is already
//      * active.
//      */
//     if (get_parent () != nullptr && !get_parent ()->somehow_activating ()) {
//         return false;
//     }
//     if (!is_activated ())
//         set_activation_state (ACTIVATING);
//     return true;
// }

void
OperationConnector::impl_activate ()
{ // DBG;
    _c_delta.enable ();
    if (_copy_on_activation) // || is_activated ())
        perform_action ();
}

void
OperationConnector::perform_action ()
{
    // std::cerr << get_hierarchy_name(this) << _delta.get_value () << " " << _dst->get_double_value () << " ";
    _dst->set_value (_dst->get_double_value () + _delta.get_value (), true);
    // std::cerr << _dst->get_double_value () << __FL__;
}

void
OperationConnector::impl_deactivate ()
{ // DBG;
    _c_delta.disable ();
}

#ifndef DJNN_NO_SERIALIZE
void
OperationConnector::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:operation_assignment");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

AdderAccumulator::AdderAccumulatorAction::AdderAccumulatorAction (CoreProcess* parent, const string& name,
                                                                  AdderAccumulator& aa)
    : Action (parent, name), _aa (aa)
{
    finalize_construction (parent, name);
}

void
AdderAccumulator::AdderAccumulatorAction::impl_activate ()
{
    double input     = _aa._input.get_value ();
    double clamp_min = _aa._clamp_min.get_value ();
    double clamp_max = _aa._clamp_max.get_value ();
    double value     = _aa._result.get_value ();
    value += input;
    if (clamp_min < clamp_max) {
        value = value < clamp_min ? clamp_min : value;
        value = value > clamp_max ? clamp_max : value;
    }
    _aa._result.set_value (value, true);
}

AdderAccumulator::AdderAccumulator (CoreProcess* parent, const string& name, double input, double clamp_min,
                                    double clamp_max)
    : FatProcess (name),
      _input (this, "input", input),
      _clamp_min (this, "clamp_min", clamp_min),
      _clamp_max (this, "clamp_max", clamp_max),

      _result (this, "result", clamp_min >= clamp_max ? input : input < clamp_min ? clamp_min
                                                            : input > clamp_max   ? clamp_max
                                                                                  : input),
      _action (this, name + "_action", *this),
      _c_input (&_input, ACTIVATION, &_action, ACTIVATION)
{
    _c_input.disable ();
    graph_add_edge (&_action, &_result);
    finalize_construction (parent, name);
}

AdderAccumulator::~AdderAccumulator ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_result);
}

void
AdderAccumulator::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);
    FatProcess::set_parent (parent);
}

void
AdderAccumulator::impl_activate ()
{
    _c_input.enable ();
}

void
AdderAccumulator::impl_deactivate ()
{
    _c_input.disable ();
}

#ifndef DJNN_NO_SERIALIZE
void
AdderAccumulator::serialize (const string& type)
{

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:adderaccumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", _input.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_min", _clamp_min.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_max", _clamp_max.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
