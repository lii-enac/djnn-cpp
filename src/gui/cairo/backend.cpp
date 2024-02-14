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

#include "gui/backend.h"

#include "cairo_backend.h"

namespace djnn {
class Backend::Impl
{
  public:
    CairoBackend* cairo_backend;
};

Backend::Impl* Backend::_instance;

AbstractBackend*
Backend::instance ()
{
    return _instance->cairo_backend;
}

void
Backend::init ()
{
    if (_instance != nullptr)
        return;
    _instance                = new Impl ();
    _instance->cairo_backend = CairoBackend::instance ();
}
} // namespace djnn
