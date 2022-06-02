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
#include "core/core-dev.h" // graph add/remove edge

#include "core/utils/djnn_dynamic_cast.h"


namespace djnn
{
  

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

  AbstractBoolProperty::AbstractBoolProperty (ParentProcess* parent, const string& name, int notify_mask)
  : AbstractProperty (parent, name, notify_mask),
    _true (this, "true"),
    _false (this, "false")
  {
    graph_add_edge (this, &_true);
    graph_add_edge (this, &_false);
  };

  AbstractBoolProperty::~AbstractBoolProperty ()
  {
    graph_remove_edge (this, &_false);
    graph_remove_edge (this, &_true);
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
  AbstractBoolProperty::set_value (const string& v, bool propagate)
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
  AbstractBoolProperty::serialize (const string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:BoolProperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("value", get_value () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  FatProcess*
  BoolProperty::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto res = new BoolProperty (nullptr, get_name (), get_value());
    origs_clones[this] = res;
    return res;
  }

  FatProcess*
  BoolPropertyProxy::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto res = new BoolPropertyProxy (nullptr, get_name (), get_ref_value(), _notify_mask);
    origs_clones[this] = res;
    return res;
  }
}
