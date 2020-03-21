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
#include "core/serializer/serializer.h"
#include "core/utils/error.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  int
  getInt (Process* p)
  {
    IntProperty *ip = dynamic_cast<IntProperty*> (p);
    if (ip != nullptr)
      return ip->get_value();
    else
      warning (p, "getInt only works on integer properties");
    return 0;
  }

  void
  setInt (Process* p, int v)
  {
    IntProperty *ip = dynamic_cast<IntProperty*> (p);
    if (ip != nullptr)
      ip->set_value (v, true);
    else
      warning (p, "setInt only works on integer properties");
  }

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
  AbstractIntProperty::set_value (const std::string& v, bool propagate)
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
    cout << (get_parent () ? get_parent ()->find_component_name(this) : get_name ()) << " [ " << get_value() << " ]";
  }

  void
  IntProperty::serialize (const std::string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:IntProperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  IntPropertyProxy::serialize (const std::string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:IntProperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process*
  IntProperty::clone ()
  {
    return new IntProperty (nullptr, get_name (), get_value());
  }

  Process*
  IntPropertyProxy::clone ()
  {
    return new IntPropertyProxy (nullptr, get_name (), get_ref_value());
  }
}
