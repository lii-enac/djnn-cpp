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

#if RMT_ENABLED
#include "core/utils/ext/remotery/Remotery.h"
#endif

//#include <locale.h>
//#include "core/utils/error.h"
#include "core/execution/graph.h"

//#include "utils/debug.h"
//#include <__iostream>

#include <string>
#include <vector>

#include <locale.h>


namespace djnn
{
  class FatProcess;
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

#if RMT_ENABLED
  Remotery* rmt;
#endif

  static bool __module_initialized = false;

  void
  init_core ()
  {    
    if (__module_initialized == false) {
      //printf("init_core\n");
      __module_initialized = true;
      //djnn::loadedModules.push_back("core");
      setlocale (LC_NUMERIC, "C");
      //Graph::instance (); // make sure an instance is built ASAP since some platform may not be able to build it globally
    }

  /* especially for unit test - make sure graph::_activation_deque is clean for each run */
  Graph::instance ().clear_activation ();

#if RMT_ENABLED
    rmtSettings* settings = rmt_Settings();
    if(settings) settings->reuse_open_port = RMT_TRUE;

    enum rmtError err = rmt_CreateGlobalInstance(&rmt);
    if(err) {
      //warning (nullptr, "Rmt error");
      //std::cerr << "rmt error " << err << __FL__;
    }
#endif
  }

  extern void
  delete_parentless_processes ();

  void
  clear_core ()
  {
    delete &Graph::instance (); // destructor will set Graph::_instance to nullptr since _instance is private
    //XML::clear_xml_parser ();
#if RMT_ENABLED
    //rmt_DestroyGlobalInstance(rmt); still a bug with opengl remotery
#endif
    //std::cerr << __PRETTY_FUNCTION__ << __FL__;
    delete_parentless_processes ();
    __module_initialized = false;
  }

  void
  graph_add_edge (CoreProcess* src, CoreProcess* dst)
  {
    Graph::instance ().add_edge (src, dst);
  }
  
  void
  graph_remove_edge (CoreProcess* src, CoreProcess* dst)
  {
    Graph::instance ().remove_edge (src, dst);
  }

  void
  graph_exec ()
  {
    Graph::instance ().exec ();
  }

}


// temporyary hack
#if DJNN_USE_FREERTOS
#include "core/xml/xml.h"

namespace djnn {
  void XML::clear_xml_parser () {}
}
#endif
