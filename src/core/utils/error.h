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

#pragma once

#include "core/ontology/process.h"
#include "core/utils/utils-dev.h" // __to_string

#include <string>

namespace djnn
{
  inline int __exit(int ret) { exit(ret); return 1; }
  /** Turn off -Wunused-result for a specific function call */
  #define __ignore_unused(M) if(1==((int)M)){;}

  #define __djnn_str(x) #x
  #define __djnn_str1(x) __djnn_str(x)

  #ifndef __DJNN_FUNCTION__
    #ifdef __PRETTY_FUNCTION__
      #define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
    #else
      #define __DJNN_FUNCTION__ __FUNCTION__
    #endif
  #endif

  //#define error(p,msg) __ignore_unused( __error(p, std::string(msg)+" " +__FILE__+":"+djnn::__to_string(__LINE__)) & __exit(0))
  #define error(p,msg) __ignore_unused( __error(p, msg, __FILE__  ":" __djnn_str1(__LINE__) ) & __exit(0))
  //#define error(p,msg) __ignore_unused( __error(p, std::string(msg)+ " " __FILE__ ":" DJNN_STR1(__LINE__))) & __exit(0))
  //#define warning(p,msg) __warning(p, std::string(msg)+" "+__FILE__+":"+djnn::__to_string(__LINE__))
  #define warning(p,msg) __warning(p, msg, __FILE__ ":" __djnn_str1(__LINE__) )

  #ifndef DBG
  #ifdef DJNN_CRAZYFLIE
  #define DBG DJNN_DEBUG_PRINT(__FILE__ ":" __djnn_str1(__LINE__) "\n");
  #else
  #define DBG __debug(__FILE__, __DJNN_FUNCTION__, __djnn_str1(__LINE__));
  #endif
  #endif

  int __error (Process *p, const char* msg, const char* ctxinfo=nullptr);
  int __error (Process *p, const std::string& msg, const char* ctxinfo=nullptr);
  void __warning (Process *p, const char* msg, const char* ctxinfo=nullptr);
  void __warning (Process *p, const std::string& msg, const char* ctxinfo=nullptr);

  void __debug (const char* file, const char* function, const char* lineno);

  class Context {
  public:
    static Context* instance ();
    void new_line (int line, const std::string& filename) { _line = line; _filename = filename; };
    int line () { return _line; }
    const std::string& filename () { return _filename; }
  private:
    Context () : _line (-1), _filename ("") {}
    static Context* _instance;
    int _line;
    std::string _filename;
  };
}
