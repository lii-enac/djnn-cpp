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

#include <stdexcept>

#include "double_property.h"
#include "core/serializer/serializer.h"
#include "core/utils/error.h"

#include "core/utils/djnn_dynamic_cast.h"


namespace djnn
{
  

  double
  getDouble (CoreProcess* p)
  {
    DoubleProperty *dp = djnn_dynamic_cast<DoubleProperty*> (p);
    if (dp != nullptr)
      return dp->get_value();
    else
      warning (p, "getDouble only works on double properties");
    return 0;
  }

  void
  setDouble (CoreProcess* p, double v)
  {
    DoubleProperty *dp = djnn_dynamic_cast<DoubleProperty*> (p);
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
  AbstractDoubleProperty::set_value (const string& v, bool propagate)
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
  AbstractDoubleProperty::set_value (CoreProcess* v, bool propagate)
  {
    warning (this, "undefined conversion from Process to Double\n");
  }

#ifndef DJNN_NO_DEBUG
  void
  AbstractDoubleProperty::dump (int level)
  {
    loginfonofl ( (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) + " [ " + get_string_value() + " ]");
    //std::cout << (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) << " [ " << get_value() << " ]";
  }
#endif

#ifndef DJNN_NO_SERIALIZE
  void
  AbstractDoubleProperty::serialize (const djnn::string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:doubleproperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->float_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  DoubleProperty*
  DoubleProperty::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto res = new DoubleProperty (nullptr, get_name (), get_value());
    origs_clones[this] = res;
    return res;
  }

  DoublePropertyProxy*
  DoublePropertyProxy::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto res = new DoublePropertyProxy (nullptr, get_name (), get_ref_value(), _notify_mask);
    origs_clones[this] = res;
    return res;
  }
}
