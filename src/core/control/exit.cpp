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

#include "exit.h"
#include "../syshook/main_loop.h"
#include "../tree/process.h"

#include <iostream>

namespace djnn
{
  using namespace std;

  Exit::Exit (Process* p, const string &name, int value, bool is_model) :
      Process (p, name)
  {
    _model = is_model;
    _value = make_unique<IntProperty> (this, "value", value);
    Process::finalize ();
  }

  void
  Exit::activate ()
  {
    MainLoop::instance ().deactivation ();
    exit (_value->get_value ());
  }

}
