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
#include "../serializer/serializer.h"
#include "../error.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  void
  BoolProperty::init_BoolProperty ()
  {
    _true = new Spike (this, "true");
    _false = new Spike (this, "false");
    Graph::instance ().add_edge (this, _true);
    Graph::instance ().add_edge (this, _false);
  }

  BoolProperty::BoolProperty (Process *p, const string& n, bool v, int notify_mask) :
      AbstractProperty (p, n, notify_mask), value (v)
  {
    init_BoolProperty ();
    Process::finalize ();
  }

  BoolProperty::BoolProperty (bool v) :
      AbstractProperty (), value (v)
  {
    init_BoolProperty ();
  }

  BoolProperty::~BoolProperty ()
  {
    if(_false) {
      Graph::instance ().remove_edge (this, _false);
      delete _false;
      _false = nullptr;
    }
    if(_true) {
      Graph::instance ().remove_edge (this, _true);
      delete _true;
      _true = nullptr;
    }
  }

  void
  BoolProperty::set_value (int v, bool propagate)
  {
    set_value((bool)(v!=0.0), propagate);
  }

  void
  BoolProperty::set_value (double v, bool propagate)
  {
    set_value((bool)(v!=0), propagate);
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
    warning (this, "undefined conversion from Component to Boolean\n");
  }

  void
  BoolProperty::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:boolproperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("value", get_value () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  BoolProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << value << " ]";
  }

  Process* 
  BoolProperty::clone () 
  {
    return new BoolProperty (value);
  }
}
