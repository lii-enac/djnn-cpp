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
 *
 */

#include "spike.h"
#include "core/serializer/serializer.h"

#include <iostream>
#include "utils/debug.h"

namespace djnn
{
  using namespace std;

  bool
  Spike::pre_activate ()
  {
    if (get_parent () == 0 || get_parent ()->somehow_activating ()) {
      set_activation_state (ACTIVATING);
      return true;
    }
    return false;
  }

  void
  Spike::post_activate ()
  {
    post_activate_auto_deactivate ();
  }

#if !defined(DJNN_NO_SERIALIZE)
  void
  Spike::serialize (const std::string& format) {
    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:spike");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

  Process* 
  Spike::clone ()
  {
    return new Spike (nullptr, get_name ());
  }
}
