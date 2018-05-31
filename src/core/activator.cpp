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

#include "activator.h"
#include <iostream>

namespace djnn
{
  using namespace std;

  Activator::Activator (Process* parent, const string &name, Process* action) :
      Process (parent, name), _action (action)
  {
    if (_action == nullptr) {
      std::cerr << "Error: action not found in activator" << name << endl;;
      return;
    }
    Process::finalize ();
  }

  Activator::Activator (Process* parent, const string &name, Process* action, const string &spec) :
      Process (parent, name)
  {
    _action = action->find_component (spec);
    if (_action == nullptr) {
      std::cerr << "Error: action not found in activator" << name << endl;;
      return;
    }
    Process::finalize ();
  }

  Activator::~Activator ()
  {
  }
}
