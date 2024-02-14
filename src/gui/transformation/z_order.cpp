#include "gui/transformation/z_order.h"

#include "display/abstract_display.h"
#include "display/display-dev.h" // DisplayBackend
#include "display/window.h"
#include "gui/abstract_backend.h"
#include "gui/backend.h"

namespace djnn {
ZOrder::ZOrder (CoreProcess* parent, const string& name, int z)
    : AbstractGObj (parent, name), _z (this, "z", z), _c_z ()
{
    finalize_construction (parent, name);
}

void
ZOrder::impl_activate ()
{
    AbstractGObj::impl_activate ();
    auto* damaged = get_frame ()->damaged ();
    _c_z.init (&_z, ACTIVATION, damaged, ACTIVATION);
    _c_z.enable ();
}

void
ZOrder::impl_deactivate ()
{
    AbstractGObj::impl_deactivate ();
    _c_z.disable ();
}

void
ZOrder::draw ()
{
    auto _frame = get_frame ();
    if (somehow_activating () && DisplayBackend::instance ()->window () == _frame) {
        Backend::instance ()->load_zorder (_z.get_value ());
    }
}

FatProcess*
ZOrder::impl_clone (map<const CoreProcess*, CoreProcess*>& origs_clones) const
{
    auto res           = new ZOrder (nullptr, get_name (), _z.get_value ());
    origs_clones[this] = res;
    impl_clone_properties (res, origs_clones);
    return res;
}

} // namespace djnn