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
    if (is_activable () && propagate)
      notify_activation ();
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
      value = stof (v);
      if (is_activable () && propagate)
        notify_activation ();
    }
    catch (const std::invalid_argument& ia) {
      value = oldVal;
      cerr << "Warning: failed to convert " << v << " into a double value";
    }
  }

  void
  DoubleProperty::set_value (Process* v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Component to Double\n";
  }

  void
  DoubleProperty::dump (int level)
  {
    cout << _name << " [ " << value << " ]";
  }
}
