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


#include "animation.h"
#include "core/serializer/serializer.h"

namespace djnn
{
  
  static bool __module_initialized = false;

  void
  init_animation () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("animation");
      
    }
  }
  
  SlowInSlowOutInterpolator::SlowInSlowOutInterpolator (Process *parent, const std::string& name) :
        HermiteCurve (parent, name, 0, 1, 0, 0) {}

  void
  SlowInSlowOutInterpolator::serialize (const std::string& type)
  {

    AbstractSerializer::pre_serialize (this, type);

    AbstractSerializer::serializer->start ("animation:slowInslowoutinterpolator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);

  }

}
