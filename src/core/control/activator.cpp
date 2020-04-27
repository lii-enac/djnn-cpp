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
#include "core/utils/error.h"
#include "core/serializer/serializer.h"

namespace djnn
{
  using namespace std;

  Activator::Activator (Process* parent, const std::string& name, Process* action) :
      Process (name), _action (action)
  {
    if (_action == nullptr) {
      error  (this, std::string("action not found in activator ") + name);
      return;
    }
    finalize_construction (parent, name);
  }

  Activator::Activator (Process* parent, const std::string& name, Process* action, const std::string& spec) :
      Process (name)
  {
    _action = action->find_child (spec);
    if (_action == nullptr) {
      error  (this, std::string("action not found in activator ") + name);
      return;
    }
    finalize_construction (parent, name);
  }

  Activator::~Activator ()
  {
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Activator::serialize (const std::string& format) {

    string buf;

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:activator");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::compute_path (get_parent (), _action, buf);
    AbstractSerializer::serializer->text_attribute ("action", buf);
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }
#endif

}
