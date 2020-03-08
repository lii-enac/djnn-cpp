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
      __module_initialized = true;
      djnn::loadedModules.push_back("core");
      setlocale(LC_NUMERIC, "C");
    }

    rmtSettings* settings = rmt_Settings();
    if(settings) settings->reuse_open_port = RMT_TRUE;
    rmt_CreateGlobalInstance(&rmt);
  }

  void
  clear_core ()
  {
    XML::clear_xml_parser ();
    rmt_DestroyGlobalInstance(rmt);
  }
}
