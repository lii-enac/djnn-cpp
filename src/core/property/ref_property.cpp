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

#include <cstdint>

#include "ref_property.h"

#include "core/serializer/serializer.h"
#include "core/utils/djnn_dynamic_cast.h"
#include "core/utils/error.h"
#include "core/utils/utils-dev.h" // __to_string

// #include <sstream>
// #include <boost/lexical_cast.hpp>

#if !defined(DJNN_NO_DEBUG)
// #include <_iostream>
#endif

namespace djnn {

using namespace djnnstl;

CoreProcess*
getRef (CoreProcess* p)
{
    AbstractRefProperty* rp = djnn_dynamic_cast<AbstractRefProperty*> (p);
    if (rp != nullptr)
        return rp->get_value ();
    else
        warning (p, "getRef only works on reference properties");
    return nullptr;
}

void
setRef (CoreProcess* p, CoreProcess* v)
{
    AbstractRefProperty* rp = djnn_dynamic_cast<AbstractRefProperty*> (p);
    if (rp != nullptr)
        rp->set_value (v, true);
    else
        warning (p, "setRef only works on reference properties");
}

string
RefProperty::get_string_value ()
{
    // std::ostringstream address;
    // address << (void const *) value;
    // return address.str ();

    // return boost::lexical_cast<string>(value);

    return __to_string (_value);
}

string
RefProperty::get_string_value () const
{
    return __to_string (_value);
}

double
RefProperty::get_double_value ()
{
    // warning (this, "undefined double value for ref property");
    auto i = reinterpret_cast<std::uintptr_t> (_value);
    return i;
}
double
RefProperty::get_double_value () const
{
    auto i = reinterpret_cast<std::uintptr_t> (_value);
    return i;
}

void
RefProperty::set_value (int v, bool propagate)
{
    warning (this, "undefined conversion from Int to Component");
}

void
RefProperty::set_value (double v, bool propagate)
{
    warning (this, "undefined conversion from Double to Component");
}

void
RefProperty::set_value (bool v, bool propagate)
{
    warning (this, "undefined conversion from Bool to Component");
}

void
RefProperty::set_value (const string& v, bool propagate)
{
    warning (this, "undefined conversion from Text to Component");
}

void
RefProperty::set_value (CoreProcess* v, bool propagate)
{
    _value = v;
    if (is_activable () && propagate) {
        notify_activation ();
        notify_parent ();
    }

    _is_null.set_value (v == nullptr, true);
}

#ifndef DJNN_NO_SERIALIZE
void
RefProperty::serialize (const string& format)
{

    string buf;

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:refproperty");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), get_value (), buf);
    AbstractSerializer::serializer->text_attribute ("value", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

CoreProcess*
RefProperty::find_child_impl (const string& path)
{
    if (path.empty ())
        return this;
    if (path.compare ("..") == 0)
        return this->get_parent ();
    size_t found   = path.find_first_of ('/');
    string key     = path;
    string subpath = "";
    if (found != string::npos) {
        key     = path.substr (0, found);
        subpath = path.substr (found + 1, path.size () - 1);
    }
    if (key.compare ("$value") == 0) {
        return _value->find_child_impl (subpath);
        // auto * fpv = dynamic_cast<FatProcess*>(_value);
        // if(fpv)
        //   return fpv->find_child_impl (subpath);
    }
    if (key.compare ("is_null") == 0) {
        return _is_null.find_child_impl (subpath);
    }
    return nullptr;
}

#ifndef DJNN_NO_DEBUG
void
RefProperty::dump (int level)
{
    loginfonocr ((get_parent () ? get_parent ()->find_child_name (this) : get_name ()) + " [ " + __to_string (_value) + " ]");
}
#endif

RefProperty*
RefProperty::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new RefProperty (nullptr, name, _value);
    origs_clones[this] = res;
    return res;
}
} // namespace djnn
