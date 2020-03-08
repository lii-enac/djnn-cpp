/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018-2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "syshook/syshook.h"
#include "syshook/global_mutex.h"
#include "syshook/main_loop.h"
#include "syshook/djnn_time_manager.h"

#define DBG_MUTEX 0
#include <iostream>
#include "utils/debug.h"

namespace djnn
{

  static bool __module_initialized = false;

  void
  init_syshook()
  {
    if (__module_initialized == false) {
      __module_initialized = true;
      djnn::loadedModules.push_back ("syshook");
      init_global_mutex();
      MainLoop::instance ();
      //DjnnTimeManager::instance().activate ();
      MainLoop::instance ().add_external_source(&DjnnTimeManager::instance());
    }
    DjnnTimeManager::instance().cleanup ();
  }

  void
  clear_syshook()
  {
  }

}
