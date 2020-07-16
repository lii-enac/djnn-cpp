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

//#include "core/utils/utils-dev.h" // __to_string

#include <string>

#ifdef DJNN_CRAZYFLIE
extern "C" {
  void DJNN_DEBUG_PRINT(const char* fmt);
}
#endif

namespace djnn
{
  class CoreProcess;

  int __error (CoreProcess *p, const char* msg, const char* ctxinfo=nullptr);
  int __error (CoreProcess *p, const std::string& msg, const char* ctxinfo=nullptr);
  void __warning (CoreProcess *p, const char* msg, const char* ctxinfo=nullptr);
  void __warning (CoreProcess *p, const std::string& msg, const char* ctxinfo=nullptr);
  void __info (const std::string& msg);
  void __debug (const char* file, const char* function, const char* lineno);
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
  #define error(p,msg) __ignore_unused( djnn::__error(p, msg, __FILE__  ":" __djnn_str1(__LINE__) ) & djnn::__exit(0))
  //#define error(p,msg) __ignore_unused( __error(p, std::string(msg)+ " " __FILE__ ":" DJNN_STR1(__LINE__))) & __exit(0))
  //#define warning(p,msg) __warning(p, std::string(msg)+" "+__FILE__+":"+djnn::__to_string(__LINE__))
  #define warning(p,msg) djnn::__warning(p, msg, __FILE__ ":" __djnn_str1(__LINE__) )
  #define info(msg) djnn::__info(msg)

  class Context {
  public:
    static Context* instance ();
    void new_line (int line, const std::string& filename) { _line = line; _filename = filename; }
    int line () { return _line; }
    const std::string& filename () { return _filename; }
  private:
    Context () : _line (-1), _filename ("") {}
    static Context* _instance;
    int _line;
    std::string _filename;
  };
}
