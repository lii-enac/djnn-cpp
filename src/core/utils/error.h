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

#include <stdlib.h> // exit
/*
extern "C" {
#if __MINGW64__
void exit(int status) ;
#else
void exit(int status) ; // noexcept;
#endif
}
*/

// #include "core/utils/utils-dev.h" // __to_string
#include "containers.h"

#ifdef DJNN_CRAZYFLIE
extern "C" {
void DJNN_DEBUG_PRINT (const char* fmt);
}
#endif

namespace djnn {
class CoreProcess;

int        djnn__error (const CoreProcess* p, const char* msg, const char* ctxinfo = nullptr);
int        djnn__error (const CoreProcess* p, const djnnstl::string& msg, const char* ctxinfo = nullptr);
void       djnn__warning (const CoreProcess* p, const char* msg, const char* ctxinfo = nullptr);
void       djnn__warning (const CoreProcess* p, const djnnstl::string& msg, const char* ctxinfo = nullptr);
void       djnn__info (const djnnstl::string& msg, bool cr, const char* ctxinfo = nullptr);
void       djnn__debug (const char* file, const char* function, const char* lineno);
inline int djnn__exit (int ret) {
    exit (ret);
    return 1;
}

/** Turn off -Wunused-result for a specific function call */
#define __ignore_unused(M) \
    if (1 == ((int)M)) {   \
        ;                  \
    }

#ifndef __djnn_str1
#define __djnn_str(x)  #x
#define __djnn_str1(x) __djnn_str (x)
#endif

#ifndef __DJNN_FUNCTION__
#ifdef __PRETTY_FUNCTION__
#define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
#else
#define __DJNN_FUNCTION__ __FUNCTION__
#endif
#endif

// #define error(p,msg) __ignore_unused( __error(p, string(msg)+" " +__FILE__+":"+djnn::__to_string(__LINE__)) & __exit(0))
#define djnn_error(p, msg) __ignore_unused (djnn::djnn__error (p, msg, __FILE__ ":" __djnn_str1 (__LINE__)) & djnn::djnn__exit (0))
// #define error(p,msg) __ignore_unused( __error(p, string(msg)+ " " __FILE__ ":" DJNN_STR1(__LINE__))) & __exit(0))
// #define warning(p,msg) __warning(p, string(msg)+" "+__FILE__+":"+djnn::__to_string(__LINE__))
#define djnn_warning(p, msg) djnn::djnn__warning (p, msg, __FILE__ ":" __djnn_str1 (__LINE__))
#define loginfo(msg)         djnn::djnn__info (msg, true, __FILE__ ":" __djnn_str1 (__LINE__))
#define loginfonocr(msg)     djnn::djnn__info (msg, false, __FILE__ ":" __djnn_str1 (__LINE__))
#define loginfonofl(msg)     djnn::djnn__info (msg, true)

#define error(p, msg)   djnn_error (p, msg)
#define warning(p, msg) djnn_warning (p, msg)
#define warn_once(p, msg) { static int once=0; if(!once) { djnn_warning (p, msg); once=1; } }

class Context {
  public:
    static Context* instance ();
    void            new_line (int line, const djnnstl::string& filename) {
        _line     = line;
        _filename = filename;
    }
    void parser_info (int begline, int begcol, int endline, int endcol, const djnnstl::string& filename) {
        _line     = begline;
        _begcol   = begcol;
        _endline  = endline;
        _endcol   = endcol;
        _filename = filename;
    }
    int line () const { return _line; }
    int begline () const { return _line; }
    int begcol () const { return _begcol; }
    int endline () const { return _endline; }
    int endcol () const { return _endcol; }

    const djnnstl::string& filename () const { return _filename; }

  private:
    Context ()
        : _line (-1), _filename ("") {}
    static Context* _instance;
    int             _line;
    int             _begcol;
    int             _endline;
    int             _endcol;
    djnnstl::string _filename;
};
} // namespace djnn
