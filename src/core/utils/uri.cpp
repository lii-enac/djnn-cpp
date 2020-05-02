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

namespace djnn
{

  std::map<std::string, FatProcess*> URI::uri_manager;

  void
  URI::add_uri (const std::string& prefix, FatProcess* p)
  {
    uri_manager.insert (std::pair<std::string, FatProcess*> (prefix, p));
  }

  FatProcess*
  URI::find_by_uri (const std::string& uri)
  {
    std::size_t found = uri.find("://");
    if (found == std::string::npos)
      return nullptr;
    std::map<std::string, FatProcess*>::iterator it;
    it = uri_manager.find (uri.substr (0, found));
    if (it == uri_manager.end ())
      return nullptr;
    return it->second->find_child (uri.substr (found + 3, std::string::npos));
  }
}
