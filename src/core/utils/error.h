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

#include <string>


namespace djnn
{

  std::string __to_string(int x);

  inline int __exit(int ret) { exit(ret); return 1; }
  /** Turn off -Wunused-result for a specific function call */
  #define __ignore_unused(M) if(1==((int)M)){;}
  #define error(p,msg) __ignore_unused( __error(p, std::string(msg)+" " +__FILE__+":"+djnn::__to_string(__LINE__)) & __exit(0))
  #define warning(p,msg) __warning(p, std::string(msg)+" "+__FILE__+":"+djnn::__to_string(__LINE__))
  #define _DBG __debug(std::string(__FILE__)+":"+djnn::__to_string(__LINE__));

  int __error (Process *p, const std::string& msg);
  void __warning (Process *p, const std::string& msg);
  void __debug (const std::string& msg);

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
