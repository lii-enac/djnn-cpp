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

  void
  init_display () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("display");
      GPUs = new Set (nullptr, "GPUs");
      GPUs->set_activated ();
      Displays = new Set (nullptr, "Displays");
      Displays->set_activated ();
      URI::add_uri ("gpu", GPUs);
      URI::add_uri ("display", Displays);
      p_init_display ();
      
    }
  }

}
