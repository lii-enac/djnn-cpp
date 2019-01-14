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
#include "../serializer/serializer.h"

namespace djnn
{
  using namespace std;

  void
  Spike::pre_activate ()
  {
    //if (_parent == 0 || _parent->get_state () == activated)
    if (_parent == 0 || _parent->is_activated ())
      set_activating ();
  }

  void
  Spike::activate ()
  {
    if ( is_activating () )
      notify_activation ();
  }

  void
  Spike::post_activate ()
  {
    set_deactivated ();
  }

  void
  Spike::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:spike");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

  Process* 
  Spike::clone ()
  {
    return new Spike ();
  }
}
