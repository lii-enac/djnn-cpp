/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "math_functions.h"

#include "core/core-dev.h" // graph add/remove edge
#include "core/serializer/serializer.h"

namespace djnn {

template <>
const char name_info<my_exp<double>>::left[] = "left";
template <>
const char name_info<my_exp<double>>::right[] = "right";
template <>
const char name_info<my_exp<double>>::serialize[] = "exp";

template <>
const char name_info<my_log<double>>::left[] = "left";
template <>
const char name_info<my_log<double>>::right[] = "right";
template <>
const char name_info<my_log<double>>::serialize[] = "log";

template <>
const char name_info<my_log10<double>>::left[] = "left";
template <>
const char name_info<my_log10<double>>::right[] = "right";
template <>
const char name_info<my_log10<double>>::serialize[] = "log10";

template <>
const char name_info<my_sqrt<double>>::left[] = "left";
template <>
const char name_info<my_sqrt<double>>::right[] = "right";
template <>
const char name_info<my_sqrt<double>>::serialize[] = "sqrt";

template <>
const char name_info<my_abs<double>>::left[] = "left";
template <>
const char name_info<my_abs<double>>::right[] = "right";
template <>
const char name_info<my_abs<double>>::serialize[] = "abs";

template <>
const char name_info<my_pow<double>>::left[] = "base";
template <>
const char name_info<my_pow<double>>::right[] = "exponent";
template <>
const char name_info<my_pow<double>>::serialize[] = "pow";

template <>
const char name_info<my_min<double>>::left[] = "left";
template <>
const char name_info<my_min<double>>::right[] = "right";
template <>
const char name_info<my_min<double>>::serialize[] = "min";

template <>
const char name_info<my_max<double>>::left[] = "left";
template <>
const char name_info<my_max<double>>::right[] = "right";
template <>
const char name_info<my_max<double>>::serialize[] = "max";

template <>
const char name_info<my_clamp_min<double>>::left[] = "min";
template <>
const char name_info<my_clamp_min<double>>::right[] = "input";
template <>
const char name_info<my_clamp_min<double>>::serialize[] = "clamp_min";

template <>
const char name_info<my_clamp_max<double>>::left[] = "max";
template <>
const char name_info<my_clamp_max<double>>::right[] = "input";
template <>
const char name_info<my_clamp_max<double>>::serialize[] = "clamp_max";

BoundedValue::BoundedValue (CoreProcess* parent, const string& name, double min, double max, double init_val)
    : FatProcess (name),
      _min (this, "min", min),
      _max (this, "max", max),
      _input (this, "input", init_val),
      _result (this, "result", init_val < min ? min : (init_val > max ? max : init_val)),
      _action (this, "action", *this),
      _c_min (&_min, ACTIVATION, &_action, ACTIVATION),
      _c_max (&_max, ACTIVATION, &_action, ACTIVATION),
      _c_input (&_input, ACTIVATION, &_action, ACTIVATION)
{
    graph_add_edge (&_action, &_result);
    finalize_construction (parent, name);
}

BoundedValue::~BoundedValue ()
{
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }
    graph_remove_edge (&_action, &_result);
}

void
BoundedValue::set_parent (CoreProcess* parent)
{
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()) {
        remove_state_dependency (get_parent (), &_action);
    }

    add_state_dependency (parent, &_action);

    FatProcess::set_parent (parent);
}

#ifndef DJNN_NO_SERIALIZE
void
BoundedValue::serialize (const string& type)
{
    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("base:boundedvalue");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("min", _min.get_value ());
    AbstractSerializer::serializer->int_attribute ("max", _max.get_value ());
    AbstractSerializer::serializer->int_attribute ("input", _input.get_value ());

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif
} // namespace djnn
