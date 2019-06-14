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

#include "ontology/process.h"

#include <string>

namespace djnn
{
  void error (Process *p, const std::string &msg);
  void warning (Process *p, const std::string &msg);

  class Context {
  public:
    static Context* instance ();
    void new_line (int line, const std::string &filename) { _line = line; _filename = filename; };
    int line () { return _line; }
    const std::string& filename () { return _filename; }
  private:
    Context () : _line (-1), _filename ("") {}
    static Context* _instance;
    int _line;
    std::string _filename;
  };
}
