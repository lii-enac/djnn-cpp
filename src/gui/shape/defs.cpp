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

#include "gui/shape/defs.h"

#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "gui/abstract_backend.h"
#include "gui/backend.h"
#include "gui/shape/sdf.h"

namespace djnn {
Defs::Defs (CoreProcess* parent, const string& n)
    : Container (parent, n)
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
Defs::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    /*Defs* newd = new Defs (nullptr, get_name ());

    for (auto c : _children) {
      newd->add_child (c->clone (), this->find_child_name(c));
    }

    return newd;*/

    auto* clone = new Defs (nullptr, get_name ());
    for (auto c : _children) {
        auto cclone = c->impl_clone (origs_clones);
        // origs_clones[c] = cclone;
        clone->add_child (cclone, this->find_child_name (c));
    }
    origs_clones[this] = clone;
    return clone;
}
} // namespace djnn
