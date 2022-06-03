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
#include "assignment_sequence.h"

#include "core/utils/error.h"
#include "core/utils/utils-dev.h"
#include "core/core-dev.h" // graph add/remove edge
#include "core/tree/component_observer.h"
#include "core/tree/structure_observer.h"

#include "core/serializer/serializer.h"

#include "gui/picking/analytical_picking_context.h"

#include "core/utils/ext/remotery/Remotery.h"

#include <algorithm>
//#include <boost/range/adaptor/reversed.hpp>

#include "core/utils/djnn_dynamic_cast.h"


#if !defined(DJNN_NO_DEBUG)
// #include <__iostream>

//#define DEBUG
//#define DEBUG_DEACTIVATE

#endif


namespace djnn
{

  /*Component*
  Componentimpl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto * clone = new Component (nullptr, get_name ());
    for (auto c : _children) {
      clone->add_child (c->clone (), this->find_child_name(c));
    }
    return clone;
  }*/

  Component*
  Component::impl_clone (map<CoreProcess*, CoreProcess*>& origs_clones)
  {
    auto * clone = new Component (nullptr, get_name ());
    for (auto c : _children) {
      auto cclone = c->impl_clone (origs_clones);
      //origs_clones[c] = cclone;
      clone->add_child (cclone , this->find_child_name(c));
    }
    origs_clones[this] = clone;
    return clone;
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Component::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:component");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}

