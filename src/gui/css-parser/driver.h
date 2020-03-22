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

#include <string>
#include "core/core-dev.h"

// core/utils/error.h macros clash with Driver::error
#undef error
#undef warning

namespace css {

class Scanner;
class Driver
{
public:
    Driver();

    std::string stream;
    bool parse_stream (std::istream& in,
		      const std::string& name, djnn::Process *p);
    bool parse_string (const std::string& input,
		      const std::string& name, djnn::Process *p);
    bool parse_file (const std::string& filename, djnn::Process *p);

    void error (const class location& l, const std::string& m);

    void error (const std::string& m);
    djnn::Process* get_parent ();
    Scanner* _lexer;
    djnn::Process* _root;

};

}
