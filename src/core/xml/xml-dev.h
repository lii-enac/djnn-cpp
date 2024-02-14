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

//#include "core/ontology/process.h"
//#include "core/utils/utils-dev.h"
//#include "xml.h"

#include "core/utils/containers/string.h"

namespace djnn
{
  void init_xml ();
  void clear_xml ();

  class FatProcess;
  FatProcess* load_from_XML (const djnnstl::string& uri);
  FatProcess* load_from_XML_once (const djnnstl::string& uri);
  FatProcess* loadFromXML (const djnnstl::string& uri);
}


