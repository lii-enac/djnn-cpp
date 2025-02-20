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
Defs::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto* clone = new Defs (nullptr, name);
    impl_clone_children (origs_clones, clone);
    origs_clones[this] = clone;
    return clone;
}
} // namespace djnn
