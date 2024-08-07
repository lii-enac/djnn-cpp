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

// core/utils/error.h macros clash with Driver::error
#undef error
#undef warning

#include "core/ontology/process.h"
#include "core/utils/containers.h"

namespace css {
using djnnstl::string;

class Scanner;
class Driver {
  public:
    Driver ();

    string stream;
    bool   parse_stream (std::istream& in,
                         const string& name, djnn::FatProcess* p);
    bool   parse_string (const string& input,
                         const string& name, djnn::FatProcess* p);
    bool   parse_file (const string& filename, djnn::FatProcess* p);

    void error (const class location& l, const string& m);

    void              error (const string& m);
    djnn::FatProcess* get_parent ();
    Scanner*          _lexer;
    djnn::FatProcess* _root;
};

} // namespace css
