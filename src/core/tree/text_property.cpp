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

#include "text_property.h"
#include "../serializer/serializer.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  TextProperty::set_value (int v, bool propagate)
  {
    value = to_string (v);
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  TextProperty::set_value (double v, bool propagate)
  {
    value = to_string (v);
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  TextProperty::set_value (bool v, bool propagate)
  {
    value = v ? "true" : "false";
    if (is_activable () && propagate)
      notify_activation ();
    ;
  }

  void
  TextProperty::set_value (const string &v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  TextProperty::set_value (Process* v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Component to Text\n";
  }

  void
  TextProperty::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:textproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  TextProperty::dump (int level) {
    cout << _name << " [ " << value << " ]" ;
  }
}
