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
//#include "core/utils/utils-dev.h"
//#include "xml.h"

namespace djnn
{
  void init_xml ();
  void clear_xml ();
  FatProcess* load_from_XML (const string& uri);
  FatProcess* load_from_XML_once (const string& uri);
  FatProcess* loadFromXML (const string& uri);
}


