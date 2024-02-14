/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "ode_backend.h"
#include "physics_backend.h"

namespace djnn {
class PhysicsBackend::Impl
{
  public:
    ODEBackend* ode_backend;
};

PhysicsBackend::Impl* PhysicsBackend::_instance;

AbstractPhysicsBackend*
PhysicsBackend::instance ()
{
    return _instance->ode_backend;
}

void
PhysicsBackend::init ()
{
    if (_instance != nullptr)
        return;
    dInitODE2 (0);
    dAllocateODEDataForThread (dAllocateMaskAll);
    _instance              = new Impl ();
    _instance->ode_backend = ODEBackend::instance ();
}
} // namespace djnn
