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

#include "bool_property.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  BoolProperty::BoolProperty (Process *p, const string& n, bool v) :
      AbstractProperty (p, n), value (v)
  {
    _type = Boolean;
    _true = std::shared_ptr<Process> (new Spike (this, "true"));
    _false = std::shared_ptr<Process> (new Spike (this, "false"));
    Process::finalize ();
  }

  BoolProperty::BoolProperty (bool v) :
      AbstractProperty (), value (v)
  {
    _type = Boolean;
    _true = std::shared_ptr<Process> (new Spike (this, "true"));
    _false = std::shared_ptr<Process> (new Spike (this, "false"));
  }

  void
  BoolProperty::set_value (int v, bool propagate)
  {
    v == 0 ? set_value (false, propagate) : set_value (true, propagate);
  }

  void
  BoolProperty::set_value (double v, bool propagate)
  {
    v == 0 ? set_value (false, propagate) : set_value (true, propagate);
  }

  void
  BoolProperty::set_value (bool v, bool propagate)
  {
    value = v;
    if (is_activable () && propagate) {
      notify_activation ();
      if (v)
        _true->notify_activation ();
      else
        _false->notify_activation ();
    }
  }

  void
  BoolProperty::set_value (const string &v, bool propagate)
  {
    if (v.compare ("true") == 0) {
      value = true;
      set_value (true, propagate);
    } else if (v.compare ("false") == 0) {
      set_value (false, propagate);
    } else
      cerr << "Warning: failed to convert " << v << " into a boolean value";
  }

  void
  BoolProperty::set_value (Process* v, bool propagate)
  {
    cerr << "Warning: undefined conversion from Component to Boolean\n";
  }

  void
  BoolProperty::dump (int level)
  {
    cout << _name << " [ " << value << " ]";
  }
}
