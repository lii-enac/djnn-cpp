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

namespace djnn
{
  using namespace std;

  void
  Spike::pre_activate ()
  {
    if (_parent == 0 || _parent->get_state () == activated)
      _activation_state = activating;
  }

  void
  Spike::activate ()
  {
    if (_activation_state == activating)
      notify_activation ();
  }

  void
  Spike::post_activate ()
  {
    _activation_state = deactivated;
  }

}
