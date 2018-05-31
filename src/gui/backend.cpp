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

#include "backend.h"

#include "qt/qt_backend.h"
#include "qt/qt_mainloop.h"

namespace djnn
{
  QtBackend* Backend::_instance;

  AbstractBackend*
  Backend::instance ()
  {
    return _instance;
  }

  void
  Backend::init ()
  {
    if (_instance != nullptr)
      return;
  _instance = QtBackend::instance();
  QtMainloop::instance();
  }
}
