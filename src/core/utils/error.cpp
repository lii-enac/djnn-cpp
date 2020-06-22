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

  int
  //__error (ParentProcess* parent, const std::string& msg, const char* ctxinfo)
  __error (CoreProcess* p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << std::endl << std::endl;
    if (p && p->debug_info ().lineno) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    if(ctxinfo)
      std::cerr << ctxinfo << ":";
    FatProcess * pp = dynamic_cast<FatProcess*>(p);
    std::cerr << "djnn - ERROR: " << (pp ? pp->get_name () : "") << " - " << msg;
    
    std::cerr << std::endl << std::endl;
    
    
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( msg ) ;
    DJNN_DEBUG_PRINT( "\n" ) ;
#endif
    return 1;
    //exit (0);
  }
  
  int
  __error (CoreProcess* p, const std::string& msg, const char* ctxinfo)
  {
    return __error(p, msg.c_str(), ctxinfo);
  }

  void
  __warning (CoreProcess *p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    auto & out = std::cerr;
    out << std::endl << std::endl;
    if (p && p->debug_info ().lineno) 
      out << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    if(ctxinfo)
      out << ctxinfo << ":";
    FatProcess * pp = dynamic_cast<FatProcess*>(p);
    out << "djnn - WARNING: " << (pp ? pp->get_name () : "") << " - " << msg;
    out << std::endl << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( msg ) ;
    DJNN_DEBUG_PRINT( "\n" ) ;
#endif
  }

  void
  __warning (CoreProcess* p, const std::string& msg, const char* ctxinfo)
  {
    __warning(p, msg.c_str(), ctxinfo);
  }

  void
  __debug (const char* file, const char* function, const char* line)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr <<  file << ":" << function << ":" << line << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    //DJNN_DEBUG_PRINT( std::string(file, function, line) ;
#endif
  }
}
