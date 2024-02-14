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

#include "remote_property.h"

#include "core/serializer/serializer.h"
#include "core/utils/error.h"
#include "core/utils/to_string.h"

#include "core/utils/djnn_dynamic_cast.h"

namespace djnn
{
  using namespace djnnstl;

  double
  AbstractRemoteProperty::get_double_value ()
  {
    try {
      double r = stof (get_ref_value());
      return r;
    }
    catch (const std::invalid_argument& ia) {
      warning (this, " - stof - undefined double value in dist property");
      return 0;
    }
  }

  double
  AbstractRemoteProperty::get_double_value () const
  {
    try {
      double r = stof (get_ref_value());
      return r;
    }
    catch (const std::invalid_argument& ia) {
      warning (nullptr, " - stof - undefined double value in dist property");
      return 0;
    }
  }

  void
  AbstractRemoteProperty::set_value (int v, bool propagate)
  {
    set_value(djnnstl::to_string (v), propagate);
  }

  void
  AbstractRemoteProperty::set_value (double v, bool propagate)
  {
    set_value(djnnstl::to_string (v), propagate);
  }

  void
  AbstractRemoteProperty::set_value (bool v, bool propagate)
  {
    set_value (v ? string("true") : string("false"), propagate);
  }

  void
  AbstractRemoteProperty::set_value (const djnn::string& v, bool propagate)
  {
    _send = true;
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  AbstractRemoteProperty::set_incoming_value (const djnn::string& v, bool propagate)
  {
    _send = false;
    get_ref_value() = v;
    if (is_activable () && propagate) {
      notify_activation ();
      notify_parent ();
    }
  }

  void
  AbstractRemoteProperty::set_value (CoreProcess* v, bool propagate)
  {
    warning(this, "undefined conversion from Process to RemoteProperty");
  }

#ifndef DJNN_NO_DEBUG
  void
  AbstractRemoteProperty::dump (int level) {
    loginfonocr( (get_parent () ? get_parent ()->find_child_name(this) : get_name ()) + " [ " + get_ref_value() + " ]") ;
  }
#endif

#if !defined(DJNN_NO_SERIALIZE)
  void
  AbstractRemoteProperty::serialize (const djnn::string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:remote-property");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  FatProcess* 
  RemoteProperty::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto res = new RemoteProperty (nullptr, get_name (), _value);
    origs_clones[this] = res;
    return res;
  }

}
