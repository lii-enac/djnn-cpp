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
#include "core/serializer/serializer.h"
#include "core/utils/error.h"
#include "core/execution/graph.h"

#include "core/utils/djnn_dynamic_cast.h"

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
//#include <__iostream>
#endif

namespace djnn
{
  using namespace std;

  bool
  getBool (CoreProcess *p)
  {
    AbstractBoolProperty *bp = djnn_dynamic_cast<AbstractBoolProperty*> (p);
    if (bp != nullptr)
      return bp->get_value();
    else
      warning (p, "getBool only works on boolean properties");
    return false;
  }

  void
  setBool (CoreProcess* p, bool v)
  {
    AbstractBoolProperty *bp = djnn_dynamic_cast<AbstractBoolProperty*> (p);
    if (bp != nullptr)
      bp->set_value(v, true);
    else
      warning (p, "setBool only works on boolean properties");
  }

  AbstractBoolProperty::AbstractBoolProperty (ParentProcess* parent, const std::string& name, int notify_mask)
  : AbstractProperty (parent, name, notify_mask),
    _true (this, "true"),
    _false (this, "false")
  {
    Graph::instance ().add_edge (this, &_true);
    Graph::instance ().add_edge (this, &_false);
  };

  AbstractBoolProperty::~AbstractBoolProperty ()
  {
    Graph::instance ().remove_edge (this, &_false);
    Graph::instance ().remove_edge (this, &_true);
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
        _true.notify_activation ();
      else
        _false.notify_activation ();
    }
  }

  void
  AbstractBoolProperty::set_value (const std::string& v, bool propagate)
  {
    if (v.compare ("true") == 0) {
      //get_ref_value() = true;
      set_value (true, propagate);
    } else if (v.compare ("false") == 0) {
      set_value (false, propagate);
    } else {
#ifndef DJNN_NO_DEBUG
      warning (this, "failed to convert " + v + " into a boolean value");
#endif
    }
  }

  void
  AbstractBoolProperty::set_value (CoreProcess* v, bool propagate)
  {
    warning (this, "undefined conversion from Process to Boolean\n");
  }

#ifndef DJNN_NO_DEBUG
  void
  AbstractBoolProperty::dump (int level)
  {
    loginfonofl ( (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) + " [ " + get_string_value() + " ]");
  }
#endif

#ifndef DJNN_NO_SERIALIZE
  void
  AbstractBoolProperty::serialize (const std::string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:BoolProperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("value", get_value () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  FatProcess*
  BoolProperty::clone ()
  {
    return new BoolProperty (nullptr, get_name (), get_value());
  }

  FatProcess*
  BoolPropertyProxy::clone ()
  {
    return new BoolPropertyProxy (nullptr, get_name (), get_ref_value());
  }
}
