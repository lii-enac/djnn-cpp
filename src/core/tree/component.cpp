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



namespace djnn
{
  Component*
  Component::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
  {
    auto * clone = new Component (nullptr, get_name ());
    for (auto c : _children) {
      auto cclone = c->impl_clone (origs_clones);
      clone->add_child (cclone , this->find_child_name(c));
    }
    origs_clones[this] = clone;
    return clone;
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Component::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:Component");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}

