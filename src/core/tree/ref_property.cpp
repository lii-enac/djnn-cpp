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
#include "core/serializer/serializer.h"

#include <sstream>
#include <iostream>

namespace djnn
{
  using namespace std;

  Process*
  getRef (Process* p)
  {
    RefProperty *rp = dynamic_cast<RefProperty*> (p);
    if (rp != nullptr)
      return rp->get_value();
    else
      warning (p, "getRef only works on reference properties");
    return nullptr;
  }

  void
  setRef (Process* p, Process *v)
  {
    RefProperty *rp = dynamic_cast<RefProperty*> (p);
    if (rp != nullptr)
      rp->set_value (v, true);
    else
      warning (p, "setRef only works on reference properties");
  }

  string
  RefProperty::get_string_value ()
  {
    std::ostringstream address;
    address << (void const *) value;
    return address.str ();
  }

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
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  RefProperty::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:refproperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), get_value (), buf);
    AbstractSerializer::serializer->text_attribute ("value", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process*
  RefProperty::find_component (const string &path)
  {
    if (path.empty ())
      return this;
    size_t found = path.find_first_of ('/');
    string key = path;
    string subpath = "";
    if (found != string::npos) {
      key = path.substr (0, found);
      subpath = path.substr (found + 1, path.size () - 1);
    }
    if (key.compare ("$value") == 0)
      return value->find_component (subpath);
    return nullptr;
  }

  void
  RefProperty::dump (int level) {
    cout << (get_parent () ? get_parent ()->find_component_name(this) : get_name ()) << " [ " << value << " ]" ;
  }

  Process* 
  RefProperty::clone ()
  {
    return new RefProperty (nullptr, get_name (), value);
  }
}
