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

#include "gui/backend.h"

#include "display/abstract_display.h"
#include "exec_env/qt/qt_mainloop.h"
#include "qt_backend.h"

namespace djnn {
class Backend::Impl
{
  public:
    QtBackend* qt_backend;
};

Backend::Impl* Backend::_instance;

AbstractBackend*
Backend::instance ()
{
    return _instance->qt_backend;
}

void
Backend::init ()
{
    if (_instance != nullptr)
        return;
    _instance             = new Impl ();
    _instance->qt_backend = QtBackend::instance ();
    QtMainloop::instance ();
}
} // namespace djnn
