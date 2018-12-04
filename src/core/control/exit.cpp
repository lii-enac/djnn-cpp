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
#include "../syshook/main_loop.h"
#include "../tree/process.h"
#include "../serializer/serializer.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Exit::Exit (Process* p, const string &name, int value, bool is_model) :
      Process (p, name)
  {
    _model = is_model;
    _value = new IntProperty (this, "value", value);
    Process::finalize ();
  }

  Exit::~Exit ()
  {
    if (_value) {delete _value; _value = nullptr;}
  }

  void
  Exit::activate ()
  {
    MainLoop::instance ().deactivation ();
    //exit (_value->get_value ());
  }

  void
  Exit::serialize (const string& format) {

    AbstractSerializer::pre_serialize(this, format);

    AbstractSerializer::serializer->start ("core:exit");
    AbstractSerializer::serializer->text_attribute ("id", _name);
    AbstractSerializer::serializer->int_attribute ("value", _value->get_value ());
    AbstractSerializer::serializer->text_attribute ("model", _model ? "true" : "false");
    AbstractSerializer::serializer->end ();

    AbstractSerializer::post_serialize(this);
  }

}
