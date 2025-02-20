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

#include "gui/shape/group.h"

#include "display/abstract_display.h" // DisplayBackend::instance
#include "display/display-dev.h"      // DisplayBackend::instance
#include "gui/abstract_backend.h"
#include "gui/backend.h"

namespace djnn {
Group::Group (CoreProcess* parent, const string& n)
    : Container (parent, n), _gobj (nullptr)
{
    _gobj = new AbstractGObj (this, "_gobj");

    /* AbstractGObj do not have a call to finalize_construction */
    /* we have to add_child it here */
    _gobj->set_parent (this);

    finalize_construction (parent, n);

    /* and set get_state_dependency here AFTER setting the parent to "this" */
    _gobj->set_state_dependency (this->get_state_dependency ());
}

Group::~Group ()
{
    delete _gobj;
}

void
Group::impl_activate ()
{
    Container::impl_activate ();
    _gobj->impl_activate ();
}

void
Group::impl_deactivate ()
{
    _gobj->impl_deactivate ();
    Container::impl_deactivate ();
}

void
Group::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Container::draw ();
    }
}

Group*
Group::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones, const string& name) const
{
    auto* clone = new Group (nullptr, name);
    impl_clone_children (origs_clones, clone);
    origs_clones[this] = clone;
    return clone;
}

void
ZOrderedGroup::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->draw_z_ordered_group (this);
    }
}
} // namespace djnn
