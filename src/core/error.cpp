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

#include "error.h"

namespace djnn
{
  Context* Context::_instance = nullptr;

  Context*
  Context::instance ()
  {
    if (_instance == nullptr)
      _instance = new Context ();
    return _instance;
  }

  void
  error (Process* p, const std::string &msg)
  {
    std::cerr << std::endl << std::endl << "djnn - ERROR: " << msg << std::endl << std::endl;
    if (p) {
      std::cerr << p->debug_info () << std::endl;
    }
    exit (0);
  }

  void
  warning (Process *p, const std::string &msg)
  {
    std::cerr << std::endl << std::endl << "djnn - WARNING: " << msg << std::endl << std::endl;
    if (p) {
      std::cerr << p->debug_info () << std::endl;
    }
  }
}
