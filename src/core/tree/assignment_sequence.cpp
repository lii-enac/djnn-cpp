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

#include "core/control/assignment.h"
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

  AssignmentSequence::AssignmentSequence (ParentProcess* parent, const string& name, bool isModel) :
      Container (parent, name)
  {
    set_is_model (isModel);
    finalize_construction (parent, name);
  }

  void
  AssignmentSequence::impl_activate ()
  {
    for (auto c : _children) {
      c->activate ();
    }
  }

  void
  AssignmentSequence::add_child (FatChildProcess* c, const string& name)
  {
    if (c == nullptr)
      return;
    Assignment *a = dynamic_cast<Assignment*> (c);
    if (a != nullptr) {
      graph_add_edge (this, a->get_dst());
    }
    _children.push_back (c);
    /* WARNING should we authorize multiple parenthood? */
    if (c->get_parent () != nullptr && c->get_parent () != this) {
      c->get_parent ()->remove_child (c);
    }
    c->set_parent (this);
    add_symbol (name, c);
  }

  void
  AssignmentSequence::post_activate ()
  {
    post_activate_auto_deactivate ();
  }

  AssignmentSequence::~AssignmentSequence ()
  {
    for (auto c : _children) {
      Assignment *a = dynamic_cast<Assignment*> (c);
      if (a != nullptr) {
        graph_remove_edge (this, a->get_dst());
      }
    }
  }

#ifndef DJNN_NO_SERIALIZE
  void
  AssignmentSequence::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:assignmentsequence");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());

    for (auto c : _children)
        c->serialize (format);

    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif
}
