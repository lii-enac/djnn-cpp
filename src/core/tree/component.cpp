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

#include "component.h"

#ifndef DJNN_NO_SERIALIZE
#include "core/serializer/serializer.h"
#endif

namespace djnn {
Component*
Component::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto* clone = new Component (nullptr, name);
    impl_clone_children (origs_clones, clone);
    origs_clones[this] = clone;
    return clone;
}

#ifndef DJNN_NO_SERIALIZE
void
Component::serialize (const string& format)
{

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("core:Component");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif

} // namespace djnn
