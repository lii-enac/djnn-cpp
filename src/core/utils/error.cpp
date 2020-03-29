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

#if !defined(DJNN_NO_DEBUG)
#include <iostream>
#endif

#ifdef DJNN_CRAZYFLIE
extern "C" {
  void DJNN_DEBUG_PRINT(const char* fmt);
}
#endif

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

  std::string
  __to_string(int x)
  {
    char tab[256];
    int i=255;
    tab[i]=0;
    --i;
    tab[i]='0';
    while (x) {
      tab[i] = '0'+(x%10);
      x/=10;
      --i;
    }
    return std::string(&tab[i+1]); 
  }

  int
  __error (Process* p, const std::string& msg)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << std::endl << std::endl << "djnn - ERROR: " << (p ? p->get_name () : "") << " - " << msg << std::endl << std::endl;
    
    if (p) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT((msg+"\n").c_str());
#endif
    return 1;
    //exit (0);
  }

  void
  __warning (Process *p, const std::string& msg)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << std::endl << std::endl << "djnn - WARNING: " << (p ? p->get_name () : "") << " - " << msg << std::endl << std::endl;
    
    if (p) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( (msg+"\n").c_str());
#endif
  }

  void
  __debug (const std::string& msg)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr <<  msg << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( (msg+"\n").c_str());
#endif
  }
}
