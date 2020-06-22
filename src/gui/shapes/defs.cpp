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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "shapes.h"
#include "gui/backend.h"
#include "gui/abstract_backend.h"
#include "display/display.h"
#include "display/abstract_display.h"

#include "core/execution/graph.h"


namespace djnn
{
  Defs::Defs (ParentProcess* parent, const std::string& n) :
      Container (parent, n)
  {
    finalize_construction (parent, n);
  }

  Defs::~Defs ()
  {
  }

  void
  Defs::impl_activate ()
  {
    // No activation propagation
  }

  void
  Defs::impl_deactivate ()
  {
    // No deactivation propagation;
  }

  Defs* 
  Defs::clone ()
  {
    Defs* newd = new Defs (nullptr, get_name ());

    for (auto c : _children) {
      newd->add_child (c->clone (), this->find_child_name(c));
    }

    return newd;
  }
}
