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

  void
  BoolPropertyProxy::init_BoolProperty ()
  {
    _true = new Spike (this, "true");
    _false = new Spike (this, "false");
    Graph::instance ().add_edge (this, _true);
    Graph::instance ().add_edge (this, _false);
  }

  AbstractBoolProperty::~AbstractBoolProperty ()
  {
    if(_false) {
      Graph::instance ().remove_edge (this, _false);
      delete _false;
    }
    if(_true) {
      Graph::instance ().remove_edge (this, _true);
      delete _true;
    }
  }

  void
  AbstractBoolProperty::set_value (double v, bool propagate)
  {
    set_value((bool)(v!=0.0), propagate);
  }

  void
  AbstractBoolProperty::set_value (int v, bool propagate)
  {
    set_value((bool)(v!=0), propagate);
  }

  void
  AbstractBoolProperty::set_value (bool v, bool propagate)
  {
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      if (v)
        _true->notify_activation ();
      else
        _false->notify_activation ();
    }
  }

  void
  AbstractBoolProperty::set_value (const string &v, bool propagate)
  {
    if (v.compare ("true") == 0) {
      //get_ref_value() = true;
      set_value (true, propagate);
    } else if (v.compare ("false") == 0) {
      set_value (false, propagate);
    } else
      cerr << "Warning: failed to convert " << v << " into a boolean value";
  }

  void
  AbstractBoolProperty::set_value (Process* v, bool propagate)
  {
    warning (this, "undefined conversion from Component to Boolean\n");
  }

  void
  AbstractBoolProperty::dump (int level)
  {
    cout << (_parent ? _parent->find_component_name(this) : _name) << " [ " << get_value() << " ]";
  }

  void
  BoolProperty::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:BoolProperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("value", get_value () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  void
  BoolPropertyProxy::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:BoolProperty");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->text_attribute ("value", get_value () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process*
  BoolProperty::clone ()
  {
    return new BoolProperty (get_value());
  }

  Process*
  BoolPropertyProxy::clone ()
  {
    return new BoolPropertyProxy (get_ref_value()); // FIXME
  }
}
