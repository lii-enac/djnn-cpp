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


#include "core-dev.h"
#include "core/syshook/syshook.h"
#include "core/syshook/djnn_time_manager.h"
#include "core/utils/ext/remotery/Remotery.h"

#include <locale.h>

namespace djnn
{

  std::vector<string> loadedModules;
  std::vector<StructureObserver*> structure_observer_list;

  static bool __module_initialized = false;

  Remotery* rmt;

  void
  init_core ()
  {
    
    if (__module_initialized == false) {
      init_global_mutex();
      __module_initialized = true;

      djnn::loadedModules.push_back("core");
      MainLoop::instance ();
      //DjnnTimeManager::instance().activate ();
      MainLoop::instance ().add_external_source(&DjnnTimeManager::instance());
     
      setlocale(LC_NUMERIC, "C");
    }

    rmtSettings* settings = rmt_Settings();
    settings->reuse_open_port = RMT_TRUE;
    rmt_CreateGlobalInstance(&rmt);
    DjnnTimeManager::instance().cleanup ();
  }

  void
  clear_core ()
  {
    XML::clear_xml_parser ();
    rmt_DestroyGlobalInstance(rmt);
  }
}
