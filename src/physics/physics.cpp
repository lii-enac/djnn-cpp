/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */


#include "physics_backend.h"
// #include "core/cor__e.h"
#include "core/serializer/serializer.h"


namespace djnn
{
  extern std::vector<std::string> loadedModules;
  
  static bool __module_initialized = false;

  void
  init_physics () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back ("physics");
      PhysicsBackend::init ();
    }
  }
  void
  clear_physics () {
  }
}
