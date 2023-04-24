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


namespace djnn {
  class URI {
  public:
    static CoreProcess* find_by_uri (const string& uri);
    static void add_uri (const string& prefix, CoreProcess* p);
  private:
    typedef map<string, CoreProcess*> uris_t;
    static uris_t uri_manager;
  };
}
