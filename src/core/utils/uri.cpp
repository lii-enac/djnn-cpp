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

#include "uri.h"

namespace djnn {
using namespace djnnstl;

URI::uris_t URI::uri_manager;

void
URI::add_uri (const string& prefix, CoreProcess* p)
{
    uri_manager.insert (uris_t::value_type (prefix, p));
}

CoreProcess*
URI::find_by_uri (const string& uri)
{
    std::size_t found = uri.find ("://");
    if (found == string::npos)
        return nullptr;
    uris_t::iterator it;
    it = uri_manager.find (uri.substr (0, found));
    if (it == uri_manager.end ())
        return nullptr;
    return it->second->find_child_impl (uri.substr (found + 3, string::npos));
}
} // namespace djnn
