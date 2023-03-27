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
#include "core/utils/iostream.h"
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
  //__error (CoreProcess* parent, const string& msg, const char* ctxinfo)
  djnn__error (const CoreProcess* p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    auto & out = std::cerr;
    out << std::endl << std::endl;
    out << "djnn - ";
    out << "\033[0;33m";
    out << "ERROR: ";
    out << "\033[0m";
    out << "\"" << (p ? p->get_debug_name () : "") << "\" - " << msg;
    if (p && p->debug_info ().lineno) 
      out << " - " << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    else if(ctxinfo)
      out << " - " << ctxinfo << ":";
    
    out << std::endl << std::endl;
#endif

#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT( msg ) ;
    DJNN_DEBUG_PRINT( "\n" ) ;
#endif
    return 1;
    //exit (0);
  }
  
  int
  djnn__error (const CoreProcess* p, const string& msg, const char* ctxinfo)
  {
    return djnn__error(p, msg.c_str(), ctxinfo);
  }

  extern string __to_string(void*p);

  void
  djnn__warning (const CoreProcess *p, const char* msg, const char* ctxinfo)
  {
#ifndef DJNN_NO_DEBUG
    auto & out = std::cerr;
    out << std::endl << std::endl;
    out << "djnn - ";
    out << "\033[0;33m";
    out << "WARNING: ";
    out << "\033[0m";
    out << "\"" << (p ? p->get_debug_name () : "") << "\" - " << msg;
    if (p && p->debug_info ().lineno) 
      out << " - " << p->debug_info ().filepath << ":" <<  p->debug_info ().lineno << ":";
    else if(ctxinfo)
      out << " - " << ctxinfo << ":";
    
    out << std::endl << std::endl;
#endif
#ifdef DJNN_CRAZYFLIE
    DJNN_DEBUG_PRINT(  __to_string(p).c_str() ) ;
    DJNN_DEBUG_PRINT( msg ) ;
    DJNN_DEBUG_PRINT( "\n" ) ;
#endif
  }

  void
  djnn__warning (const CoreProcess* p, const string& msg, const char* ctxinfo)
  {
    djnn__warning(p, msg.c_str(), ctxinfo);
  }

  void
  djnn__info (const string& msg, bool cr, const char* ctxinfo)
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
    //DJNN_DEBUG_PRINT( string(file, function, line) ;
#endif
  }
}
