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
#include <iostream>

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

  int
  __error (Process* p, const std::string& msg)
  {
    std::cerr << std::endl << std::endl << "djnn - ERROR: " << (p ? p->get_name () : "") << " - " << msg << std::endl << std::endl;
    
    if (p) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << std::endl;
    
    return 1;
    //exit (0);
  }

  void
  __warning (Process *p, const std::string& msg)
  {
    std::cerr << std::endl << std::endl << "djnn - WARNING: " << (p ? p->get_name () : "") << " - " << msg << std::endl << std::endl;
    
    if (p) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << std::endl;
    
  }
}
