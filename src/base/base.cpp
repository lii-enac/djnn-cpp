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


#include "base.h"

namespace djnn
{
  
  static bool __module_initialized = false;

  void
  init_base () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("base");
      
    }
  }

  void
  clear_base ()
  {
  }

}

extern "C" {
  void djnn_init_base () { djnn::init_base (); }
  void djnn_clear_base () { djnn::clear_base (); }
}

