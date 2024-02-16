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

#include "driver.h"

#include <fstream>
#include <sstream>

#include "core/utils/iostream.h"
#include "scanner.h"

namespace css {
using namespace djnn;
using namespace djnnstl;

Driver::Driver ()
    : _lexer (nullptr), _root (nullptr)
{
}

bool
Driver::parse_stream (std::istream& in, const string& name, FatProcess* p)
{
    _root  = p;
    stream = name.c_str ();

    Scanner scanner (&in);
    this->_lexer = &scanner;
    Parser parser (*this);
    _lexer = nullptr; // scanner will not exist anymore when the method returns

    return (parser.parse () == 0);
}

bool
Driver::parse_file (const string& filename, FatProcess* p)
{
    std::ifstream in (filename.c_str ());
    if (!in.good ())
        return false;
    return parse_stream (in, filename, p);
}

bool
Driver::parse_string (const string& input, const string& sname, FatProcess* p)
{
    std::istringstream iss (input.c_str ());
    return parse_stream (iss, sname, p);
}

FatProcess*
Driver::get_parent ()
{
    return _root;
}

#if DJNN_STL_DJNN || DJNN_USE_DJNN_IOSTREAM
ostream&
operator<< (ostream& out, const location& l)
{
    out << "location TODO\n";
    return out;
}
#endif

void
Driver::error (const class location& l, const string& m)
{
#if DJNN_STL_DJNN || DJNN_USE_DJNN_IOSTREAM
    ostream& operator<< (ostream&, const location&);
#endif
    djnnstl::cerr << l << ": " << m << djnnstl::endl;
}

void
Driver::error (const string& m)
{
    djnnstl::cerr << m << djnnstl::endl;
}

} // namespace css
