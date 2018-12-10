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
 *
 */

#include "double_property.h"
#include "../serializer/serializer.h"
#include "../error.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  DoubleProperty::set_value (int v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  DoubleProperty::set_value (double v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate) {
      notify_activation ();
      if( _notify_mask != notify_none) {
        get_parent () -> notify( _notify_mask );
      }
    }
  }

  void
  DoubleProperty::set_value (bool v, bool propagate)
  {
    value = v ? 1 : 0;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  DoubleProperty::set_value (const string &v, bool propagate)
  {

    int oldVal = value;
    try {

      if (!v.empty ()) {
        value = stof (v);
        if (is_activable () && propagate)
        notify_activation ();
      }

      
    }
    catch (const std::invalid_argument& ia) {
      value = oldVal;
      warning (this, "failed to convert the string \"" + v + "\" into a double property value\n");
    }
  }

  void
  DoubleProperty::set_value (Process* v, bool propagate)
  {
    warning (this, "undefined conversion from Component to Double\n");
  }

  void
  DoubleProperty::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:doubleproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  DoubleProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << value << " ]";
  }

  Process* DoubleProperty::clone ()
  {
    return new DoubleProperty (value);
  }
}
