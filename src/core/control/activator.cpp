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
  

  Activator::Activator (ParentProcess* parent, const djnn::string& name, CoreProcess* action) :
      FatProcess (name), _action (action)
  {
    if (_action == nullptr) {
      error  (this, djnn::string("action not found in activator ") + name);
      return;
    }
    finalize_construction (parent, name);
  }

  Activator::Activator (ParentProcess* parent, const djnn::string& name, CoreProcess* action, const djnn::string& spec) :
      Activator (parent, name, action->find_child_impl (spec))
  {
  }

  Activator::~Activator ()
  {
  }

#ifndef DJNN_NO_SERIALIZE
  void
  Activator::serialize (const djnn::string& format) {

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
