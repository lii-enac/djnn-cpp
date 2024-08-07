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

#include "exit.h"

#include "exec_env/main_loop.h"
// #include "ontology/process.h"
#include "core/serializer/serializer.h"

namespace djnn {

Exit::Exit (CoreProcess* parent, const string& name, int value, bool isModel)
    : FatProcess (name),
      _value (this, "value", value)
{
    set_is_model (isModel);
    finalize_construction (parent, name);
}

Exit::~Exit ()
{
}

void
Exit::impl_activate ()
{
    MainLoop::instance ().deactivate ();
}

#ifndef DJNN_NO_SERIALIZE
void
Exit::serialize (const string& format)
{

    AbstractSerializer::pre_serialize (this, format);

    AbstractSerializer::serializer->start ("exec_env:exit");
    AbstractSerializer::serializer->text_attribute ("id", get_name ());
    AbstractSerializer::serializer->int_attribute ("value", _value.get_value ());
    AbstractSerializer::serializer->text_attribute ("model", is_model () ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize (this);
}
#endif
} // namespace djnn
