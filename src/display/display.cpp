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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */


#include "display-priv.h"
#include "../core/tree/set.h"
#include "../core/uri.h"

namespace djnn
{
  
  static bool __module_initialized = false;

  Process *GPUs = nullptr;
  Process *Displays = nullptr;

  DisplayBackend::Impl* DisplayBackend::_instance;

  AbstractDisplay*
  DisplayBackend::instance ()
  {
    return _instance->backend;
  }

  void
  init_display () {
    if ( __module_initialized == false ) {
      //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
      GPUs = new Set (nullptr, "GPUs");
      GPUs->set_activated ();
      Displays = new Set (nullptr, "Displays");
      Displays->set_activated ();
      URI::add_uri ("gpu", GPUs);
      URI::add_uri ("display", Displays);
      //std::cerr << __FILE__ << " " << __LINE__ << std::endl;
      //p_init_display ();
      djnn::loadedModules.push_back("display");
      __module_initialized = true;
    }
  }  

}
