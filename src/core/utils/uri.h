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
#include <map>

namespace djnn {
  class URI {
  public:
    static Process* find_by_uri (const std::string& uri);
    static void add_uri (const std::string& prefix, Process* p);
  private:
    static std::map<std::string, Process*> uri_manager;
  };
}
