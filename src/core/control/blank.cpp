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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "blank.h"

#include "core/serializer/serializer.h"

namespace djnn {
#if !defined(DJNN_NO_SERIALIZE)
void
Blank::serialize (const string& format)
{

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:blank");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

FatProcess*
Blank::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto res           = new Blank (nullptr, name);
    origs_clones[this] = res;
    return res;
}
} // namespace djnn
