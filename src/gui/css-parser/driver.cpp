/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */
#include <fstream>
#include <sstream>

#include "driver.h"
#include "scanner.h"

namespace css
{
  using namespace djnn;

  Driver::Driver () :
      _lexer (nullptr), _root (nullptr)
  {

  }

  bool
  Driver::parse_stream (std::istream& in, const std::string& name, Process *p)
  {
    _root = p;
    stream = name;

    Scanner scanner (&in);
    this->_lexer = &scanner;

    Parser parser (*this);
    return (parser.parse () == 0);
  }

  bool
  Driver::parse_file (const std::string &filename, Process *p)
  {
    std::ifstream in (filename.c_str ());
    if (!in.good ())
      return false;
    return parse_stream (in, filename, p);
  }

  bool
  Driver::parse_string (const std::string &input, const std::string& sname, Process *p)
  {
    std::istringstream iss (input);
    return parse_stream (iss, sname, p);
  }

  Process*
  Driver::get_parent ()
  {
    return _root;
  }

  void
  Driver::error (const class location& l, const std::string& m)
  {
    std::cerr << l << ": " << m << std::endl;
  }

  void
  Driver::error (const std::string& m)
  {
    std::cerr << m << std::endl;
  }

}
