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

#include "logic.h"
#include "core/serializer/serializer.h"

namespace djnn
{

#if NEW_OP

  template <> const char name_info<std::logical_and<bool>>::left[] = "left";
  template <> const char name_info<std::logical_and<bool>>::right[] = "right";
  template <> const char name_info<std::logical_and<bool>>::serialize[] = "and";
  
  template <> const char name_info<std::logical_or<bool>>::left[] = "left";
  template <> const char name_info<std::logical_or<bool>>::right[] = "right";
  template <> const char name_info<std::logical_or<bool>>::serialize[] = "or";
  
  template <> const char name_info<std::not_equal_to<bool>>::left[] = "left";
  template <> const char name_info<std::not_equal_to<bool>>::right[] = "right";
  template <> const char name_info<std::not_equal_to<bool>>::serialize[] = "xor";
  
  template <> const char name_info<std::logical_not<bool>>::left[] = "left";
  template <> const char name_info<std::logical_not<bool>>::right[] = "right";
  template <> const char name_info<std::logical_not<bool>>::serialize[] = "not";

#else

  And::And (Process *parent, const string &n, int l_val, int r_val) :
      BinaryOperator (parent, n)
  {
    _left = new BoolProperty (this, "left", l_val);
    _right = new BoolProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val && r_val);
    init_couplings (new AndAction (this, "action", _left, _right, _result));
    Process::finalize_construction (parent);
  }

  void
  And::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:and");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("left", dynamic_cast<BoolProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("right", dynamic_cast<BoolProperty*> (_right)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Or::Or (Process *parent, const string &n, int l_val, int r_val) :
      BinaryOperator (parent, n)
  {
    _left = new BoolProperty (this, "left", l_val);
    _right = new BoolProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val || r_val);
    init_couplings (new OrAction (this, "action", _left, _right, _result));
    Process::finalize_construction (parent);
  }

  void
  Or::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:or");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("left", dynamic_cast<BoolProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("right", dynamic_cast<BoolProperty*> (_right)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  XOr::XOr (Process *parent, const string &n, int l_val, int r_val) :
      BinaryOperator (parent, n)
  {
    _left = new BoolProperty (this, "left", l_val);
    _right = new BoolProperty (this, "right", r_val);
    _result = new BoolProperty (this, "result", l_val != r_val);
    init_couplings (new XOrAction (this, "action", _left, _right, _result));
    Process::finalize_construction (parent);
  }

  void
  XOr::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:xor");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("left", dynamic_cast<BoolProperty*> (_left)->get_value ());
    AbstractSerializer::serializer->int_attribute ("right", dynamic_cast<BoolProperty*> (_right)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }

  Not::Not (Process *parent, const string &n, int in_val) :
      UnaryOperator (parent, n)
  {
    _input = new BoolProperty (this, "input", in_val);
    _output = new BoolProperty (this, "output", !in_val);
    init_couplings (new NotAction (this, "action", _input, _output));
    Process::finalize_construction (parent);
  }

  void
  Not::serialize (const string& type) {
   
    AbstractSerializer::pre_serialize(this, type);

    AbstractSerializer::serializer->start ("base:not");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("input", dynamic_cast<BoolProperty*> (_input)->get_value ());
   
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);

  }
#endif
}

