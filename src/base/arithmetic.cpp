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

namespace djnn
{

  template <> const char name_info<std::plus<double>>::left[] = "left";
  template <> const char name_info<std::plus<double>>::right[] = "right";
  template <> const char name_info<std::plus<double>>::serialize[] = "adder";

  template <> const char name_info<std::minus<double>>::left[] = "left";
  template <> const char name_info<std::minus<double>>::right[] = "right";
  template <> const char name_info<std::minus<double>>::serialize[] = "subtractor";

  template <> const char name_info<std::multiplies<double>>::left[] = "left";
  template <> const char name_info<std::multiplies<double>>::right[] = "right";
  template <> const char name_info<std::multiplies<double>>::serialize[] = "multiplier";

  template <> const char name_info<std::divides<double>>::left[] = "left";
  template <> const char name_info<std::divides<double>>::right[] = "right";
  template <> const char name_info<std::divides<double>>::serialize[] = "divider";

  template <> const char name_info<std::modulus<int>>::left[] = "left";
  template <> const char name_info<std::modulus<int>>::right[] = "right";
  template <> const char name_info<std::modulus<int>>::serialize[] = "modulo";

  template <> const char name_info<std::greater<double>>::left[] = "left";
  template <> const char name_info<std::greater<double>>::right[] = "right";
  template <> const char name_info<std::greater<double>>::serialize[] = "ascendingcomparator";

  template <> const char name_info<std::greater_equal<double>>::left[] = "left";
  template <> const char name_info<std::greater_equal<double>>::right[] = "right";
  template <> const char name_info<std::greater_equal<double>>::serialize[] = "strictascendingcomparator";

  template <> const char name_info<std::equal_to<double>>::left[] = "left";
  template <> const char name_info<std::equal_to<double>>::right[] = "right";
  template <> const char name_info<std::equal_to<double>>::serialize[] = "equalitycomparator";

  template <> const char name_info<std::negate<double>>::left[] = "left";
  template <> const char name_info<std::negate<double>>::right[] = "right";
  template <> const char name_info<std::negate<double>>::serialize[] = "signinverter";

  

  Previous::Previous (ParentProcess* parent, const std::string& name, double i_val)
  : FatProcess (name),
    _input(this, "input", i_val),
    _output(this, "output", i_val),
    _action(this, "action", *this, i_val),
    _coupling(&_input, ACTIVATION, &_action, ACTIVATION)
  {
    init_unary_couplings(_input, _output, _action, _coupling);
    finalize_construction (parent, name);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Previous::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:previous");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", _input.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

  int
  Incr::init_incr (bool isModel)
  {
    graph_add_edge (this, &_state);
    set_is_model (isModel);
    return 1;
  }

  Incr::Incr (ParentProcess* parent, const std::string& name, bool isModel) :
      FatProcess (name),
      _delta (this, "delta", 1),
      _state (this, "state", 0)
  {
    init_incr (isModel);
    finalize_construction (parent, name);
  }

  Incr::~Incr ()
  { 
    remove_state_dependency (get_parent (), &_state);
    graph_remove_edge (this, &_state);
  }

  void
  Incr::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
       remove_state_dependency (get_parent (), &_state);
    }

    add_state_dependency (parent, &_state);
    FatProcess::set_parent (parent); 
  }

  void
  Incr::impl_activate ()
  {
    _state.set_value (_state.get_value () + _delta.get_value (), true);
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Incr::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:incr");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

  AdderAccumulator::AdderAccumulatorAction::AdderAccumulatorAction (ParentProcess* parent, const std::string& name,
                                                                    AdderAccumulator& aa) :
      Action (parent, name), _aa(aa)
  {
    finalize_construction (parent, name);
  }

  void
  AdderAccumulator::AdderAccumulatorAction::impl_activate ()
  {
    double input = _aa._input.get_value ();
    double clamp_min = _aa._clamp_min.get_value ();
    double clamp_max = _aa._clamp_max.get_value ();
    double value = _aa._result.get_value ();
    value += input;
    if (clamp_min < clamp_max) {
      value = value < clamp_min ? clamp_min : value;
      value = value > clamp_max ? clamp_max : value;
    }
    _aa._result.set_value (value, true);
  }

  AdderAccumulator::AdderAccumulator (ParentProcess* parent, const std::string& name, double input, double clamp_min,
                                      double clamp_max)
  : FatProcess (name),
    _input (this, "input", input),
    _clamp_min (this, "clamp_min", clamp_min),
    _clamp_max (this, "clamp_max", clamp_max),

    _result (this, "result", clamp_min >= clamp_max ? input : input < clamp_min ? clamp_min : input > clamp_max ? clamp_max : input),
    _action (this, name + "_action", *this),
    _c_input (&_input, ACTIVATION, &_action, ACTIVATION)
  {
    _c_input.disable ();
    graph_add_edge (&_input, &_action);
    graph_add_edge (&_action, &_result);
    finalize_construction (parent, name);
  }

  AdderAccumulator::~AdderAccumulator ()
  {
    remove_state_dependency (get_parent (), &_action);
    graph_remove_edge (&_input, &_action);
    graph_remove_edge (&_action, &_result);
  }

  void
  AdderAccumulator::set_parent (ParentProcess* parent)
  { 
    /* in case of re-parenting remove edge dependency in graph */
    if (get_parent ()){
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
  AdderAccumulator::serialize (const std::string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:adderaccumulator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("input", _input.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_min", _clamp_min.get_value ());
    AbstractSerializer::serializer->float_attribute ("clamp_max", _clamp_max.get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif

}

