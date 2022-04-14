#pragma once

#include "core/property/int_property.h"
#include "core/property/double_property.h"
#include "core/property/text_property.h"
#include "core/property/bool_property.h"

namespace djnn {

  template <typename T>
  struct PropertyTrait {
  };

  template <>
  struct PropertyTrait<double> {
    using property_type = DoubleProperty;
    static double get_value (AbstractProperty* p) { return p->get_double_value (); }
  };
  template <>
  struct PropertyTrait<int> {
    using property_type = IntProperty;
    static int get_value (AbstractProperty* p) { return p->get_double_value (); }
  };
  template <>
  struct PropertyTrait<bool> {
    using property_type = BoolProperty;
    static bool get_value (AbstractProperty* p) { return p->get_double_value (); }
  };
  template <>
  struct PropertyTrait<string> {
    using property_type = TextProperty;
    static const string get_value (AbstractProperty* p) { return p->get_string_value (); }
  };

}
