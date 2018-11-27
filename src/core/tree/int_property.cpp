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

#include "int_property.h"
#include "../serializer/serializer.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  IntProperty::set_value (int v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  IntProperty::set_value (double v, bool propagate)
  {
    value = (int) v;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  IntProperty::set_value (bool v, bool propagate)
  {
    value = v ? 1 : 0;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  IntProperty::set_value (const string &v, bool propagate)
  {
    int oldVal = value;
    try {
      value = stoi (v);
      if (is_activable () && propagate)
        notify_activation ();
    }
    catch (const std::invalid_argument& ia) {
      value = oldVal;
      cerr << "Warning: failed to convert " << v << " into an integer value";
    }
  }

  void
  IntProperty::set_value (Process* v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Component to integer\n";
  }

  void
  IntProperty::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:intproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  IntProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << value << " ]";
  }

  Process*
  IntProperty::clone ()
  {
    return new IntProperty (value);
  }
}
