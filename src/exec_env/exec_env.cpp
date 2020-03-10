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

#include "exec_env/exec_env.h"
#include "exec_env/global_mutex.h"
#include "exec_env/main_loop.h"
#include "exec_env/djnn_time_manager.h"

#define DBG_MUTEX 0
#include <iostream>
#include "utils/debug.h"

namespace djnn
{

  static bool __module_initialized = false;
  Process* mainloop;

  void
  init_exec_env()
  {
    if (__module_initialized == false) {
      __module_initialized = true;
      djnn::loadedModules.push_back ("exec_env");
      init_global_mutex();
      mainloop = &MainLoop::instance ();
      //DjnnTimeManager::instance().activate ();
      MainLoop::instance ().add_external_source(&DjnnTimeManager::instance());
      setlocale(LC_NUMERIC, "C");
    }
    DjnnTimeManager::instance().cleanup ();
  }

  void
  clear_exec_env()
  {
  }

}
