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

#include "text_property.h"

#include "core/serializer/serializer.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"
#include "core/utils/to_string.h"

#if !defined(DJNN_NO_DEBUG) || !defined(DJNN_NO_SERIALIZE)
#include "core/utils/iostream.h"
#endif

namespace djnn {

using namespace djnnstl;

static string null_string = "null string";

string
toString (const AbstractSimpleProperty& x)
{
    return x.get_string_value ();
}

string&
toString (CoreProcess* p)
{
    return getString (p);
}

string&
getString (CoreProcess* p)
{
    AbstractTextProperty* tp = djnn_dynamic_cast<AbstractTextProperty*> (p);
    if (tp != nullptr)
        return tp->get_value ();
    else
        warning (p, "getString only works on text/string properties");
    return null_string;
}

void
setString (CoreProcess* p, string& v)
{
    AbstractTextProperty* tp = djnn_dynamic_cast<AbstractTextProperty*> (p);
    if (tp != nullptr)
        tp->set_value (v, true);
    else
        warning (p, "setString only works on text/string properties");
}

double
AbstractTextProperty::get_double_value ()
{
    try {
        double r = stof (get_ref_value ());
        return r;
    } catch (const std::invalid_argument& ia) {
        warning (this, " - stof - undefined double value in text property");
        return 0;
    }
}

double
AbstractTextProperty::get_double_value () const
{
    try {
        double r = stof (get_ref_value ());
        return r;
    } catch (const std::invalid_argument& ia) {
        warning (nullptr, " - stof - undefined double value in text property");
        return 0;
    }
}

void
AbstractTextProperty::set_value (int v, bool propagate)
{
    set_value (djnnstl::to_string (v), propagate);
}

void
AbstractTextProperty::set_value (double v, bool propagate)
{
    set_value (djnnstl::to_string (v), propagate);
}

void
AbstractTextProperty::set_value (bool v, bool propagate)
{
    set_value (v ? string ("true") : string ("false"), propagate);
}

void
AbstractTextProperty::set_value (const string& v, bool propagate)
{
    get_ref_value () = v;
    if (is_activable () && propagate) {
        notify_activation ();
        notify_parent ();
    }
}

void
AbstractTextProperty::set_value (CoreProcess* v, bool propagate)
{
    warning (this, "undefined conversion from Process to Text");
}

#ifndef DJNN_NO_DEBUG
void
AbstractTextProperty::dump (int level)
{
    using namespace djnnstl;
    cout << (get_parent () ? get_parent ()->find_child_name (this) : get_name ()) << " [ " << get_ref_value () << " ]";
}
#endif

#if !defined(DJNN_NO_SERIALIZE)
void
AbstractTextProperty::serialize (const string& format)
{

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:textproperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->text_attribute ("value", get_value ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

FatProcess*
TextProperty::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new TextProperty (nullptr, get_name (), value);
    origs_clones[this] = res;
    return res;
}

FatProcess*
TextPropertyProxy::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    error (this, "*PropertyProxy should not be cloned");
    return nullptr;
}
} // namespace djnn
