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

#include "spike.h"

#include "core/serializer/serializer.h"

namespace djnn {

#if !defined(DJNN_NO_SERIALIZE)
void
Spike::serialize (const string& format)
{
    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:spike");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

Spike*
Spike::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new Spike (nullptr, name);
    origs_clones[this] = res;
    return res;
}
} // namespace djnn
