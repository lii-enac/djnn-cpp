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

#include "int_property.h"
#include "../serializer/serializer.h"
#include "../error.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  AbstractIntProperty::set_value (double v, bool propagate)
  {
    set_value((int)v, propagate);
  }

  void
  AbstractIntProperty::set_value (int v, bool propagate)
  {
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  AbstractIntProperty::set_value (bool v, bool propagate)
  {
    set_value((int)(v ? 1 : 0), propagate);
  }

  void
  AbstractIntProperty::set_value (const string &v, bool propagate)
  {
    int oldVal = get_value();
    try {
      if (!v.empty ()) {
        set_value((int)stoi (v), propagate);
      }
    }
    catch (const std::invalid_argument& ia) {
      get_ref_value() = oldVal;
      warning (this, "failed to convert the string \"" + v + "\" into an int property value\n");
    }
  }

  void
  AbstractIntProperty::set_value (Process* v, bool propagate)
  {
    warning (this, "undefined conversion from Component to Int\n");
  }

  void
  AbstractIntProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << get_value() << " ]";
  }

  void
  IntProperty::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:IntProperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  IntPropertyProxy::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:IntProperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process*
  IntProperty::clone ()
  {
    return new IntProperty (get_value());
  }

  Process*
  IntPropertyProxy::clone ()
  {
    return new IntPropertyProxy (get_ref_value()); // FIXME
  }
}
