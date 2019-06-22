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

#include "double_property.h"
#include "../serializer/serializer.h"
#include "../utils/error.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  double
  getDouble (Process* p)
  {
    DoubleProperty *dp = dynamic_cast<DoubleProperty*> (p);
    if (dp != nullptr)
      return dp->get_value();
    else
      warning (p, "getDouble only works on double properties");
    return 0;
  }

  void
  setDouble (Process* p, double v)
  {
    DoubleProperty *dp = dynamic_cast<DoubleProperty*> (p);
    if (dp != nullptr)
      dp->set_value(v, true);
    else
      warning (p, "setDouble only works on double properties");
  }
  void
  AbstractDoubleProperty::set_value (int v, bool propagate)
  {
    set_value((double)v, propagate);
  }

  void
  AbstractDoubleProperty::set_value (double v, bool propagate)
  {
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  AbstractDoubleProperty::set_value (bool v, bool propagate)
  {
    set_value((double)(v ? 1 : 0), propagate);
  }

  void
  AbstractDoubleProperty::set_value (const string &v, bool propagate)
  {
    double oldVal = get_value();
    try {
      if (!v.empty ()) {
        set_value((double)stof (v), propagate);
      }
    }
    catch (const std::invalid_argument& ia) {
      get_ref_value() = oldVal;
      warning (this, "failed to convert the string \"" + v + "\" into a double property value\n");
    }
  }

  void
  AbstractDoubleProperty::set_value (Process* v, bool propagate)
  {
    warning (this, "undefined conversion from Component to Double\n");
  }

  void
  AbstractDoubleProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << get_value() << " ]";
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
  DoublePropertyProxy::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:doubleproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->float_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process*
  DoubleProperty::clone ()
  {
    return new DoubleProperty (get_value());
  }

  Process*
  DoublePropertyProxy::clone ()
  {
    return new DoublePropertyProxy (get_ref_value()); // FIXME
  }
}
