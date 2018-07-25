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

#include "blank.h"
#include "../serializer/serializer.h"
#include <iostream>
#include <string>

namespace djnn
{
  using namespace std;

  void
  Blank::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:blank");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
}
