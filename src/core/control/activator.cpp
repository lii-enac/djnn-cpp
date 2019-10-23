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
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */

#include "activator.h"
#include "../utils/error.h"
#include "../serializer/serializer.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  Activator::Activator (Process* parent, const string &name, Process* action) :
      Process (name), _action (action)
  {
    if (_action == nullptr) {
      error  (this, "action not found in activator " + name);
      return;
    }
    Process::finalize_construction (parent);
  }

  Activator::Activator (Process* parent, const string &name, Process* action, const string &spec) :
      Process (name)
  {
    _action = action->find_component (spec);
    if (_action == nullptr) {
      error  (this, "action not found in activator " + name);
      return;
    }
    Process::finalize_construction (parent);
  }

  Activator::~Activator ()
  {
  }

  void
  Activator::serialize (const string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:activator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _action, buf);
    AbstractSerializer::serializer->text_attribute ("action", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

}
