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
#include "core/utils/utils-dev.h"
#include "xml.h"

#include <map>



class DJNComponentAttrs_Hash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static djnn::djn_XMLAttrHandler* djn_DJNComponentAttrsLookup (const char *str, unsigned int len);
};

typedef struct djn_ComponentArgs_t
{
  const char* id;
  int model;
} djn_ComponentArgs_t;

extern struct djn_ComponentArgs_t djn_ComponentArgs;
extern struct djn_PropertyAttrs
{
  const char* value;
} djn_PropertyAttrs;

extern struct djn_PropagatorArgs
{
  const char* in;
  const char* out;
} djn_PropagatorArgs;

extern struct djn_LibraryLoaderArgs
{
  const char *uri;
  int autonaming;
} djn_LibraryLoaderArgs;

extern struct djn_ModuleArgs
{
  const char *name;
} djn_ModuleArgs;

namespace djnn {
extern map<djnn::string, djnn::FatProcess*> djn__id_to_process;
}

extern void
djn__InitXMLLoaders ();


