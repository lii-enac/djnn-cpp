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

#include "ref_property.h"
#include "../serializer/serializer.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  RefProperty::set_value (int v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Int to Component\n";
  }

  void
  RefProperty::set_value (double v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Double to Component\n";
  }

  void
  RefProperty::set_value (bool v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Bool to Component\n";
  }

  void
  RefProperty::set_value (const string &v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Text to Component\n";
  }

  void
  RefProperty::set_value (Process* v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate)
      notify_activation ();
  }

  void
  RefProperty::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:refproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::compute_path (get_parent (), get_value (), buf);
    AbstractSerializer::serializer->text_attribute ("value", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  RefProperty::dump (int level) {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << value << " ]" ;
  }

  Process* 
  RefProperty::clone ()
  {
    return new RefProperty (value);
  }
}
