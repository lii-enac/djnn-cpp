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


//#include "core/utils/ext/remotery/Remotery.h"

//#include <locale.h>
//#include "core/utils/error.h"
#include "core/execution/graph.h"

//#include "utils/debug.h"
//#include <iostream>

#include <string>
#include <vector>


namespace djnn
{
  class Process;
  class StructureObserver;


  std::vector<std::string> loadedModules;
/*
  void
  module_loaded(const char*)
  {
    djnn::loadedModules.push_back("core");
  }
*/
  std::vector<StructureObserver*> structure_observer_list;
//  Remotery* rmt;

  static bool __module_initialized = false;

  void
  init_core ()
  {    
    if (__module_initialized == false) {
      __module_initialized = true;
      //djnn::loadedModules.push_back("core");
//      setlocale(LC_NUMERIC, "C");
      Graph::instance (); // make sure an instance is built ASAP since some platform may not be able to build it globally
    }

/*    rmtSettings* settings = rmt_Settings();
    if(settings) settings->reuse_open_port = RMT_TRUE;

    enum rmtError err = rmt_CreateGlobalInstance(&rmt);
    if(err) {
      //warning (nullptr, "Rmt error");
      //std::cerr << "rmt error " << err << __FL__;
    }
*/
  }

  void
  clear_core ()
  {
    //XML::clear_xml_parser ();
    //rmt_DestroyGlobalInstance(rmt); still a bug with opengl remotery
  }
}

extern "C" {
  void djnn_init_core () { djnn::init_core (); }
  void djnn_clear_core () { djnn::clear_core (); }
}

// temporyary hack
#if DJNN_USE_FREERTOS
#include "core/xml/xml.h"

namespace djnn {
  void XML::clear_xml_parser () {}
}
#endif
