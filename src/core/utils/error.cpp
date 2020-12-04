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

#include "core/ontology/process.h"

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
  djnn__error (CoreProcess* p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr << std::endl << std::endl;
    if (p && p->debug_info ().lineno) 
      std::cerr << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    else if(ctxinfo)
      std::cerr << ctxinfo << ":";
    auto * pp = p;
    std::cerr << "djnn - ERROR: " << (pp ? pp->get_debug_name () : "") << " - " << msg;
    
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
  djnn__error (CoreProcess* p, const std::string& msg, const char* ctxinfo)
  {
    return djnn__error(p, msg.c_str(), ctxinfo);
  }

  extern std::string __to_string(void*p);

  void
  djnn__warning (CoreProcess *p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    auto & out = std::cerr;
    out << std::endl << std::endl;
    if (p && p->debug_info ().lineno) 
      out << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    else if(ctxinfo)
      out << ctxinfo << ":";
    auto * pp = p;
    out << "djnn - WARNING: " << pp << " " << (pp ? pp->get_debug_name () : "") << " - " << msg;
    out << std::endl << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT(  __to_string(p).c_str() ) ;
    DJNN_DEBUG_PRINT( msg ) ;
    DJNN_DEBUG_PRINT( "\n" ) ;
#endif
  }

  void
  djnn__warning (CoreProcess* p, const std::string& msg, const char* ctxinfo)
  {
    djnn__warning(p, msg.c_str(), ctxinfo);
  }

  void
  djnn__info (const std::string& msg, bool cr, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    auto & out = std::cout;
    out << msg;
    if (cr) {
      if (ctxinfo) out << " " << ctxinfo;
      out << std::endl;
    }
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( msg.c_str() ) ;
    if (cr) {
      if (ctxinfo) DJNN_DEBUG_PRINT( msg.c_str() ) ;
      DJNN_DEBUG_PRINT( "\n" ) ;
    }
#endif
    
  }

  void
  djnn__debug (const char* file, const char* function, const char* line)
  {
#ifndef DJNN_NO_DEBUG
    std::cerr <<  file << ":" << function << ":" << line << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    //DJNN_DEBUG_PRINT( std::string(file, function, line) ;
#endif
  }
}
