#include "analytical_picking.h"

#include "analytical_picking_context.h"
#include "core/utils/ext/remotery/Remotery.h"
#include "display/window.h"

// #include "core/utils/error.h"
// #include "core/utils/iostream.h"
// #include "utils/debug.h"

namespace djnn {
AnalyticalPicking::AnalyticalPicking (Window* win)
    : Picking (win)
{
}

AnalyticalPicking::~AnalyticalPicking ()
{
}

void
AnalyticalPicking::init ()
{
}

AbstractGShape*
AnalyticalPicking::pick (double x, double y)
{
    rmt_BeginCPUSample (analytical_pick, RMTSF_None);
    FatProcess*           p = _win->holder ();
    PickAnalyticalContext pac;
    pac.x                  = x;
    pac.y                  = y;
    pac.half_outline_width = 0; // FIXME should be default value
    pac.filled             = true;
    pac.outlined           = false;
    pac.clipped            = false;

    AbstractGShape* picked = p->pick_analytical (pac);
    // std::cerr << picked << __FL__;
    rmt_EndCPUSample ();
    return picked;
}

void
AnalyticalPicking::add_pick_shape (PickUI* pshape, bool cache)
{
}

} // namespace djnn
