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
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#include "text_property.h"
#include "../serializer/serializer.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  AbstractTextProperty::set_value (int v, bool propagate)
  {
    set_value(to_string (v), propagate);
  }

  void
  AbstractTextProperty::set_value (double v, bool propagate)
  {
    set_value(to_string (v), propagate);
  }

  void
  AbstractTextProperty::set_value (bool v, bool propagate)
  {
    set_value (v ? string("true") : string("false"), propagate);
  }

  void
  AbstractTextProperty::set_value (const string &v, bool propagate)
  {
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  AbstractTextProperty::set_value (Process* v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Component to Text\n";
  }

  void
  AbstractTextProperty::dump (int level) {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << get_ref_value() << " ]" ;
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

  Process* 
  TextProperty::clone ()
  {
    return new TextProperty (value);
  }

  void
  TextPropertyProxy::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:textproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process* 
  TextPropertyProxy::clone ()
  {
    return new TextPropertyProxy (value);
  }
}
