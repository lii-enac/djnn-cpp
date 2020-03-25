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

namespace djnn
{
  using namespace std;

  bool
  Spike::pre_activate ()
  {
    if (get_parent () == 0 || get_parent ()->get_activation_state () == ACTIVATED) {
      set_activation_state (ACTIVATING);
      return true;
    }
    return false;
  }

  void
  Spike::impl_activate ()
  {
    if ( get_activation_state () == ACTIVATING)
      notify_activation ();
  }

  void
  Spike::post_activate ()
  {
    set_activation_state (DEACTIVATED);
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
